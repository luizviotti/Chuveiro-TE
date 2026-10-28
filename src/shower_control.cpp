#include "shower_control.h"
#include "config.h"

Controller* Controller::instancia = nullptr;

Controller::Controller():
    modo(ModoOperacao::MIX_ONLY),

    pid(Config::KP_MIX, Config::KI_MIX, Config::KD_MIX,
        Config::KP_HEAT, Config::KI_HEAT, Config::KD_HEAT,
        temp_mista, output_mix, output_heat, setpoint //input = temperatura medida, setpoint = temperatura escolhida (limitada)
        ),

    ssr(Config::PIN_SSR, 0, Config::CICLOS_ANALISADOS_PADRAO),

    leitor_temp(Config::PIN_TEMP_SENSOR, temp_fria, temp_quente, temp_mista),

    valvulas(output_mix, input_vazao, Config::PIN_SERVO_QF, Config::PIN_SERVO_VAZAO),

    display(Config::LCD_ADDR, Config::COLUNAS_DISPLAY, Config::LINHAS_DISPLAY),

    leitor_vazao(Config::PIN_FLUX_SENSOR),

    pin_pot_temp(Config::PIN_TEMP_POT),

    pin_pot_vazao(Config::PIN_FLUX_POT),

    pin_flux_sensor(Config::PIN_FLUX_SENSOR),

    pin_ON_OFF(Config::PIN_ON_OFF)

    {}

void Controller::begin(){
    instancia = this;

    display.init();
    display.backlight();

    pid.begin();
    ssr.begin();
    leitor_temp.begin();
    valvulas.begin();
    leitor_vazao.begin();

    pinMode(pin_ON_OFF, INPUT_PULLUP);
    attachInterrupt(pin_ON_OFF, wrapperChave, FALLING);

    desliga(); //o sistema inicia desligado: válvulas fechadas e resistência desligada
}

void Controller::run(){
    //os sensores continuam sendo lidos com o sistema desligado, assim ao ligar as leituras já estão atualizadas
    leitor_vazao.atualizarLeitura();
    leitor_temp.atualizarTemperatura();

    bool estado = estado_atual; //cópia local: estado_atual pode mudar na ISR durante o loop
    if(estado != ultimo_estado){
        ultimo_estado = estado;
        if(estado){ liga(); }
        else { desliga(); }
    }

    if(!estado) { return; } //sistema desligado não faz nada

    //lendo input de temperatura e vazão
    input_vazao = analogRead(pin_pot_vazao)/4095.0f * 100.0f; //valor percentual (ADC de 12 bits: 0 a 4095)

    input_temp = 20.0f + analogRead(pin_pot_temp)/4095.0f * 25.0f; //20c -> leitura mínima | 25c -> Incremento máximo | 45c -> Temperatura máxima

    atualizarSetpoint();

    //define modo de operação
    definirModoOperacao();

    //Computar PID
    pid.computar(modo);

    //atualizar variável compartilhada com a isr
    if(modo == ModoOperacao::HEAT_ASSIST){
        ssr.setNumeroCiclos(output_heat);
    }
    //Atualizar Válvulas
    valvulas.atualizarValvulas();

    //Atualizar display
    atualizarDisplay();

}

void Controller::atualizarDisplay(){
    //display.clear() a cada loop causa flicker e trava o loop no I2C. Atualiza no máximo a cada INTERVALO_DISPLAY_MS
    //e sobrescreve a linha inteira (16 colunas) com espaços no lugar de limpar.
    if(millis() - ultima_atualizacao_display < Config::INTERVALO_DISPLAY_MS){ return; }
    ultima_atualizacao_display = millis();

    display.setCursor(0,0);
    display.printf("T:%4.1f/%4.1fC %c ", setpoint, temp_mista, modo == HEAT_ASSIST ? 'R' : 'M'); //mostra o alvo efetivo
    display.setCursor(0,1);
    display.printf("Vazao: %5.1f%%   ", input_vazao); //LCD HD44780 não tem 'ã'; '%%' imprime '%'
}

void Controller::atualizarSetpoint(){
    setpoint = input_temp;
    //só usa temp_fria se ela for confiável; sem leitura válida o sistema já está em MIX_ONLY sem resistência
    if(leitor_temp.leituraValida() && setpoint < temp_fria){
        setpoint = temp_fria;
    }
}

void Controller::definirModoOperacao(){
    ModoOperacao novo_modo = modo;
    float diff = setpoint - temp_quente;
    bool seguro = leitor_vazao.vazaoSegura() && leitor_temp.leituraValida();

    //setpoint - temp_quente <= 0 -> fonte quente dá conta
    if(!seguro || diff <= -Config::MODULO_TOLERANCIA){
        novo_modo = MIX_ONLY;
    }
    //setpoint - temp_quente >= 0 -> fonte quente insuficiente
    else if(diff >= Config::MODULO_TOLERANCIA){
        novo_modo = HEAT_ASSIST;
    }
    //entre -tolerância e +tolerância mantém o modo atual (histerese)

    if(novo_modo == modo){ return; }
    modo = novo_modo;

    if(modo == MIX_ONLY){
        ssr.disable();
        output_heat = 0;
        pid.setModeMix(); //output_mix parte de 100 (válvula quente aberta): transição sem salto
    }
    else{
        valvulas.abrirValvulaQuente();
        output_heat = 0;
        ssr.setNumeroCiclos(0);
        pid.setModeHeat();
        ssr.enable();
    }
}

void IRAM_ATTR Controller::chave(){
    //debounce: botões mecânicos geram vários FALLING por toque, o que alternaria o estado várias vezes
    unsigned long agora = millis();
    if(agora - ultimo_toque < Config::DEBOUNCE_MS){ return; }
    ultimo_toque = agora;
    estado_atual = !estado_atual; 
}

void IRAM_ATTR Controller::wrapperChave(){
    if(instancia) instancia->chave();
}

void Controller::liga(){
    modo = MIX_ONLY;
    ssr.disable();
    output_heat = 0;
    pid.setModeMix();

    display.clear();
    ultima_atualizacao_display = 0;
}

void Controller::desliga(){

    //desliga o controle de potência da resistência
    ssr.disable(); 
    pid.desligar(); //em MANUAL o PID não sobrescreve os outputs zerados abaixo

    //zera as variaveis de controle
    output_heat = 0;
    output_mix = 0;
    input_vazao = 0;

    //fecha as válvulas, encerrando a saída de água.
    valvulas.atualizarValvulas();

    display.clear();
    display.setCursor(0,0);
    display.print("Desligado");
}