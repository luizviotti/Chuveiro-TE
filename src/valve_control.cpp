#include "valve_control.h"

ControleValvula::ControleValvula(double& abertura_quente, double& vazao, uint8_t pinservo_QF, uint8_t pinservo_vazao):
abertura_quente(abertura_quente), vazao(vazao), _pinservo_QF(pinservo_QF), _pinservo_vazao(pinservo_vazao)
{   
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    valvulas_QF.attach(pinservo_QF); //os valores empiricos máximos de abertura e fechamento ainda precisam ser testados.
    valvula_vazao.attach(pinservo_vazao);
}

void ControleValvula::atualizarValvulas(){
    valvulas_QF.write(abertura_quente);
    valvula_vazao.write(vazao);
}