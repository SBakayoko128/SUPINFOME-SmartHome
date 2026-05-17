#include <Servo.h>

// VMC
#define MOTOR_PIN 3
#define HUMIDITY_PIN A0

// LED RGB
#define LED_R 9
#define LED_G 10
#define LED_B 11

// Servo volet
Servo voletServo;
#define SERVO_VOLET 6

// Servo porte
Servo porteServo;
#define SERVO_PORTE 5

bool porteOuverte = false;
bool voletOuvert = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  voletServo.attach(SERVO_VOLET);
  voletServo.write(0); // volet fermé

  porteServo.attach(SERVO_PORTE);
  porteServo.write(0); // porte verrouillée

  // LED blanche au démarrage
  analogWrite(LED_R, 255);
  analogWrite(LED_G, 255);
  analogWrite(LED_B, 255);
}

void loop() {
  // VMC humidité automatique
  int val = analogRead(HUMIDITY_PIN);
  int humidity = map(val, 0, 1023, 0, 100);

  if (humidity > 60) {
    digitalWrite(MOTOR_PIN, HIGH); // VMC ON
  } else {
    digitalWrite(MOTOR_PIN, LOW);  // VMC OFF
  }

  // Commandes reçues depuis Arduino 1
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'O') {
      porteServo.write(90);
      porteOuverte = true;
    } else if (cmd == 'F') {
      porteServo.write(0);
      porteOuverte = false;
    } else if (cmd == 'V') {
      if (voletOuvert) {
        voletServo.write(0);
        voletOuvert = false;
      } else {
        voletServo.write(90);
        voletOuvert = true;
      }
    } else if (cmd == 'R') {
      analogWrite(LED_R, 255);
      analogWrite(LED_G, 0);
      analogWrite(LED_B, 0);
    } else if (cmd == 'G') {
      analogWrite(LED_R, 0);
      analogWrite(LED_G, 255);
      analogWrite(LED_B, 0);
    } else if (cmd == 'B') {
      analogWrite(LED_R, 0);
      analogWrite(LED_G, 0);
      analogWrite(LED_B, 255);
    } else if (cmd == 'W') {
      analogWrite(LED_R, 255);
      analogWrite(LED_G, 255);
      analogWrite(LED_B, 255);
    } else if (cmd == 'H') {
      // Répond avec état VMC
      if (humidity > 60) {
        Serial.write('M'); // VMC marche
      } else {
        Serial.write('S'); // VMC stop
      }
    }
  }

  delay(100);
}