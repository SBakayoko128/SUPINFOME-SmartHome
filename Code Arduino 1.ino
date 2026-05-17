#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Clavier
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {A0, A1, A2, A3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define TEMP_PIN   A5
#define BUZZER_PIN A4

// États du système
bool chauffageOn = false;
bool vmcOn = false;

void bip(int freq, int duree) {
  tone(BUZZER_PIN, freq, duree);
  delay(duree);
  noTone(BUZZER_PIN);
}

void sonnette() {
  bip(1000, 200);
  delay(100);
  bip(800, 200);
  delay(100);
  bip(1000, 400);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.begin(16, 2);

  lcd.print("  SUPINFOME v1.0");
  lcd.setCursor(0, 1);
  lcd.print("  Smart Home    ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Température
  int val = analogRead(TEMP_PIN);
  float temp = map(val, 0, 1023, 0, 50);

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print("C        ");

  // Chauffage + VMC automatique toutes les 2 secondes
  static unsigned long dernierCheck = 0;
  if (millis() - dernierCheck >= 2000) {
    dernierCheck = millis();

    // Chauffage
    if (temp < 19) {
      chauffageOn = true;
      Serial.write('R');
      lcd.setCursor(0, 1);
      lcd.print("Chauffage ON!   ");
    } else {
      chauffageOn = false;
      Serial.write('W');
      lcd.setCursor(0, 1);
      lcd.print("Chauffage OFF   ");
    }

    // VMC — on lit l'état depuis Arduino 2 via Serial
    // On garde juste une variable locale basée sur un seuil fictif
    // (la vraie lecture humidité est sur Arduino 2)
  }

  // Clavier
  char key = keypad.getKey();
  if (key != NO_KEY) {
    lcd.setCursor(0, 1);

    if (key == '1') {
      // Affiche état chauffage
      if (chauffageOn) {
        lcd.print("Chauffage: ON   ");
        lcd.setCursor(0, 0);
        lcd.print("Temp:");
        lcd.print(temp);
        lcd.print("C <19  ");
      } else {
        lcd.print("Chauffage: OFF  ");
        lcd.setCursor(0, 0);
        lcd.print("Temp:");
        lcd.print(temp);
        lcd.print("C OK   ");
      }
    } else if (key == '2') {
      lcd.print("Menu:Eclairage  ");
      Serial.write('W');
    } else if (key == '3') {
      lcd.print("Volet: toggle   ");
      Serial.write('V');
      bip(500, 100);
    } else if (key == '4') {
      // Affiche état VMC
      // On envoie une demande à Arduino 2
      Serial.write('H');
      lcd.print("VMC: auto >60%  ");
    } else if (key == '5') {
      lcd.print("Porte: Ouvert   ");
      Serial.write('O');
      bip(1000, 100);
    } else if (key == 'A') {
      lcd.print("Sonnette!       ");
      sonnette();
    } else if (key == 'B') {
      lcd.print("Porte: Ferme    ");
      Serial.write('F');
      bip(500, 100);
    } else if (key == 'C') {
      lcd.print("LED: Rouge      ");
      Serial.write('R');
    } else if (key == 'D') {
      lcd.print("LED: Verte      ");
      Serial.write('G');
    } else if (key == '#') {
      lcd.print("LED: Bleue      ");
      Serial.write('B');
    } else if (key == '0') {
      lcd.print("LED: Blanche    ");
      Serial.write('W');
    } else if (key == '*') {
      lcd.clear();
      lcd.print("  SUPINFOME v1.0");
    }
  }

  delay(200);
}