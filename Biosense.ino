#include <Servo.h>

const int trigPin  = 5;
const int echoPin  = 6;
const int servoPin = 7;
const int ledPin   = 10;

const int ANGULO_ABERTA  = 120;
const int ANGULO_FECHADA = 0;
const int DIST_MAXIMA    = 50;                 // cm
const unsigned long TEMPO_APOS_SAIR = 5000;    // 5 s depois que a pessoa sai

Servo servo;

enum Estado { FECHADA, ABERTA_PRESENTE, ABERTA_AGUARDANDO };
Estado estado = FECHADA;

unsigned long instanteSaida = 0;

void moverServo(int angulo) {
  servo.attach(servoPin);
  servo.write(angulo);
  delay(600);          // tempo para o servo chegar na posição
  servo.detach();      // evita tremor e economiza energia
}

long medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;               // sem eco = nada na frente
  return (duration / 2) / 29.1;
}

// Retorna true se as 3 leituras indicarem presença (filtra ruído)
bool pessoaPresente() {
  for (int i = 0; i < 3; i++) {
    long d = medirDistancia();
    if (d < 2 || d >= DIST_MAXIMA) return false;
    delay(30);
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  moverServo(ANGULO_FECHADA);
  Serial.println("Sistema iniciado - tampa FECHADA");
}

void loop() {
  bool presente = pessoaPresente();

  switch (estado) {

    case FECHADA:
      if (presente) {
        Serial.println("Pessoa detectada -> ABRINDO");
        digitalWrite(ledPin, HIGH);
        moverServo(ANGULO_ABERTA);
        estado = ABERTA_PRESENTE;
      }
      break;

    case ABERTA_PRESENTE:
      if (!presente) {
        Serial.println("Pessoa saiu -> aguardando 5 s");
        instanteSaida = millis();
        estado = ABERTA_AGUARDANDO;
      }
      break;

    case ABERTA_AGUARDANDO:
      if (presente) {
        Serial.println("Pessoa voltou -> mantem aberta");
        estado = ABERTA_PRESENTE;
      }
      else if (millis() - instanteSaida >= TEMPO_APOS_SAIR) {
        Serial.println("5 s sem ninguem -> FECHANDO");
        moverServo(ANGULO_FECHADA);
        digitalWrite(ledPin, LOW);
        estado = FECHADA;
      }
      break;
  }
}