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

        double temperatura_fria = 0, temperatura_quente = 0, temperatura_mista = 0;
        
        unsigned long ultimaSolicitacao = 0;
        const unsigned long TEMPO_CONVERSAO = 400;
        bool aguardandoConversao = false;

    public:

        LeitorTemperatura(uint8_t pino_sensores);

        void begin();

        void solicitar();

        /**
         * @brief Realiza uma nova leitura das temperaturas
         */
        void atualizarTemperatura();

        double getTempFria() const;
        double getTempQuente() const;
        double getTempMista() const;


};






#endif