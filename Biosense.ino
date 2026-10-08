#include <Servo.h> // Biblioteca utilizada para controlar o servo motor

// ==========================
// A. CONFIGURAÇÃO DOS PINOS
// ==========================

const int trigPin = 5;   // Pino TRIG do sensor ultrassônico
const int echoPin = 6;   // Pino ECHO do sensor ultrassônico
const int servoPin = 7;  // Pino de controle do servo motor
const int ledPin = 10;   // Pino do LED

// ==========================
// C. VARIÁVEIS UTILIZADAS
// ==========================

Servo servo;

long duration;           // Tempo de retorno do sinal ultrassônico
long dist;               // Distância calculada em centímetros
long leituras[3];        // Armazena 3 medições para calcular a média

// ==========================
// B. INICIALIZAÇÃO
// ==========================

void setup() {
  Serial.begin(9600);

  // Configuração dos pinos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Inicializa o servo com a tampa fechada
  servo.attach(servoPin);
  servo.write(0);
  delay(100);
  servo.detach();
}

// ==========================
// D. LEITURA DO SENSOR
// ==========================

void medirDistancia() {

  // Envia um pulso pelo sensor ultrassônico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);

  digitalWrite(trigPin, LOW);

  // Recebe o tempo de retorno do sinal
  duration = pulseIn(echoPin, HIGH);

  // Converte o tempo em distância (cm)
  dist = (duration / 2) / 29.1;
}

// ==========================
// PROGRAMA PRINCIPAL
// ==========================

void loop() {

  // Realiza 3 medições para obter uma distância média
  for (int i = 0; i < 3; i++) {

    medirDistancia();
    leituras[i] = dist;

    delay(10);
  }

  // Calcula a média das três medições
  dist = (leituras[0] + leituras[1] + leituras[2]) / 3;

  // Exibe a distância no Monitor Serial
  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.println(" cm");

  // ==========================
  // E. ESTRUTURA CONDICIONAL
  // ==========================

  // Verifica se algum objeto está a menos de 50 cm
  if (dist < 50) {

    Serial.println("OBJETO DETECTADO!");

    // Acende o LED para indicar a detecção
    digitalWrite(ledPin, HIGH);

    // ==========================
    // F. ACIONAMENTO DO ATUADOR
    // ==========================

    // Ativa o servo motor
    servo.attach(servoPin);

    // Mantém a tampa fechada inicialmente
    servo.write(0);
    delay(3000);

    // Abre a tampa
    servo.write(150);
    delay(1000);

    // Desliga o servo após o movimento
    servo.detach();

    // Apaga o LED
    digitalWrite(ledPin, LOW);
  }
}