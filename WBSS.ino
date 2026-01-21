/*
 * ESP32 + HX711 Weight-Based Sorting System
 * 
 * Wiring:
 * HX711 VCC -> ESP32 3.3V
 * HX711 GND -> ESP32 GND
 * HX711 DT (Data) -> ESP32 GPIO 16
 * HX711 SCK (Clock) -> ESP32 GPIO 4
 * Servo 1 -> ESP32 GPIO 13
 * Servo 2 -> ESP32 GPIO 12
 * 
 * Install Libraries: 
 * - HX711 by Bogdan Necula
 * - ESP32Servo
 */

#include <HX711.h>
#include <ESP32Servo.h>


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

HX711 scale;

// Calibration factor - adjust this for your load cell
float calibration_factor = -7050;

// Servo motors
Servo servo1;  // SM_1
Servo servo2;  // SM_2

const int SERVO1_PIN = 13;  // Pin for Servo Motor 1
const int SERVO2_PIN = 12;  // Pin for Servo Motor 2

const int START_POSITION = 0; // Starting position for both servos

void setup() {
  Serial.begin(115200);
  Serial.println("Weight-Based Servo Sorting System");
  
  // Initialize the scale
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  Serial.println("Initializing scale...");
  
  if (scale.is_ready()) {
    Serial.println("HX711 found.");
  } else {
    Serial.println("HX711 not found. Check wiring!");
    while (1);
  }
  
  // Reset the scale to 0
  scale.set_scale();
  scale.tare();
  
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place items on scale for sorting");
  Serial.println("Press 't' to tare (reset to zero)");
  Serial.println("Press 'c' to enter calibration mode");
  delay(2000);
  
  scale.set_scale(calibration_factor);
  
  // Initialize servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  
  // Set both servos to starting position
  servo1.write(START_POSITION);
  servo2.write(START_POSITION);
  
  Serial.println("System Ready!");
}

void loop() {
  // Check if scale is ready
  if (scale.is_ready()) {
    float weight = scale.get_units(10); // Average of 10 readings
    
    Serial.print("Weight: ");
    Serial.print(weight, 1);
    Serial.println(" g");
    
    // Sort based on weight
    sortByWeight(weight);
    
    // Check for serial commands
    if (Serial.available()) {
      char cmd = Serial.read();
      
      if (cmd == 't' || cmd == 'T') {
        // Tare the scale
        Serial.println("Taring scale...");
        scale.tare();
        Serial.println("Scale tared!");
      }
      else if (cmd == 'c' || cmd == 'C') {
        // Calibration mode
        calibrate();
      }
    }
  } else {
    Serial.println("HX711 not ready!");
  }
  
  delay(500);
}

void sortByWeight(float weight) {
  // Only sort if weight is above threshold (avoid noise)
  if (weight < 5) {
    return; // Ignore readings below 5g
  }
  
  // Weight < 50g: Only rotate SM_2 to 150°
  if (weight < 50) {
    Serial.println("Sorting: Weight < 50g");
    Serial.println("  SM_2 rotating to 150°");
    servo2.write(150);
    delay(1500);
  }
  // Weight between 50-100g: Rotate SM_1 to 90° & SM_2 to 150°
  else if (weight >= 50 && weight <= 100) {
    Serial.println("Sorting: Weight 50-100g");
    Serial.println("  SM_1 rotating to 90°");
    Serial.println("  SM_2 rotating to 150°");
    servo1.write(90);
    servo2.write(150);
    delay(1500);
  }
  // Weight between 101-150g: Rotate SM_1 to 180° & SM_2 to 150°
  else if (weight > 100) {
    Serial.println("Sorting: Weight > 100g");
    Serial.println("  SM_1 rotating to 180°");
    Serial.println("  SM_2 rotating to 150°");
    servo1.write(180);
    servo2.write(150);
    delay(1500);
  }
  // Weight > 150g: No servo action (4th condition removed)
  else {
    Serial.println("Weight > 150g - No sorting action");
    return;
  }
  
  // Return both servos to starting position
  Serial.println("Returning servos to start position");
  servo1.write(START_POSITION);
  servo2.write(START_POSITION);
  delay(1500);
  
  Serial.println("Ready for next item\n");
}

void calibrate() {
  Serial.println("\n--- CALIBRATION MODE ---");
  Serial.println("Remove all weight and press any key...");
  
  while (!Serial.available());
  while (Serial.available()) Serial.read();
  
  scale.set_scale();
  scale.tare();
  Serial.println("Scale tared!");
  
  Serial.println("\nPlace a known weight on scale");
  Serial.println("Enter the weight in grams:");
  
  while (!Serial.available());
  float known_weight = Serial.parseFloat();
  while (Serial.available()) Serial.read();
  
  Serial.print("Reading scale with ");
  Serial.print(known_weight);
  Serial.println("g weight...");
  
  delay(1000);
  
  long reading = scale.get_units(10);
  
  calibration_factor = reading / known_weight;
  scale.set_scale(calibration_factor);
  
  Serial.print("\nNew calibration factor: ");
  Serial.println(calibration_factor);
  Serial.println("Update this value in your code!");
  Serial.println("--- CALIBRATION COMPLETE ---\n");
}