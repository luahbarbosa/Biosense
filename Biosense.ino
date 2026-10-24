#include <Servo.h>

const int trigPin = 5;
const int echoPin = 6;
const int servoPin = 7;
const int ledPin = 10;

Servo servo;
long leituras[3];

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  servo.attach(servoPin);
  servo.write(0);
  delay(500);
  servo.detach();
}

long medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Timeout de 30 ms: se não houver eco, retorna 0
  long duration = pulseIn(echoPin, HIGH, 30000);

  // Sem eco = nada na frente -> distância "infinita"
  if (duration == 0) return 999;

  return (duration / 2) / 29.1;
}

void loop() {
  for (int i = 0; i < 3; i++) {
    leituras[i] = medirDistancia();
    delay(30);   // sensor precisa de um intervalo entre medições
  }

  // Só detecta se TODAS as leituras forem válidas e menores que 50 cm
  bool detectado = true;
  for (int i = 0; i < 3; i++) {
    if (leituras[i] < 2 || leituras[i] >= 50) detectado = false;
  }

  long media = (leituras[0] + leituras[1] + leituras[2]) / 3;
  Serial.print("Distancia: ");
  Serial.print(media);
  Serial.println(" cm");

  if (detectado) {
    Serial.println("OBJETO DETECTADO!");
    digitalWrite(ledPin, HIGH);

    servo.attach(servoPin);
    servo.write(150);      // abre a tampa
    delay(3000);           // mantém aberta por 3 s

    servo.write(0);        // fecha a tampa
    delay(1000);
    servo.detach();

    digitalWrite(ledPin, LOW);
    delay(500);            // pausa antes de uma nova detecção
  }
}