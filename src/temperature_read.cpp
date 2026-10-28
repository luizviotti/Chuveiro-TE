#include "temperature_read.h"

LeitorTemperatura::LeitorTemperatura(uint8_t pino_sensores, double& fria, double& quente, double& mista):
onewire(pino_sensores), sensores(&onewire) , temperatura_fria(fria), temperatura_quente(quente), temperatura_mista(mista) 
{}

void LeitorTemperatura::begin(){
    sensores.begin();
    sensores.setResolution(11);
    sensores.setWaitForConversion(false);
    //A leitura dos sensores de temperatura será feita de forma assíncrona para não prejudicar a execução do loop principal.
    solicitar(); //dispara a primeira conversão, as seguintes são encadeadas em atualizarTemperatura()
}

void LeitorTemperatura::solicitar(){
    sensores.requestTemperatures();
    ultimaSolicitacao = millis();
    aguardandoConversao = true;
}

void LeitorTemperatura::atualizarTemperatura(){
    if(!aguardandoConversao) return;

    if(millis() - ultimaSolicitacao >= TEMPO_CONVERSAO){
        float fria = sensores.getTempC(sensor_frio);
        float quente = sensores.getTempC(sensor_quente);
        float mista = sensores.getTempC(sensor_misto);
        aguardandoConversao = false;

        leitura_valida = fria != DEVICE_DISCONNECTED_C
                      && quente != DEVICE_DISCONNECTED_C
                      && mista != DEVICE_DISCONNECTED_C;

        //só atualiza as variáveis de controle com leituras válidas, para não alimentar o PID com -127 °C
        if(leitura_valida){
            temperatura_fria = fria;
            temperatura_quente = quente;
            temperatura_mista = mista;
        }

        solicitar();
    }
}

bool LeitorTemperatura::leituraValida() const{
    return leitura_valida;
}
