#include "triac_control.h"

ControleTriac* ControleTriac::instancia = nullptr;

ControleTriac::ControleTriac(): ControleTriac(8333) {}

ControleTriac::ControleTriac(long atrasoPadrao): atrasoMicroSegundos(atrasoPadrao), timerDisparo(nullptr) {}

void ControleTriac::init(uint8_t pinZeroCross, uint8_t pinTriac){
    this->pinoTriac = pinTriac;

    pinMode(pinZeroCross, INPUT);
    pinMode(pinoTriac, OUTPUT);
    digitalWrite(pinoTriac, LOW);

    attachInterrupt(digitalPinToInterrupt(pinZeroCross), ControleTriac::zeroCrossingWrapper, RISING); // interrompe execucao do loop quando a onda intercepta zero e executa zeroCrossingWrapper

    timerDisparo = timerBegin(0, 80, true); //cada tick do timer demora 1 microsegundo, a frequencia do contador é de 80mhz
    timerAttachInterrupt(timerDisparo, ControleTriac::timerDisparoWrapper, true);
}

void ControleTriac::setAtrasoMicros(long atrasoMicros){
    this->atrasoMicroSegundos = atrasoMicros;
}

void IRAM_ATTR ControleTriac::handleZeroCrossing(){
    timerRestart(timerDisparo);
    timerAlarmWrite(timerDisparo, atrasoMicroSegundos, false);
    timerAlarmEnable(timerDisparo);
}

void IRAM_ATTR ControleTriac::handleTimerDisparo(){
    digitalWrite(pinoTriac, HIGH);
    delayMicroseconds(50);
    digitalWrite(pinoTriac, LOW);
    timerAlarmDisable(timerDisparo);
}

void IRAM_ATTR ControleTriac::zeroCrossingWrapper(){
    if(instancia){ instancia->handleZeroCrossing(); }
}

void IRAM_ATTR ControleTriac::timerDisparoWrapper(){
    if(instancia){ instancia->handleTimerDisparo(); }
}