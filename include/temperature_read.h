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

        const DeviceAddress sensor_quente = {0x28, 0x96, 0xE7, 0x47, 0x00, 0x00, 0x00, 0xF0}; ///< Endereço do sensor quente
        const DeviceAddress sensor_frio = {0x28, 0x47, 0x93, 0x4A, 0x00, 0x00, 0x00, 0xEC};   ///< Endereço do sensor frio
        const DeviceAddress sensor_misto = {0x28, 0x83, 0x9E, 0x25, 0x00, 0x00, 0x00, 0x49};  ///< Endereço do sensor misto

        double& temperatura_fria;
        double& temperatura_quente;
        double& temperatura_mista;
        
        unsigned long ultimaSolicitacao = 0;        ///< Contador de quando foi a ultima solicitação de leitura
        const unsigned long TEMPO_CONVERSAO = 400;  ///< Constante de tempo de conversão (Para 11 bits de leitura)
        bool aguardandoConversao = false;           ///< Booleano de conversão
        bool leitura_valida = false;                ///< Última leitura completa e sem sensores desconectados

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

        /**
         * @brief true se já houve ao menos uma leitura e nenhum sensor está desconectado/inválido.
         * Enquanto false o sistema não deve acionar a resistência.
         */
        bool leituraValida() const;

};

#endif