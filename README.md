# Biosense

O **Biosense** é um sistema automatizado de abertura de lixeira utilizando um sensor ultrassônico e um servo motor.

## Funcionamento

O sensor ultrassônico HC-SR04 realiza medições para identificar a distância de um objeto. O Arduino realiza três medições e calcula a média para obter um resultado mais estável.

Quando um objeto é detectado a uma distância menor que **120 cm**, o sistema identifica a aproximação e aciona o servo motor, responsável pelo movimento da tampa da lixeira. Um LED também é utilizado para indicar a detecção.

## Componentes utilizados

* Arduino UNO
* Sensor ultrassônico HC-SR04
* Servo motor
* LED
* Jumpers
* Lixeira

## Pinos utilizados

| Componente      | Pino |
| --------------- | ---- |
| TRIG do HC-SR04 | 5    |
| ECHO do HC-SR04 | 6    |
| Servo motor     | 7    |
| LED             | 10   |

## Tecnologias

* Arduino
* C/C++
* Sensor ultrassônico HC-SR04
* Servo motor

````

**Para o GitHub**, eu organizaria o repositório mais ou menos assim:

```text
Biosense/
│
├── Biosense.ino
│
└── README.md
````

Assim, quando o professor abrir o repositório, ele já encontra **o código principal + README**, exatamente como foi solicitado.

Só uma observação importante: no seu código original, você colocou `digitalWrite(10,HIGH)`, mas **não tinha colocado `pinMode(10, OUTPUT)` no `setup()`**. Como o LED faz parte do projeto, vale corrigir isso.
