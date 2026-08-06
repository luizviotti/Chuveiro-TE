#ifndef TEMPERATURE_READ_H
#define TEMPERATURE_READ_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>

/**
 * @brief Agrupa operações de leitura de temperatura.
 */
class LeitorTemperatura{

    private:

        OneWire onewire;
        DallasTemperature sensores;

        const DeviceAddress sensor_quente = {}; //esses valores são empriricos e dependem do endereço de cada unidade de sensor
        const DeviceAddress sensor_frio = {};   //os endereços são imutáveis entre execuções
        const DeviceAddress sensor_misto = {};   //mapeando por index não se tem certeza da ordem mapeada e não é possível determinar qual sensor é cada

        double temperatura_fria;
        double temperatura_quente;
        double temperatura_mista;

    public:

        LeitorTemperatura(uint8_t pino_sensores);

        /**
         * @brief Realiza uma nova leitura das temperaturas
         */
        void atualizarTemperatura();

        double& getTempFria();
        double& getTempQuente();
        double& getTempMista();


};






#endif