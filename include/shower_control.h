#ifndef SHOWER_CONTROL_H
#define SHOWER_CONTROL_H

#include "pid_control.h"
#include "ssr_control.h"
#include "temperature_read.h"
#include "valve_control.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



/**
 * @brief Classe que centraliza e orquestra o funcionamento do chuveiro
 */
class Controller{

    private:
        double input_temp = 25;         ///< Input de temperatura em °C
        double input_vazao = 50;        ///< Input de vazão em %
        double fluxo = 0;               ///< Vazão lida pelo sensor de fluxo
        double output_mix = 0;          ///< Output do PID de mistura
        double output_heat = 0;         ///< Output do PID da resistência
        double temp_fria = 0;           ///< Temperatura da fonte fria de água em °C
        double temp_quente = 0;         ///< Temperatura da fonte quente de água em °C
        double temp_mista = 0;          ///< Temperatura da saída do chuveiro em °C
        volatile float output_heat_compartilhado = 0; ///< Variável compartilhada entre loop e isr

        ModoOperacao modo;              ///< Modo de operação atual (MIX_ONLY ou HEAT_ASSIST)
        ControlePID pid;                ///< PID que controla a abertura das válvulas/potência
        ControleSsr ssr;                ///< Controle de potência
        LeitorTemperatura leitor_temp;  ///< Sensores de temperatura
        ControleValvula valvulas;       ///< Válvulas do sistema
        LiquidCrystal_I2C display;       ///< Display do protótipo

        uint8_t pin_pot_temp;           ///< Pino analógico de input de temperatura
        uint8_t pin_pot_vazao;          ///< Pino analógico de input de vazão
        uint8_t pin_flux_sensor;        ///< Pino do sensor de fluxo

    public:

        /**
         * @brief Construtor da classe Controller.
         * Não recebe parâmetros diretos. Para alterar a maneira como os componentes internos
         * são inicializados altere Config.h
         */
        Controller();

        /**
         * @brief Setup do sistema do chuveiro.
         */
        void begin();

        /**
         * @brief Pipeline de execução do loop principal.
         * 
         * Faz a leitura dos sensores/entradas, define modo de operação, Calcula PID, atualiza o display.
         * O controle de SSR ocorre paralelamente de forma assíncrona via timer de hardware.
         * 
         * Lê entradas (potênciometros e sensores) -> Define Modo de operação -> Atualiza PID -> Ajusta Válvulas -> Atualiza Display -> Reinicia
         */
        void run();

        /**
         * @brief Atualiza as informações de temperatura e vazão no display
         */
        void atualizarDisplay();

        /**
         * @brief Define o modo de operação do chuveiro (MIX_ONLY ou HEAT_ASSIST) e realiza a transição entre modos
         */
        void definirModoOperacao();
};





#endif