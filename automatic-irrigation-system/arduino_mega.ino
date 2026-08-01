// Pin definitions for the soil moisture sensors
const int sensor_pins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int moisture_data[4];  // Holds average moisture values for 4 plots
int waterLevel = 0;    // Holds the water level percentage

// Solenoid and pump pins
const int solenoidPin1 = 3;
const int solenoidPin2 = 4;
const int solenoidPin3 = 5;
const int solenoidPin4 = 6;
const int pumpPin1 = 7;
const int pumpPin2 = 8;

// Timing and previous time tracking
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
const long interval1 = 150000;  // 2.5 minutes for Plot 1
const long interval2 = 300000;  // 5 minutes for Plot 2
const long interval3 = 450000;  // 7.5 minutes for Plot 3
const long interval4 = 600000;  // 10 minutes for Plot 4

void setup() {
  Serial.begin(9600);  // Initialize the hardware serial for communication with Uno
 
  // Set all solenoid and pump pins as OUTPUT
  pinMode(solenoidPin1, OUTPUT);
  pinMode(solenoidPin2, OUTPUT);
  pinMode(solenoidPin3, OUTPUT);
  pinMode(solenoidPin4, OUTPUT);
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);

  // Initialize all valves and pumps to be closed (LOW)
  digitalWrite(solenoidPin1, LOW);
  digitalWrite(solenoidPin2, LOW);
  digitalWrite(solenoidPin3, LOW);
  digitalWrite(solenoidPin4, LOW);
  digitalWrite(pumpPin1, LOW);
  digitalWrite(pumpPin2, LOW);
}

void loop() {
  // Read the moisture levels from the sensors (pair of sensors per plot)
  for (int i = 0; i < 4; i++) {
    int sensor1 = analogRead(sensor_pins[i * 2]);
    int sensor2 = analogRead(sensor_pins[i * 2 + 1]);
    moisture_data[i] = (100 - ((sensor1 / 1023.0) * 100) + 100 - ((sensor2 / 1023.0) * 100)) / 2;
  }

  // Read the water level (You can integrate the water level code here)
  waterLevel = getWaterLevel();

  // Make sure we send data when requested (This can be extended)
  if (Serial.available() > 0) {
    char request = Serial.read();  // Read the request byte from the Uno
    if (request == '1') {
      sendData(1);  // Send moisture data for Plot 1
    } else if (request == '2') {
      sendData(2);  // Send moisture data for Plot 2
    } else if (request == '3') {
      sendData(3);  // Send moisture data for Plot 3
    } else if (request == '4') {
      sendData(4);  // Send moisture data for Plot 4
    } else if (request == '5') {
      sendWaterLevel();  // Send water level data
    }
  }

  // Handle watering based on moisture levels and the timer
  handleWatering();
}

void handleWatering() {
  unsigned long currentMillis = millis();
 
  // Check if it is time for Plot 1
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;  // Reset the timer for Plot 1

    if (moisture_data[0] < 20) {
      // Turn on Pump 1 for 20 seconds
      digitalWrite(pumpPin1, HIGH);

      // Open Solenoid 1 for Plot 1 (close others)
      digitalWrite(solenoidPin1, HIGH);
      digitalWrite(solenoidPin2, LOW);
      digitalWrite(solenoidPin3, LOW);
      digitalWrite(solenoidPin4, LOW);

      delay(20000);  // Wait for 20 seconds

      // Turn off Pump 1 after 20 seconds
      digitalWrite(pumpPin1, LOW);

      delay(20000);  // Wait 20 more seconds
      digitalWrite(solenoidPin1, LOW);  // Turn off Solenoid 1
    }
  }

  // Check if it is time for Plot 2
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;  // Reset the timer for Plot 2

    if (moisture_data[1] < 20) {
      // Turn on Pump 1 for 20 seconds
      digitalWrite(pumpPin1, HIGH);

      // Open Solenoid 2 for Plot 2 (close others)
      digitalWrite(solenoidPin1, LOW);
      digitalWrite(solenoidPin2, HIGH);
      digitalWrite(solenoidPin3, LOW);
      digitalWrite(solenoidPin4, LOW);

      delay(20000);  // Wait for 20 seconds

      // Turn off Pump 1 after 20 seconds
      digitalWrite(pumpPin1, LOW);

      delay(20000);  // Wait 20 more seconds
      digitalWrite(solenoidPin2, LOW);  // Turn off Solenoid 2
    }
  }

  // Check if it is time for Plot 3
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis;  // Reset the timer for Plot 3

    if (moisture_data[2] < 20) {
      // Turn on Pump 2 for 20 seconds
      digitalWrite(pumpPin2, HIGH);

      // Open Solenoid 3 for Plot 3 (close others)
      digitalWrite(solenoidPin1, LOW);
      digitalWrite(solenoidPin2, LOW);
      digitalWrite(solenoidPin3, HIGH);
      digitalWrite(solenoidPin4, LOW);

      delay(20000);  // Wait for 20 seconds

      // Turn off Pump 2 after 20 seconds
      digitalWrite(pumpPin2, LOW);

      delay(20000);  // Wait 20 more seconds
      digitalWrite(solenoidPin3, LOW);  // Turn off Solenoid 3
    }
  }

  // Check if it is time for Plot 4
  if (currentMillis - previousMillis4 >= interval4) {
    previousMillis4 = currentMillis;  // Reset the timer for Plot 4

    if (moisture_data[3] < 20) {
      // Turn on Pump 2 for 20 seconds
      digitalWrite(pumpPin2, HIGH);

      // Open Solenoid 4 for Plot 4 (close others)
      digitalWrite(solenoidPin1, LOW);
      digitalWrite(solenoidPin2, LOW);
      digitalWrite(solenoidPin3, LOW);
      digitalWrite(solenoidPin4, HIGH);

      delay(20000);  // Wait for 20 seconds

      // Turn off Pump 2 after 20 seconds
      digitalWrite(pumpPin2, LOW);

      delay(20000);  // Wait 20 more seconds
      digitalWrite(solenoidPin4, LOW);  // Turn off Solenoid 4
    }
  }
}

void sendData(int plot) {
  // Send the moisture data for the requested plot
  int index = plot - 1;
  Serial.print("Plot ");
  Serial.print(plot);
  Serial.print(" Moisture: ");
  Serial.println(moisture_data[index]);
}

void sendWaterLevel() {
  // Send the water level data
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
}

int getWaterLevel() {
  // Insert your water level sensor code here (using I2C or analog)
  return 75;  // Example value for water level (you can replace this with your actual logic)
}

