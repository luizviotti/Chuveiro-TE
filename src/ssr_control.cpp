#include "ssr_control.h"
#include "config.h"

ControleSsr* ControleSsr::instancia = nullptr;

//periodo de uma onda, dada a frequência de 60hz
static constexpr uint32_t PERIODO_CICLO_US = 1000000UL / 60UL;

ControleSsr::ControleSsr(uint8_t pinSsr, int ciclos_padrao, int ciclos_analisados):
    _pinoSsr(pinSsr),
    _timerDisparo(nullptr),
    _ciclosLigados(ciclos_padrao),
    _ciclosAnalisados(max(ciclos_analisados, 1)),
    ciclos_compartilhados(ciclos_padrao)
    {}


void ControleSsr::begin(){
    instancia = this;
    pinMode(_pinoSsr, OUTPUT);
    digitalWrite(_pinoSsr, LOW);

    _timerDisparo = timerBegin(0, 80, true);
    timerAttachInterrupt(_timerDisparo, ControleSsr::timerDisparoWrapper, true);

}

void ControleSsr::setNumeroCiclos(float output_pid){
    //a conversão (com float) é feita aqui no loop. A ISR só copia um inteiro, evitando uso da FPU dentro da interrupção.
    int ciclos = converterPIDparaCiclos(output_pid, _ciclosAnalisados);
    portENTER_CRITICAL(&Config::ssrMux);
    ciclos_compartilhados = ciclos;
    portEXIT_CRITICAL(&Config::ssrMux);
}

void IRAM_ATTR ControleSsr::handleTimerDisparo(){
    //o valor de ciclos ligados é atualizado ao fim da fase off, ou seja, no início de um novo intervalo.
    //assim cada intervalo tem exatamente _ciclosAnalisados ciclos (on + off).
    if(!faseOn){
        portENTER_CRITICAL_ISR(&Config::ssrMux);
        _ciclosLigados = ciclos_compartilhados;
        portEXIT_CRITICAL_ISR(&Config::ssrMux); //previne erros de leitura, impossibilita acesso e escrita simultânea 
    }
    faseOn = !faseOn;
    iniciarFase();
}

void IRAM_ATTR ControleSsr::timerDisparoWrapper(){
    if(instancia){ instancia->handleTimerDisparo(); }
}

void IRAM_ATTR ControleSsr::iniciarFase(){

    //calcula o quantos ciclos serão gastos nessa fase com base no faseOn. faseOn -> ssr ligado durante x ciclos | !faseOn -> ssr desligado durante os ciclos restantes
    uint32_t ciclosNestaFase = faseOn ? _ciclosLigados : (_ciclosAnalisados - _ciclosLigados);

    if(ciclosNestaFase == 0){ //caso os ciclos sejam 0 nesta fase aqui pulamos direto para a próxima
        faseOn = !faseOn;
        ciclosNestaFase = faseOn ? _ciclosLigados : (_ciclosAnalisados - _ciclosLigados);

        if(ciclosNestaFase == 0){ ciclosNestaFase = _ciclosAnalisados;} //em caso de pane o sistema só volta a analisar no próximo intervalo
    }

    digitalWrite(_pinoSsr, faseOn ? HIGH : LOW); //atualiza o pino digital do ssr

    uint64_t duracaoMicroSegundos = (uint64_t)ciclosNestaFase * PERIODO_CICLO_US;
    timerAlarmWrite(_timerDisparo, duracaoMicroSegundos, false);
    timerAlarmEnable(_timerDisparo);
}

void ControleSsr::enable(){
    faseOn = false;
    timerWrite(_timerDisparo, 0);
    iniciarFase();
}

void ControleSsr::disable(){
    timerAlarmDisable(_timerDisparo);
    digitalWrite(_pinoSsr, LOW);
}

int ControleSsr::converterPIDparaCiclos(float PID_output, int ciclos_analisados){
    PID_output = constrain(PID_output, 0.0f, 100.0f);
    return (int)lroundf(PID_output * ciclos_analisados / 100.0f);
}