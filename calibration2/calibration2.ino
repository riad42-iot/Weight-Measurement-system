#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <EEPROM.h>

// ---------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- HX711 ----------
HX711 scale;
#define HX711_DT 4
#define HX711_SCK 5

float calibration_factor = -7000.0;
int eepromAddr = 0;

// ---------- Buttons ----------
#define BTN_UP      6
#define BTN_DOWN    7
#define BTN_ENTER   8
#define BTN_CALIB   9

// ---------- Buzzer ----------
#define BUZZER_PIN A2

// ---------- LEDs ----------
#define LED_A 11
#define LED_B 12
#define LED_C 13
#define LED_D A0
#define LED_E A1

// ---------- Groups ----------
const char* groupName[7] = {
  "YB25L-C", "12N5-3B", "YB9-B",
  "RTX4L-B", "YB7-B", "RTX5L-B", "12N6.5-3B"
};

const int groupCount = 7;
int selectedGroup = 0;

// ---------- Weight Tables (grams) ----------
float weightMin[7][5] = {
  { 96, 91, 86, 0, 0 },
  { 190, 185, 180, 175, 170 },
  { 336, 331, 326, 321, 315 },
  { 151, 146, 141, 136, 131 },
  { 281, 276, 271, 266, 261 },
  { 191, 186, 181, 176, 0 },
  { 251, 246, 241, 236, 231 }
};

float weightMax[7][5] = {
  { 100, 95, 90, 0, 0 },
  { 194.9, 189.9, 184.9, 179.9, 174.9 },
  { 340, 335, 330, 325, 320 },
  { 155, 150, 145, 140, 135 },
  { 285, 280, 275, 270, 265 },
  { 195, 190, 185, 180, 0 },
  { 255, 250, 245, 240, 235 }
};

// ---------- Debounce ----------
unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 180;

bool isPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    if (millis() - lastPressTime > debounceDelay) {
      lastPressTime = millis();
      return true;
    }
  }
  return false;
}

// ---------- LED Control ----------
void clearLEDs() {
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
  digitalWrite(LED_D, LOW);
  digitalWrite(LED_E, LOW);
}

void indicateGradeLED(int g) {
  clearLEDs();
  if (g == 0) digitalWrite(LED_A, HIGH);
  if (g == 1) digitalWrite(LED_B, HIGH);
  if (g == 2) digitalWrite(LED_C, HIGH);
  if (g == 3) digitalWrite(LED_D, HIGH);
  if (g == 4) digitalWrite(LED_E, HIGH);
}

// ---------- LCD ----------
void showSelectScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Group:");
  lcd.setCursor(0, 1);
  lcd.print("> ");
  lcd.print(groupName[selectedGroup]);
}

// ---------- Grading ----------
int getWeightLevel(int group, float w) {
  for (int i = 0; i < 5; i++) {
    if (weightMin[group][i] == 0 && weightMax[group][i] == 0) continue;
    if (w >= weightMin[group][i] && w <= weightMax[group][i]) return i;
  }
  return -1;
}

// ---------- Min / Max helpers ----------
float getGroupMinWeight(int group) {
  float m = 99999;
  for (int i = 0; i < 5; i++) {
    if (weightMin[group][i] > 0 && weightMin[group][i] < m)
      m = weightMin[group][i];
  }
  return m;
}

float getGroupMaxWeight(int group) {
  float m = 0;
  for (int i = 0; i < 5; i++) {
    if (weightMax[group][i] > m)
      m = weightMax[group][i];
  }
  return m;
}

// ---------- Calibration ----------
void saveCalibration(float factor) {
  EEPROM.put(eepromAddr, factor);
}

float loadCalibration() {
  float v;
  EEPROM.get(eepromAddr, v);
  if (isnan(v) || v < -100000 || v > 100000) return calibration_factor;
  return v;
}

void runCalibration() {

  lcd.clear();
  lcd.print("Calibration");
  delay(1200);

  lcd.clear();
  lcd.print("Remove Weight");
  lcd.setCursor(0, 1);
  lcd.print("Press CALIB");
  while (!isPressed(BTN_CALIB));

  scale.tare();
  delay(800);

  lcd.clear();
  lcd.print("Place 0.5kg");
  lcd.setCursor(0, 1);
  lcd.print("Press CALIB");
  while (!isPressed(BTN_CALIB));

  float knownWeightKg = 0.5;
  long raw = scale.read_average(15);

  calibration_factor = raw / knownWeightKg;
  scale.set_scale(calibration_factor);
  saveCalibration(calibration_factor);

  lcd.clear();
  lcd.print("Saved Cal:");
  lcd.setCursor(0, 1);
  lcd.print(calibration_factor, 1);
  delay(2000);

  showSelectScreen();
}

// ---------- Setup ----------
void setup() {

  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_ENTER, INPUT_PULLUP);
  pinMode(BTN_CALIB, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);

  clearLEDs();

  Wire.begin();
  lcd.init();
  lcd.backlight();

  scale.begin(HX711_DT, HX711_SCK);
  calibration_factor = loadCalibration();
  scale.set_scale(calibration_factor);
  scale.tare();

  lcd.setCursor(0, 0);
  lcd.print("Weight System");
  lcd.setCursor(0, 1);
  lcd.print("Cal:");
  lcd.print(calibration_factor, 0);

  delay(1500);
  showSelectScreen();
}

// ---------- Loop ----------
void loop() {

  if (isPressed(BTN_CALIB)) {
    runCalibration();
    return;
  }

  if (isPressed(BTN_UP)) {
    selectedGroup = (selectedGroup + 1) % groupCount;
    showSelectScreen();
    return;
  }

  if (isPressed(BTN_DOWN)) {
    selectedGroup = (selectedGroup - 1 + groupCount) % groupCount;
    showSelectScreen();
    return;
  }

  if (isPressed(BTN_ENTER)) {

    lcd.clear();
    lcd.print("Taring...");
    scale.tare();
    delay(700);

    while (true) {

      if (isPressed(BTN_UP) || isPressed(BTN_DOWN) ||
          isPressed(BTN_ENTER) || isPressed(BTN_CALIB)) {
        showSelectScreen();
        return;
      }

      float weightKg = scale.get_units(5);
      float weight_g = weightKg * 1000.0;
      if (weight_g < 0) weight_g = 0;

      int grade = getWeightLevel(selectedGroup, weight_g);
      float minW = getGroupMinWeight(selectedGroup);
      float maxW = getGroupMaxWeight(selectedGroup);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(groupName[selectedGroup]);
      lcd.setCursor(0, 1);
      lcd.print("W:");
      lcd.print(weight_g, 1);
      lcd.print("g ");

      if (grade >= 0) {
        lcd.print("G:");
        lcd.print(char('A' + grade));
        indicateGradeLED(grade);
        noTone(BUZZER_PIN);
      }
      else if (weight_g < minW) {
        lcd.print("LOW");
        clearLEDs();
        tone(BUZZER_PIN, 800, 200);   // 🔉 low sound
      }
      else if (weight_g > maxW) {
        lcd.print("HIGH");
        clearLEDs();
        tone(BUZZER_PIN, 2500, 200);  // 🔊 high sound
      }

      delay(200);
    }
  }
}
