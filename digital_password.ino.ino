// C++ code
//

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

void setLocked(bool locked);

const byte redLed = 12;
const byte blueLed = 13;
const byte servo = A1;
const byte rs = 9;
const byte en = 10;
const byte db4 = A2;
const byte db5 = A3;
const byte db6 = A4;
const byte db7 = A5;

LiquidCrystal lcd(rs, en, db4, db5, db6, db7);
Servo servoMotor;

// Teclado
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[rows] = { 8, 7, 6, 5 };
byte colPins[cols] = { 4, 3, 2, 1 };
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// Senha. * = Ponteiro para o array que aponta para o 8.
char *password = "861375";
byte totalKeys = strlen(password);
byte tries = 0;
byte hits = 0;
bool locked = true;

void setup() {
  Serial.begin(9600);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(servo, OUTPUT);
  lcd.begin(16, 2);
  servoMotor.attach(servo);
  setLocked(locked);
  lcd.setCursor(1, 0);
  lcd.print("Digite a senha");
  delay(2000);
  lcd.clear();
}

void loop() {
  char key = teclado.getKey();

  if (key) {

    if (key == '#' || key == '*') {

      lcd.clear();
      hits = 0;
      tries = 0;
      delay(200);
      locked = true;
      setLocked(locked);
    }

    else {
      tries++;
      lcd.print("*");

      // Se digíto estiver correto
      if (key == password[hits]) {
        hits++;
      }

      // Tentativas
      if (tries == totalKeys) {
        tries = 0;

        // Se senha correta
        if (hits == totalKeys) {
          hits = 0;
          locked = false;
          setLocked(locked);
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Senha Correta!");
          lcd.setCursor(1, 1);
          lcd.print("Porta Aberta!");
          delay(5000);
          lcd.clear();
          locked = true;
          setLocked(locked);
        }

        else {
          locked = true;
          setLocked(locked);
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Senha Errada!");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tente Novamente");
          delay(2000);
          lcd.clear();
        }

        delay(200);
      }
    }
  }
}

void setLocked(bool locked) {
  if (locked) {
    digitalWrite(redLed, HIGH);
    digitalWrite(blueLed, LOW);
    servoMotor.write(0);
  }

  else {
    digitalWrite(redLed, LOW);
    digitalWrite(blueLed, HIGH);
    servoMotor.write(90);
  }
}