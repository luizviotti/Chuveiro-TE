#ifndef SHOWER_CONTROL_H
#define SHOWER_CONTROL_H

#include "pid_control.h"
#include "ssr_control.h"

class Controller{

    private:

        ModoOperacao modo;
        ControlePID controle;
        ControleSsr ssr;

};





#endif