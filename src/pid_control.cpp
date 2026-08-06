#include "pid_control.h"


ControlePID::ControlePID(
    const double& kp_mix,
    const double& ki_mix,
    const double& kd_mix,
    const double& kp_heat,
    const double& ki_heat,
    const double& kd_heat,
    double& input,
    double& output,
    double& setpoint
):  pidControleDeValvulas(&input, &output, &setpoint, kp_mix, ki_mix, kd_mix, DIRECT),
    pidControleDeResistencia(&input, &output, &setpoint, kp_heat, ki_heat, kd_heat, DIRECT)
    {
        pidControleDeResistencia.SetOutputLimits(0, 100);
        pidControleDeResistencia.SetSampleTime(100); //sample time maior, o proprio controle burst fire vai demorar mais que isso. nao faz sentido computar todo loop.

        pidControleDeValvulas.SetOutputLimits(0, 100);
        pidControleDeValvulas.SetSampleTime(20); //sample time menor, pois o controle via misturas já é mais volátil.

    }

void ControlePID::setModeHeat(){
    pidControleDeValvulas.SetMode(MANUAL);
    pidControleDeResistencia.SetMode(AUTOMATIC);
}

void ControlePID::setModeMix(){
    pidControleDeResistencia.SetMode(AUTOMATIC);
    pidControleDeValvulas.SetMode(MANUAL);
}

bool ControlePID::computar(ModoOperacao modo){
    if(modo == MIX_ONLY){
        pidControleDeValvulas.Compute();
    }
    if(modo == HEAT_ASSIST){
        pidControleDeResistencia.Compute();
    }
}
