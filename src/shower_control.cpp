#include "shower_control.h"
#include "config.h"

Controller::Controller():
    modo(ModoOperacao::MIX_ONLY),

    pid(Config::KP_MIX, Config::KI_MIX, Config::KD_MIX,
        Config::KP_HEAT, Config::KI_HEAT, Config::KD_HEAT,
        input_temp, output_mix, output_heat, temp_mista
        ),

    ssr(Config::PIN_SSR),

    leitor_temp(Config::PIN_TEMP_SENSOR, temp_fria, temp_quente, temp_mista),

    valvulas(output_mix, input_vazao, Config::PIN_SERVO_QF, Config::PIN_SERVO_VAZAO),

    display(Config::LCD_ADDR, Config::COLUNAS_DISPLAY, Config::LINHAS_DISPLAY),

    leitor_vazao(Config::PIN_FLUX_SENSOR),

    pin_pot_temp(Config::PIN_TEMP_POT),

    pin_pot_vazao(Config::PIN_FLUX_POT),

    pin_flux_sensor(Config::PIN_FLUX_SENSOR)

    {}

void Controller::begin(){

    display.init();
    display.backlight();

    pid.begin();
    ssr.begin();
    leitor_temp.begin();
    valvulas.begin();
    leitor_vazao.begin();

}

void Controller::run(){
    
    //lendo input de temperatura e vazão
    input_vazao = analogRead(pin_pot_vazao)/4095.0f * 100.0f; //valor percentual, multiplica-se a leitura analógica por 1000

    input_temp = 20.0f + analogRead(pin_pot_temp)/4095.0f * 25.0f; //20c -> leitura mínima | 25c -> Incremento máximo | 45c -> Temperatura máxima

    //atualizando leituras de vazão
    leitor_vazao.atualizarLeitura();

    //leitura dos sensores de temperatura
    leitor_temp.atualizarTemperatura();

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

    display.clear();
    display.setCursor(0,0);
    display.printf("Temp: %.1f C", input_temp);
    display.setCursor(0,1);
    display.printf("Vazão: %.1f %", input_vazao);

}

void Controller::definirModoOperacao(){
    float diff = input_temp - temp_quente;
    //input_temp - temp_quente <= 0 -> fonte quente dá conta
    if(diff <= -Config::MODULO_TOLERANCIA || !leitor_vazao.vazaoSegura()){
        modo = MIX_ONLY;
        pid.setModeMix();
        pid.computar(modo);
        ssr.disable();
    }
    //input_temp - temp_quente >= 0 -> fonte quente insuficiente
    else if(diff >= Config::MODULO_TOLERANCIA){
        modo = HEAT_ASSIST;
        valvulas.abrirValvulaQuente();
        output_heat = 0;
        pid.setModeHeat();
        pid.computar(modo);
        ssr.enable();
    }

}