#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <Arduino.h>

/**
 * Pinagem e constantes empíricas para o funcionamento do sistema
 */
namespace Config{

    //Pinagem do esp
    constexpr uint8_t PIN_FLUX_POT =    34;
    constexpr uint8_t PIN_TEMP_POT =    35;
    constexpr uint8_t PIN_FLUX_SENSOR = 32;
    constexpr uint8_t PIN_TEMP_SENSOR = 33;
    constexpr uint8_t PIN_SERVO_VAZAO = 19;
    constexpr uint8_t PIN_SERVO_QF =    18;
    constexpr uint8_t PIN_SSR =         17;
    constexpr uint8_t LCD_ADDR =        0x27;
    constexpr uint8_t PIN_ON_OFF =      25;

    //Configurações do display
    constexpr uint8_t COLUNAS_DISPLAY = 16;
    constexpr uint8_t LINHAS_DISPLAY = 2;

    //Configurações do PID de mistura
    constexpr double KP_MIX = 1;
    constexpr double KI_MIX = 1;
    constexpr double KD_MIX = 1;

    //Configurações do PID de aquecimento
    constexpr double KP_HEAT = 1;
    constexpr double KI_HEAT = 1;
    constexpr double KD_HEAT = 1;

    //Configurações de controle do SSR
    constexpr int CICLOS_ANALISADOS_PADRAO = 20;
    constexpr float MODULO_TOLERANCIA = 1.0; //1 grau de tolerância para ativar / desativar o ssr

    //Configurações de interface
    constexpr unsigned long DEBOUNCE_MS = 250;          //tempo mínimo entre dois toques válidos no botão on/off
    constexpr unsigned long INTERVALO_DISPLAY_MS = 250; //intervalo mínimo entre atualizações do display (I2C é lento)

    //Configurações das válvulas motorizadas
    constexpr int ABERTURA_MAXIMA_VALVULA = 180; //valor ainda precisa ser determinado quando a válvula for desenvolvida

    //Variaveis auxiliares para leitura e escrita de variáveis nas ISR
    extern portMUX_TYPE ssrMux; //mecânismo que garante que não hajam leituras e escritas simultâneas nas ISR
    extern portMUX_TYPE flowMux; 

    //Configurações de segurança de vazão
    constexpr float VAZAO_MINIMA_NECESSARIA = 15; //ainda deve ser decidido
}











#endif