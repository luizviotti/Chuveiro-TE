#ifndef SSR_CONTROL_H
#define SSR_CONTROL_H

#include <Arduino.h>

/**
 * @brief Classe que reune operações de controle burst fire utilizando um solid state relay
 */
class ControleSsr{
    
    public:
        /**
         * @brief Construtor da classe
         * @param ciclos_padrao Número padrão de ciclos pelos quais o ssr vai ficar ligado (valor padrão 0)
         * @param ciclos_analisados Intervalo padrão sobre o qual o controle é realizado.
         * Como a rede elétrica opera em 60Hz, cada ciclo dura aproximadamente 16,67ms.
         * Para um valor de 20, o controle PID é aplicado via burst fire ao longo de
         * 20 ciclos de rede antes que o valor de ciclos ativos seja recalculado.
         */
        ControleSsr(int ciclos_padrao = 0, int ciclos_analisados = 20);

        /**
         * @brief Método para iniciar o controle no setup
         */
        void init(uint8_t pino_ssr);

        /**
         * @brief Altera a quantidade de ciclos ligados e desligados na próxima etapa de controle.
         */
        void setNumeroCiclos(int ciclos);

    private:
        
        uint8_t _pinoSsr;
        hw_timer_t* _timerDisparo;
        volatile int _ciclosLigados;
        int _ciclosAnalisados;
        volatile bool faseOn;

        /**
         * @brief Lógica de disparo de timer para controle de potência.
         */
        void IRAM_ATTR handleTimerDisparo();

        /**
         * @brief gerencia a lógica de burst fire. Monitora o periodo analisado e controla a saida do pinoSsr
         */
        void IRAM_ATTR iniciarFase();

        /**
         * @brief Wrapper para handleTimerDisparo(), timerAttachInterrupt() não aceita métodos de instância.
         */
        static void IRAM_ATTR timerDisparoWrapper();

        static ControleSsr* instancia; // Gambiarra para utilizar o handleTimerDisparo()
};

#endif