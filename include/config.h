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
    constexpr uint8_t LCD_ADDR =        0x27;

    constexpr uint8_t COLUNAS_DISPLAY = 16;
    constexpr uint8_t LINHAS_DISPLAY = 2;

    constexpr double KP_MIX = 1;
    constexpr double KI_MIX = 1;
    constexpr double KD_MIX = 1;

    constexpr double KP_HEAT = 1;
    constexpr double KI_HEAT = 1;
    constexpr double KD_HEAT = 1;

    constexpr int CICLOS_ANALISADOS_PADRAO = 20;

    constexpr float MODULO_TOLERANCIA = 1.0; //1 grau de tolerância para ativar / desativar o ssr

    constexpr int ABERTURA_MAXIMA_VALVULA; //valor ainda precisa ser determinado quando a válvula for desenvolvida

    portMUX_TYPE ssrMux = portMUX_INITIALIZER_UNLOCKED; //mecânismo que garante que não hajam leituras e escritas simultâneas nas ISR
    portMUX_TYPE flowMux = portMUX_INITIALIZER_UNLOCKED; 
}











#endif