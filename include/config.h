#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <Arduino.h>

/**
 * Pinagem e constantes empíricas para o funcionamento do sistema
 */
namespace Config{

    constexpr uint8_t PIN_FLUX_POT =    34;
    constexpr uint8_t PIN_TEMP_POT =    35;
    constexpr uint8_t PIN_FLUX_SENSOR = 32;
    constexpr uint8_t PIN_TEMP_SENSOR = 33;
    constexpr uint8_t PIN_SERVO_VAZAO = 19;
    constexpr uint8_t PIN_SERVO_QF =    18;
    constexpr uint8_t PIN_SSR =         17;

    constexpr double KP_MIX = 1;
    constexpr double KI_MIX = 1;
    constexpr double KD_MIX = 1;

    constexpr double KP_HEAT = 1;
    constexpr double KI_HEAT = 1;
    constexpr double KD_HEAT = 1;

    constexpr int CICLOS_ANALISADOS_PADRAO = 20;


}











#endif