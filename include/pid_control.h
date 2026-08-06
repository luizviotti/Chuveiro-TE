#ifndef PID_CONTROL_H
#define PID_CONTROL_H

#include <PID_v1.h>
#include <Arduino.h>

enum ModoOperacao{
    MIX_ONLY,
    HEAT_ASSIST
};

/**
 * @brief Classe que decide qual tipo de controle PID será aplicado no momento;
 */
class ControlePID{
    
    private:
        PID pidControleDeValvulas;
        PID pidControleDeResistencia;

        ModoOperacao modo;

    public:
        
        ControlePID(
            const double& kp_mix,
            const double& ki_mix,
            const double& kd_mix,
            const double& kp_heat,
            const double& ki_heat,
            const double& kd_heat,
            double& input,
            double& output_mix,
            double& output_heat,
            double& setpoint
        );

        /**
         * @brief Altera o modo de controle para a função de esquentar.
         */
        void setModeHeat();

        /**
         * @brief Altera o modo de controle para a função de misturar apenas
         */
        void setModeMix();

        /**
         * @brief Atualiza as variaveis de output com base no input e no setpoint e no modo de operação atual
         */
        bool computar(ModoOperacao modo);
};

#endif