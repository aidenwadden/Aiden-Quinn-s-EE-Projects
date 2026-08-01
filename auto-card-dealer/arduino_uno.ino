// ============================================================
//  ELECTRIC JACKS ⚡
//  Automatic Card Dealing System – Wiring Diagram & Pin Setup
// ============================================================
//
//  HARDWARE OVERVIEW:
//  - Microcontroller: Arduino Uno (ATmega328P)
//  - Rotation Motor (Lazy Susan): FS90R Continuous Rotation Servo
//  - Card Flicking Motor: SG90 9g Micro Servo
//  - Input Device: XY Joystick Module (VRx, VRy, SW)
//  - Display (optional): 16x2 LCD w/ I2C Backpack
//
// ------------------------------------------------------------
//  JOYSTICK WIRING:
//    VRx  → A0   // Left/Right Analog Input
//    VRy  → A1   // Up/Down Analog Input
//    SW   → D2   // Joystick Button (active LOW)
//    VCC  → 5V
//    GND  → GND
//
// ------------------------------------------------------------
//  FS90R ROTATION SERVO (LAZY SUSAN):
//    Signal → D9
//    +5V   → External 5V Supply
//    GND   → Shared Ground w/ Arduino
//
//    *Note: FS90R requires stable power. Do NOT run it off
//    Arduino 5V. Use an external 5V supply (1–2A).  
//
// ------------------------------------------------------------
//  SG90 FLICKING SERVO (CARD PUSHER):
//    Signal → D10
//    +5V   → External 5V Supply
//    GND   → Shared Ground w/ Arduino
//
// ------------------------------------------------------------
//  OPTIONAL: 16x2 I2C LCD DISPLAY
//    SDA   → A4
//    SCL   → A5
//    VCC   → 5V
//    GND   → GND
//
// ------------------------------------------------------------
//  POWER SETUP:
//  - FS90R + SG90 servos powered by 5V 2A supply
//  - Arduino powered by USB or 9V barrel jack
//  - MUST share grounds between Arduino + servo power supply
//
// ------------------------------------------------------------
//  NOTES:
//  - Keep servo wires short to reduce jitter.
//  - Use separate +5V supply for motors to prevent resets.
//  - Use writeMicroseconds() for smoother FS90R control if needed.
// ============================================================


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h>

// --------------------------
// Joystick Pins
// --------------------------
const int VRx = A0;  
const int VRy = A1;  
const int JSW = 2;   

// --------------------------
// Game Settings
// --------------------------
int currentGame = 1;
int totalGames  = 3;

int playerCount = 2;
int maxPlayers  = 4;

unsigned long lastMove = 0;
const int moveDelay = 300;

// --------------------------
// Flicker Servo
// --------------------------
const int SERVO_FLICK_PIN = 10;
Servo flickServo;

const int FLICK_STOP = 90;
const int FLICK_FWD  = 135;
const int FLICK_REV  = 60;

int FLICK_FWD_TIME  = 500;
int FLICK_BACK_TIME = 200;
int FLICK_SETTLE    = 400;

// --------------------------
// Stepper Motor
// --------------------------
const int stepsPerRev = 2048;
Stepper deckStepper(stepsPerRev, 3, 5, 4, 6);

const int NUM_SEATS = 5;  
int stepsPerSeat = stepsPerRev / NUM_SEATS;

int currentSeat = 0;

// --------------------------
// LCD
// --------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Game names for LCD
String gameNames[4] = {
  "",       // ignore index 0
  "BJ",
  "POKER",
  "EUCHRE"
};

// ===================================================
//           LCD UPDATE FUNCTION
// ===================================================
void updateLCD() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Game: ");
  lcd.print(gameNames[currentGame]);

  lcd.setCursor(0, 1);
  lcd.print("Players: ");
  lcd.print(playerCount);
}

// ===================================================
//           SETUP
// ===================================================
void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  updateLCD();

  pinMode(JSW, INPUT_PULLUP);

  flickServo.attach(SERVO_FLICK_PIN);
  flickServo.write(FLICK_STOP);

  deckStepper.setSpeed(10);

  Serial.println("=== MENU READY ===");
}

// ===================================================
//           MAIN LOOP
// ===================================================
void loop() {
  int xVal = analogRead(VRx);
  int yVal = analogRead(VRy);
  bool click = (digitalRead(JSW) == LOW);

  unsigned long now = millis();

  if (now - lastMove > moveDelay) {

    if (yVal < 300) {              // game up
      currentGame--;
      if (currentGame < 1) currentGame = totalGames;
      lastMove = now;
      updateLCD();
    }
    else if (yVal > 700) {         // game down
      currentGame++;
      if (currentGame > totalGames) currentGame = 1;
      lastMove = now;
      updateLCD();
    }
    else if (xVal < 300) {         // players --
      playerCount--;
      if (playerCount < 1) playerCount = 1;
      lastMove = now;
      updateLCD();
    }
    else if (xVal > 700) {         // players ++
      playerCount++;
      if (playerCount > maxPlayers) playerCount = maxPlayers;
      lastMove = now;
      updateLCD();
    }
  }

  if (click) {
    Serial.println(">>> START GAME <<<");

    if (currentGame == 1) runGame1(playerCount);
    else if (currentGame == 2) runGame2(playerCount);
    else if (currentGame == 3) runGame3(playerCount);

    flickServo.write(FLICK_STOP);
    delay(500);
  }
}

// ===================================================
//           SUPPORT FUNCTIONS
// ===================================================
void flickOnce() {
  Serial.println("    Flick!");
  flickServo.write(FLICK_FWD);
  delay(FLICK_FWD_TIME);
  flickServo.write(FLICK_REV);
  delay(FLICK_BACK_TIME);
  flickServo.write(FLICK_STOP);
  delay(FLICK_SETTLE);
}

void moveToSeat(int targetSeat) {
  if (targetSeat < 0 || targetSeat >= NUM_SEATS) return;

  int diff = targetSeat - currentSeat;
  int steps = diff * stepsPerSeat;

  deckStepper.step(steps);
  delay(200);

  currentSeat = targetSeat;
}

// ===================================================
//           GAME 1: BLACKJACK
// ===================================================
void runGame1(int players) {
  currentSeat = 0;

  for (int round = 1; round <= 2; round++) {
    for (int seat = 1; seat <= players; seat++) {
      moveToSeat(seat);
      flickOnce();
    }
    moveToSeat(0);
    flickOnce();
  }
}

// ===================================================
//           GAME 2: TEXAS HOLD'EM
// ===================================================
void runGame2(int players) {
  currentSeat = 0;

  for (int round = 1; round <= 2; round++) {
    for (int seat = 1; seat <= players; seat++) {
      moveToSeat(seat);
      flickOnce();
    }
  }

  moveToSeat(0);
  for (int i = 1; i <= 5; i++) flickOnce();
}

// ===================================================
//           GAME 3: MICHIGAN EUCHRE
// ===================================================
void runGame3(int players) {
  if (players < 2) players = 2;
  if (players > 4) players = 4;

  currentSeat = 0;

  int firstRound[4]  = {3, 2, 3, 2};
  int secondRound[4] = {2, 3, 2, 3};

  for (int seat = 1; seat <= players; seat++) {
    moveToSeat(seat);
    for (int i = 0; i < firstRound[seat - 1]; i++) flickOnce();
  }

  for (int seat = 1; seat <= players; seat++) {
    moveToSeat(seat);
    for (int i = 0; i < secondRound[seat - 1]; i++) flickOnce();
  }

  moveToSeat(0);
  for (int i = 0; i < 4; i++) flickOnce();
}

