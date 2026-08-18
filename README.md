# Chuveiro Termoeletrônico

Chuveiro eletrônico que controla automaticamente válvulas e resistor para atingir a temperatura desejada de forma ótima.

## Sumário

- [Módulos](#módulos)
- [Arquitetura](#arquitetura)
- [Requisitos](#requisitos)
- [Configuração](#configuração)

---

## Módulos

Lista rápida das classes do projeto, cada um com link para sua seção detalhada abaixo.
| [Controle de potência](#controlessr) | 
| [Leitura de vazão](#leitorvazao) | 
| [PID](#controlpid) |
| [Leitura de temperatura](#leitortemperatura) |
| [Ajuste de válvulas](#controlevalvula) |
| [Configurações](#configurações)|
| [Controle chuveiro](#controller)|

---

### ControleSSR

**Responsabilidade:** Agrupa operações de controle de potência via burst firing utilizando um SSR.

**Arquivos/Classes principais:**
- `include/ssr_control.h`
- `src/ssr_control.cpp`

**Detalhes:**
O controle do ssr ocorre via interrupção de timer de hardware (De forma paralela ao loop). Define-se um intervalo de operação para o controle burst firing, assim como o percentual de potência que se deseja transmitir. O timer de hardware opera alternando as fases, ou seja, ligando/desligando o pino do ssr durante esse intervalo de operação. O percentual de potência que se deseja transmitir é atualizado dinâmicamente entre cada intervalo de operação.

[⬆ Voltar ao sumário](#sumário)

---

### LeitorVazao

**Responsabilidade:** Agrupa operações de leitura de vazão, utilizando o YF-B6 3/4

**Arquivos/Classes principais:**
- `include/flow_sensor.h`
- `src/flow_sensor.h`

**Detalhes:**
Pelo funcionamento do YF-B6, a leitura de vazão ocorre de forma paralela no hardware, utilizando detecção de pulsos via attachInterrupt. O sensor envia varios pulsos e a partir da contagem destes é possível determinar a vazão atual em L/min.
Possui um método atualizar leitura que recalcula a vazão atual e que deve ser chamado no pipeline do loop principal.
Sua utilidade principal é verificar se o sistema possui uma vazão mínima necessária que permite que a resistência seja ligada sem danificar a estrutura.

[⬆ Voltar ao sumário](#sumário)

---

### ControlePID

**Responsabilidade:** Regula o funcionamento do sistema com base no feedback nas leituras de temperatura e setpoint.

**Arquivos/Classes principais:**
- `include/pid_control.h`
- `src/pid_control.cpp`

**Detalhes:**
Armazena duas instâncias de PID da lib "br3ttb/PID@^1.2.1", uma para o modo de mistura e para o modo de aquecimento. Essa necessidade surge do fato de que, por serem mecânismos que regulam a temperatura da água de forma diferente, os valores de KP, KI, KD podem ser diferentes. De toda forma, a classe recebe instruções do programa principal para computar o pid correspondente ao modo ativo e atualiza a variável responsável.

[⬆ Voltar ao sumário](#sumário)

---

### LeitorTemperatura

**Responsabilidade:** Agrupa operações de leitura de temperatura via protocolo Onewire, utilizando sensores DS18B20

**Arquivos/Classes principais:**
- `include/temperature_read.h`
- `src/temperature_read.cpp`

**Detalhes:**
Utiliza as libs "paulstoffregen/OneWire@^2.3.8" e "milesburton/DallasTemperature@^4.0.6". Utilizando o protocolo onewire é possível conectar varios sensores ao mesmo pino de dados. Entretanto é necessário hardcodar o endereço de cada um para identificar qual é o sensor respectivo (quente, frio, misto). A leitura dos sensores não é instantânea, por esse motivo os sensores fazem leituras de 11 bits, o que reduz o tempo de processamento para cerca de 400ms, o que é adequado para o programa, visto que o controle burst-fire ocorre por padrão em intervalos de 333ms e a água é resistente a variações muito rápidas de temperatura. O método atualizarTemperatura() deve ser chamado no loop principal com objetivo de atualizar as leituras.

[⬆ Voltar ao sumário](#sumário)

---

### ControleValvula

**Responsabilidade:** Controla as válvulas do chuveiro, tanto a válvula de mistura quanto a de vazão.

**Arquivos/Classes principais:**
- `include/valve_control.h`
- `src/valve_control.cpp`

**Detalhes:**
Utiliza a lib "madhephaestus/ESP32Servo@^3.2.1". Recebe do programa principal instruções para controle de válvulas. As instruções são passadas como valor percentual, que diz a % de abertura da válvula. A abertura da válvula quente depende do modo de operação e do PID de mistura, enquanto a válvula de vazão depende apenas do setpoint escolhido.

[⬆ Voltar ao sumário](#sumário)

---

### Configurações

**Responsabilidade:** Agrupa constantes e variáveis auxiliares do programa.

**Arquivos/Classes principais:**
- `include/config.h`

**Detalhes:**
Criei com o intuito de centralizar todas as variáveis auxiliares passíveis de teste empírico para facilitar a prototipagem.

[⬆ Voltar ao sumário](#sumário)

---

### Controller

**Responsabilidade:** Orquestra o funcionamento do chuveiro

**Arquivos/Classes principais:**
- `include/shower_control.h`
- `src/shower_control.h`

**Detalhes:**
Orquestra todo o funcionamento do chuveiro. Armazena uma instância de cada classe acima, além de pinos analógicos para input de temperatura e vazão. Coordena o setup do sistema, define o modo de operação do chuveiro com base nas leituras, computa o pid, atualiza a posição das válvulas e atualiza a fase do controle burt firing. Centraliza todas as operações principais.

**Pipeline de execução:**
Input analógico -> Leitura dos sensores (vazão e temperatura) -> Define o modo de operação -> computa PID -> atualiza valvulas ->
atualiza display


[⬆ Voltar ao sumário](#sumário)



## Arquitetura

```
.
├── include/
│   ├── config.h
│   ├── flow_sensor.h
│   ├── pid_control.h
│   ├── README
│   ├── shower_control.h
│   ├── ssr_control.h
│   ├── temperature_read.h
│   └── valve_control.h
├── lib/
├── src/
│   ├── flow_sensor.cpp
│   ├── main.cpp
│   ├── pid_control.cpp
│   ├── shower_control.cpp
│   ├── ssr_control.cpp
│   ├── temperature_read.cpp
│   └── valve_control.cpp
├── test/
├── .gitignore
├── platformio.ini
└── README.md
```

---

## Bibliotecas

Lógica do Programa:
- [Controle PID](https://registry.platformio.org/libraries/br3ttb/PID)

Sensor de Temperatura DS18B20
- [OneWire](https://registry.platformio.org/libraries/paulstoffregen/OneWire)
- [DallasTemperature](https://registry.platformio.org/libraries/milesburton/DallasTemperature)

Controle de Servo Motores:
- [ESP32Servo](https://registry.platformio.org/libraries/madhephaestus/ESP32Servo)

Display:
- [LiquidCrystal_I2C](https://registry.platformio.org/libraries/enjoyneering/LiquidCrystal_I2C)

---

##  Configuração

Necessário alterar as constantes no arquivo config.h com base nos testes empíricos.

---
