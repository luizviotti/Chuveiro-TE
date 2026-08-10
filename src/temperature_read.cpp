#include "temperature_read.h"

LeitorTemperatura::LeitorTemperatura(uint8_t pino_sensores):
onewire(pino_sensores), sensores(&onewire) 
{}

void LeitorTemperatura::begin(){
    sensores.begin();
    sensores.setResolution(11);
    sensores.setWaitForConversion(false);
    //A leitura dos sensores de temperatura será feita de forma assíncrona para não prejudicar a execução do loop principal.
}

void LeitorTemperatura::solicitar(){
    sensores.requestTemperatures();
    ultimaSolicitacao = millis();
    aguardandoConversao = true;
}

void LeitorTemperatura::atualizarTemperatura(){
    if(!aguardandoConversao) return;

    if(millis() - ultimaSolicitacao >= TEMPO_CONVERSAO){
        temperatura_fria = sensores.getTempC(sensor_frio);
        temperatura_quente = sensores.getTempC(sensor_quente);
        temperatura_mista = sensores.getTemp(sensor_misto);
        aguardandoConversao = false;

        solicitar();
    }
}

double LeitorTemperatura::getTempFria() const   { return temperatura_fria; }
double LeitorTemperatura::getTempQuente() const { return temperatura_quente; }
double LeitorTemperatura::getTempMista() const    { return temperatura_mista; }