/*
숫자야구 키트 제작
사용 부품
  - 아두이노 나노
  - 1602LCD
  - 3*4 키패드
*/

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // 에러 발생 시 0x27을 지우고 0x3F로 바꿔주세요.
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 3, 8, 7, 5 };
byte colPins[COLS] = { 4, 2, 6 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char result[4];
int ball = 0, strike = 0;
char keyInput[4];
int times = 0;
int state = 0;
char key;

void mainFunc() {
  for (int i = 0; i < 4; i++) {
    keyInput[i] = keypad.waitForKey();
    while (1) {
      if (keyInput[i] == '*' || keyInput[i] == '#') {
        for (int k = 0; k < 16; k++) {
          lcd.setCursor(k, 0);
          lcd.print(" ");
        }
        lcd.setCursor(0, 0);
        lcd.print("Press number");
        keyInput[i] = keypad.waitForKey();
      } else if (i == 0) {
        for (int k = 0; k < 16; k++) {
          lcd.setCursor(k, 0);
          lcd.print(" ");
        }
        lcd.setCursor(0, 0);
        lcd.print(F("S : "));
        lcd.setCursor(4, 0);
        lcd.print(strike);
        lcd.setCursor(9, 0);
        lcd.print(F("B : "));
        lcd.setCursor(13, 0);
        lcd.print(ball);
        break;
      } else break;
    }
    for (int j = 0; j < i; j++) {
      if (keyInput[i] == keyInput[j]) {
        i--;
        for (int k = 0; k < 16; k++) {
          lcd.setCursor(k, 0);
          lcd.print(" ");
        }
        lcd.setCursor(0, 0);
        lcd.print(F("Another number"));
        break;
      } else {
        for (int k = 0; k < 16; k++) {
          lcd.setCursor(k, 0);
          lcd.print(" ");
        }
        lcd.setCursor(0, 0);
        lcd.print(F("S : "));
        lcd.setCursor(4, 0);
        lcd.print(strike);
        lcd.setCursor(9, 0);
        lcd.print(F("B : "));
        lcd.setCursor(13, 0);
        lcd.print(ball);
      }
    }
    lcd.setCursor(i, 1);
    lcd.print(keyInput[i]);
  }
  delay(200);
  ball = 0;
  strike = 0;
  for (int i = 0; i < 4; i++) {
    if (keyInput[i] == result[i]) strike++;
    else {
      for (int j = 0; j < 4; j++) {
        if (keyInput[i] == result[j]) {
          ball++;
        }
      }
    }
  }
  if (ball == 0 && strike == 0) {
    lcd.clear();
    lcd.print(F("OUT"));
    delay(1000);
    times++;
    lcd.clear();
    lcd.print(F("S : "));
    lcd.setCursor(4, 0);
    lcd.print(strike);
    lcd.setCursor(9, 0);
    lcd.print(F("B : "));
    lcd.setCursor(13, 0);
    lcd.print(ball);
    return;
  } else if (strike == 4) {
    times++;
    lcd.clear();
    lcd.print(F("Correct answer!"));
    delay(2000);
    lcd.clear();
    lcd.print(F("attempts : "));
    lcd.setCursor(11, 0);
    lcd.print(times);
    lcd.setCursor(0, 1);
    lcd.print(F("Press 1 to start"));
    state = 0;
    times = 0;
    ball = 0;
    strike = 0;
    return;
  } else {
    lcd.clear();
    lcd.print(F("S : "));
    lcd.setCursor(4, 0);
    lcd.print(strike);
    lcd.setCursor(9, 0);
    lcd.print(F("B : "));
    lcd.setCursor(13, 0);
    lcd.print(ball);
    times++;
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Gongzipsa"));
  lcd.setCursor(0, 1);
  lcd.print(F("Bulls and Cows"));
  delay(2000);
  lcd.clear();
  lcd.print(F("Press 1 to Start"));

  Serial.begin(9600);
  Serial.println("start");
}

void loop() {
  if (state == 0) {
    randomSeed(millis());
    for (int i = 0; i < 4; i++) {
      result[i] = random(48, 58);
      for (int j = 0; j < i; j++) {
        if (result[i] == result[j]) i--;
      }
    }
    key = keypad.waitForKey();
  }

  if (key != NO_KEY) {
    state = 1;
    if (times == 0) {
      if (key == '1') {
        lcd.clear();
        lcd.print(F("Press 4 numbers"));
        mainFunc();
      } else {
        state = 0;
        return;
      }
    } else {
      mainFunc();
    }
  }
}