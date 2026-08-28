#include <Arduino.h>

#include "shower_control.h"

Controller chuveiro;

void setup(){
    chuveiro.begin();
}

void loop(){
    chuveiro.run();
}