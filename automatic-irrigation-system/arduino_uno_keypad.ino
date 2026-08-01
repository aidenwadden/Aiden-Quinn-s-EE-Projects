#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// LCD setup (Make sure the address matches your setup)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};    // Connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9};    // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);  // Start serial communication with the Mega
 
  lcd.begin(16, 2);  // Initialize the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Auto Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("System");
 
  delay(2000);  // Wait 2 seconds for the intro message
  lcd.clear();   // Clear the display
}

void loop() {
  char key = keypad.getKey();
 
  if (key) {  // If a key is pressed, perform an action
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Requesting data: ");
    lcd.setCursor(0, 1);
    lcd.print(key);
    delay(1000); // Show the pressed key for a second

    // Based on the key press, send a request to the Mega
    if (key == '1') {
      requestPlotData(1);  // Request data for Plot 1
    } else if (key == '2') {
      requestPlotData(2);  // Request data for Plot 2
    } else if (key == '3') {
      requestPlotData(3);  // Request data for Plot 3
    } else if (key == '4') {
      requestPlotData(4);  // Request data for Plot 4
    } else if (key == '5') {
      requestWaterLevel(); // Request water level data
    }
  }
}

void requestPlotData(int plot) {
  // Send the plot number ('1' to '4') to the Mega to request moisture data for the plot
  Serial.print(plot);  // Send plot number to Mega
 
  // Wait for the response from the Mega
  waitForResponse();
}

void requestWaterLevel() {
  // Send the character '5' to request water level data
  Serial.print('5');  // Send request for water level data
 
  // Wait for the response from the Mega
  waitForResponse();
}

// Function to wait for response from the Mega
void waitForResponse() {
  unsigned long timeout = millis() + 5000;  // Wait for 5 seconds max
  while (millis() < timeout) {
    if (Serial.available() > 0) {
      String response = Serial.readString();  // Read the response from Mega
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(response);  // Display the response from Mega (e.g., moisture level or water level)
      break;  // Exit once response is received
    }
  }

  if (millis() >= timeout) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No response");
    delay(2000);  // Wait before retrying
  }
}

