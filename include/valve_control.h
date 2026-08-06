#ifndef VALVE_CONTROL_H
#define VALVE_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>

/**
 * @brief Classe que agrupa operações para controle das válvulas do chuveiro
 */
class ControleValvula{

    private:

        Servo valvulas_QF;
        Servo valvula_vazao;

        double& abertura_quente;
        double& vazao;

        uint8_t _pinservo_QF;
        uint8_t _pinservo_vazao;

    public:

        ControleValvula(double& abertura_quente, double& vazao, uint8_t pinservo_QF, uint8_t pinservo_vazao);
        
        /**
         * @brief atualiza a posição das valvulas a cada loop
         */
        void atualizarValvulas();

        /**
         * @brief Converte output PID para angulo correspondente no registro motorizado. Apenas esqueleto do método, necessário testar empiricamente primeiro para definir conversões.
         */
        static int converterPIDparaAngulo(double PID_output);
};


#endif