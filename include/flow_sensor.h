#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

/**
 * @brief Classe que agrupa operações de leitura de vazão com YF-B6 ¾
 */
class LeitorVazao{
    private:

        uint8_t pinoYF_B6;                      ///< Pinagem do sensor de vazao
        volatile uint32_t contador_pulso = 0;   ///< Contador de pulsos enviados pelo YF-B6 ¾
        uint32_t tempo_ultimo_calculo = 0;      ///< Ultimo momento que foi realizado leitura;
        float vazao_Lmin = 0;                   ///< Vazao lida

        /**
         * @brief Incrementa o contador toda vez que é chamado.
         */
        void IRAM_ATTR contadorPulsos();

        /**
         * @brief Gambiarra para utilizar attachInterrupt que aceita apenas referencias para funções c-style
         */
        static void IRAM_ATTR contadorPulsosWrapper();

        static LeitorVazao* instancia; ///< Parte da gambiarra do wrapper

    public:
        /**
         * @brief Método construtor
         */
        LeitorVazao(uint8_t pin);

        /**
         * @brief Atualiza as leituras do sensor
         */
        void atualizarLeitura();
        
        /**
         * @brief Rotina de setup do sensor
         */
        void begin();

        /**
         * @brief retorna true caso a vazão atual seja segura para ativar a resistência
         */
        bool vazaoSegura();  
};

#endif