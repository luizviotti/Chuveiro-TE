#ifndef TRIAC_CONTROL_H
#define TRIAC_CONTROL_H

#include <Arduino.h>

/**
 * @brief Classe que reune operacoes de controle de potência via componente TRIAC
 */
class ControleTriac {
    public:
        /**
         * Metodo construtor
         */
        ControleTriac();

        ControleTriac(long atrasoPadrao);

        /**
         * Metodo para iniciar o controle em setup
         */
        void init(uint8_t pinoZeroCross, uint8_t pinoTriac);

        /**
         * Metodo chamado a partir do output do PID
         */
        void setAtrasoMicros(long atrasoMicros);

    private:

        uint8_t pinoTriac;
        hw_timer_t* timerDisparo;
        volatile long atrasoMicroSegundos;

        /**
         * Logica de detecção de zero crossing para ondas de tensão alternadas.
         */
        void IRAM_ATTR handleZeroCrossing();

        /**
         * Logica de disparo do timer com atraso para controle de potência
         */
        void IRAM_ATTR handleTimerDisparo();

        /**
         * Wrapper para this->handleZeroCrossing(), função attachInterrupt e timerAttachInterrupt não suportam métodos de instância
         */
        static void IRAM_ATTR zeroCrossingWrapper();

        /**
         * Wrapper para this->thandleTimerDisparo(), função attachInterrupt e timerAttachInterrupt não suportam métodos de instância
         */        
        static void IRAM_ATTR timerDisparoWrapper();

        /**
         * Gambiarra para poder usar os métodos estáticos. Para o escopo do projeto isso não será problema pois
         * apenas um TRIAC será controlado por vez.
         */
        static ControleTriac *instancia;
};

#endif