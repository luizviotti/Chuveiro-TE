#include "shower_control.h"
#include "config.h"

Controller::Controller():
    modo(ModoOperacao::MIX_ONLY),

    pid(Config::KP_MIX, Config::KI_MIX, Config::KD_MIX,
        Config::KP_HEAT, Config::KI_HEAT, Config::KD_HEAT,
        input_temp, output_mix, output_heat, temp_mista
        ),

    ssr(Config::PIN_SSR),

    leitor_temp(Config::PIN_TEMP_SENSOR, temp_fria, temp_quente, temp_mista),

    valvulas(output_mix, input_vazao, Config::PIN_SERVO_QF, Config::PIN_SERVO_VAZAO),

    pin_pot_temp(Config::PIN_TEMP_POT),

    pin_pot_vazao(Config::PIN_FLUX_POT),

    pin_flux_sensor(Config::PIN_FLUX_SENSOR)

    {}

void Controller::begin(){
    pid.begin();
    ssr.begin();
    leitor_temp.begin();
    valvulas.begin();

    //pin_pot_temp -> leitura analógica
    //pin_pot_vazao -> leitura análogica
    //pin_flux_sensor -> ?
}

