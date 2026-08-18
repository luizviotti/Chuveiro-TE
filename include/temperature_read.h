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

        const DeviceAddress sensor_quente = {}; ///< Endereço do sensor quente
        const DeviceAddress sensor_frio = {};   ///< Endereço do sensor frio
        const DeviceAddress sensor_misto = {};  ///< Endereço do sensor misto

        double& temperatura_fria;
        double& temperatura_quente;
        double& temperatura_mista;
        
        unsigned long ultimaSolicitacao = 0;        ///< Contador de quando foi a ultima solicitação de leitura
        const unsigned long TEMPO_CONVERSAO = 400;  ///< Constante de tempo de conversão (Para 11 bits de leitura)
        bool aguardandoConversao = false;           ///< Booleano de conversão

    public:
        /**
         * @brief Construtor de LeitorTemperatura em memória
         */
        LeitorTemperatura(uint8_t pino_sensores, double& fria, double& quente, double& mista);

        /**
         * @brief Setup e ajuste dos sensores
         */
        void begin();

        /**
         * @brief Solicita leitura dos sensores de temperatura (operação não instantânea)
         */
        void solicitar();

        /**
         * @brief Realiza uma nova leitura das temperaturas
         */
        void atualizarTemperatura();

};

#endif