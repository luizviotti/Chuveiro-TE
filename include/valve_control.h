#ifndef VALVE_CONTROL_H
#define VALVE_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>

/**
 * @brief Classe que agrupa operações para controle das válvulas do chuveiro
 */
class ControleValvula{

    private:

        Servo valvulas_QF;      ///< Servo motor que controla a abertura das válvulas quente e fria
        Servo valvula_vazao;    ///< Servo motor que controla a abertura da válvula de vazão

        double& abertura_quente;
        double& vazao;

        uint8_t _pinservo_QF;
        uint8_t _pinservo_vazao;

    public:
        /**
         * @brief Construtor da classe de controle de válvula
         */
        ControleValvula(double& abertura_quente, double& vazao, uint8_t pinservo_QF, uint8_t pinservo_vazao);

        /**
         * @brief Setup dos servo motores e alocação de PWM no esp
         */
        void begin();
        
        /**
         * @brief atualiza a posição das valvulas a cada loop
         */
        void atualizarValvulas();

        /**
         * @brief Abre por completo a válvula quente
         */
        void abrirValvulaQuente();

        /**
         * @brief Converte output PID para angulo correspondente no registro motorizado. Apenas esqueleto do método, necessário testar empiricamente primeiro para definir conversões.
         */
        static int converterPIDparaAngulo(double PID_output);
};


#endif