#include "temperature_read.h"

LeitorTemperatura::LeitorTemperatura(uint8_t pino_sensores, double& fria, double& quente, double& mista):
onewire(pino_sensores), sensores(&onewire) , temperatura_fria(fria), temperatura_quente(quente), temperatura_mista(mista) 
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
