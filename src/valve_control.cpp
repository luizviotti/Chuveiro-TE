#include "valve_control.h"
#include "config.h"

ControleValvula::ControleValvula(double& abertura_quente, double& vazao, uint8_t pinservo_QF, uint8_t pinservo_vazao):
abertura_quente(abertura_quente), vazao(vazao), _pinservo_QF(pinservo_QF), _pinservo_vazao(pinservo_vazao)
{}

void ControleValvula::begin(){
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    valvulas_QF.attach(_pinservo_QF); //os valores empiricos máximos de abertura e fechamento ainda precisam ser testados.
    valvula_vazao.attach(_pinservo_vazao);
}

void ControleValvula::atualizarValvulas(){
    valvulas_QF.write(converterPIDparaAngulo(abertura_quente));
    valvula_vazao.write(converterPIDparaAngulo(vazao)); //vazao está em %, precisa ser convertido para ângulo
}

void ControleValvula::abrirValvulaQuente(){
    abertura_quente = 100;
    valvulas_QF.write(Config::ABERTURA_MAXIMA_VALVULA);
}

int ControleValvula::converterPIDparaAngulo(double PID_output){
    PID_output = constrain(PID_output, 0.0, 100.0);
    return round(PID_output * Config::ABERTURA_MAXIMA_VALVULA/100);
}