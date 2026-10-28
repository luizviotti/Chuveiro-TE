#include "flow_sensor.h"
#include "config.h"

LeitorVazao* LeitorVazao::instancia = nullptr;

LeitorVazao::LeitorVazao(uint8_t pin): pinoYF_B6(pin) {}

void LeitorVazao::begin(){
    instancia = this;
    pinMode(pinoYF_B6, INPUT_PULLUP);
    attachInterrupt(pinoYF_B6, LeitorVazao::contadorPulsosWrapper, FALLING);
}

void IRAM_ATTR LeitorVazao::contadorPulsos(){
    portENTER_CRITICAL_ISR(&Config::flowMux);
    contador_pulso++;
    portEXIT_CRITICAL_ISR(&Config::flowMux);
}

void IRAM_ATTR LeitorVazao::contadorPulsosWrapper(){
    if(instancia){instancia -> contadorPulsos(); }
}

void LeitorVazao::atualizarLeitura(){
    uint32_t agora = millis();
    uint32_t decorrido = agora - tempo_ultimo_calculo;
    if(decorrido >= 1000){
        portENTER_CRITICAL(&Config::flowMux);
        uint32_t pulsos = contador_pulso;
        contador_pulso = 0;
        portEXIT_CRITICAL(&Config::flowMux);

        // Fator de calibração do YF-B6
        float frequencia = pulsos * 1000.0f / decorrido; //pulsos por segundo (o loop pode atrasar e passar de 1s)
        vazao_Lmin = frequencia / 6.6; //no datasheet do YF-B6 ¾: f(Hz) = 6.6 * Q(L/min)

        tempo_ultimo_calculo = agora;
    }
}

bool LeitorVazao::vazaoSegura(){
    if(vazao_Lmin >= Config::VAZAO_MINIMA_NECESSARIA){
        return true;
    }
    return false;
}