#include <LedControl.h>

LedControl lc = LedControl(10, 8, 9, 1);

const int shapeButton = 2;
const int rollButton = 3;
int currentShape = 0;
unsigned long lastPress = 0;

void setup() {
  pinMode(shapeButton, INPUT_PULLUP);
  pinMode(rollButton, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  drawShape(currentShape);
  randomSeed(analogRead(0));
}

void loop() {
  if (digitalRead(shapeButton) == LOW && millis() - lastPress > 300) {
    currentShape = (currentShape + 1) % 4;
    drawShape(currentShape);
    lastPress = millis();
  }

  if (digitalRead(rollButton) == LOW && millis() - lastPress > 300) {
    int maxValue;
    switch (currentShape) {
      case 0: maxValue = 4; break;
      case 1: maxValue = 6; break;
      case 2: maxValue = 8; break;
      case 3: maxValue = 20; break;
    }

    int result = random(1, maxValue + 1);
    animateShape(currentShape);
    displayNumber(result);
    drawShape(currentShape); // redesenha a forma depois do número
    lastPress = millis();
  }
}

void drawShape(int shape) {
  lc.clearDisplay(0);
  switch (shape) {
    case 0: drawTriangle(); break;
    case 1: drawSquare(); break;
    case 2: drawDiamond(); break;
    case 3: drawHexagon(); break;
  }
}

void drawTriangle() {
  for (int col = 1; col <= 6; col++) lc.setLed(0, 5, col, true);
  lc.setLed(0, 4, 1, true); lc.setLed(0, 4, 6, true);
  lc.setLed(0, 3, 2, true); lc.setLed(0, 3, 5, true);
  lc.setLed(0, 2, 3, true); lc.setLed(0, 2, 4, true);
}

void drawSquare() {
  for (int i = 2; i <= 5; i++) {
    lc.setLed(0, 2, i, true);
    lc.setLed(0, 5, i, true);
    lc.setLed(0, i, 2, true);
    lc.setLed(0, i, 5, true);
  }
}

void drawDiamond() {
  lc.setLed(0, 6, 3, true); lc.setLed(0, 6, 4, true);
  lc.setLed(0, 5, 2, true); lc.setLed(0, 4, 1, true);
  lc.setLed(0, 3, 2, true);
  lc.setLed(0, 2, 3, true); lc.setLed(0, 2, 4, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 4, 6, true);
  lc.setLed(0, 3, 5, true);
}

void drawHexagon() {
  for (int i = 3; i <= 4; i++) {
    lc.setLed(0, 1, i, true);
    lc.setLed(0, 6, i, true);
  }
  lc.setLed(0, 2, 2, true); lc.setLed(0, 2, 5, true);
  lc.setLed(0, 3, 1, true); lc.setLed(0, 3, 6, true);
  lc.setLed(0, 4, 1, true); lc.setLed(0, 4, 6, true);
  lc.setLed(0, 5, 2, true); lc.setLed(0, 5, 5, true);
}

// Pisca a forma correspondente 3 vezes
void animateShape(int shape) {
  for (int i = 0; i < 3; i++) {
    lc.clearDisplay(0);
    delay(150);
    drawShape(shape);
    delay(150);
  }
}

// Digitos simplificados: 3x5 (centralizados verticalmente)
byte digits[10][5] = {
  {B111, B101, B101, B101, B111}, // 0
  {B010, B110, B010, B010, B111}, // 1
  {B111, B001, B111, B100, B111}, // 2
  {B111, B001, B111, B001, B111}, // 3
  {B101, B101, B111, B001, B001}, // 4
  {B111, B100, B111, B001, B111}, // 5
  {B111, B100, B111, B101, B111}, // 6
  {B111, B001, B010, B010, B010}, // 7
  {B111, B101, B111, B101, B111}, // 8
  {B111, B101, B111, B001, B111}  // 9
};

void displayDigit(int digit, int colOffset = 2) {
  lc.clearDisplay(0);
  for (int row = 1; row <= 5; row++) {
    byte line = digits[digit][row - 1];
    for (int bit = 0; bit < 3; bit++) {
      bool led = bitRead(line, 2 - bit);
      lc.setLed(0, row + 1, bit + colOffset, led);
    }
  }
  delay(800);
}

void displayNumber(int num) {
  lc.clearDisplay(0);

  if (num < 10) {
    // Número com 1 dígito → centralizado
    for (int row = 0; row < 5; row++) {
      byte line = digits[num][row];
      for (int col = 0; col < 3; col++) {
        bool led = bitRead(line, 2 - col);
        lc.setLed(0, row + 1, col + 2, led); // Centralizado nas colunas 2-4
      }
    }
  } else {
    // Número com 2 dígitos → mostrar os dois lado a lado
    int tens = num / 10;
    int units = num % 10;

    for (int row = 0; row < 5; row++) {
      byte left = digits[tens][row];
      byte right = digits[units][row];

      for (int col = 0; col < 3; col++) {
        lc.setLed(0, row + 1, col, bitRead(left, 2 - col));       // colunas 0–2
        lc.setLed(0, row + 1, col + 4, bitRead(right, 2 - col));  // colunas 4–6
      }
    }
  }

  delay(2000); // Tempo de exibição
}
