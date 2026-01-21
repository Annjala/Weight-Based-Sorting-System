# Weight-Based Sorting System 

A simple and interactive hardware project that automatically sorts items into different categories based on their weight.

---

##  Project Overview

This system uses an **ESP32**, a **load cell**, an **HX711 amplifier**, and **two servo motors** to detect the weight of an object and move it into the correct bin.
It measures the weight of an object placed on a load cell.  
Using predefined weight ranges, the ESP32 decides where the item should go and moves servo motors accordingly to sort it.

It’s a practical project to understand:

- How load cells measure weight  
- How HX711 communicates with microcontrollers  
- How servo motors work  
- Basic decision-based automation  
- Serial commands for tare & calibration  

---

##  What You'll Learn

- Reading accurate weight using **HX711**
- Controlling servo motors with **ESP32Servo**
- Setting custom weight-based sorting rules
- Using serial commands (`t`, `c`)  
- Calibrating your load cell  
- Building a beginner-level automation system  

---

##  Features

- High-accuracy weight readings  
- Three customizable sorting categories  
- Automatic servo routing based on measured weight  
- Built-in commands for **tare** and **calibration**  
- Noise-filtering (ignores readings under 5g)  
- Beginner-friendly 
- Works with any load cell (1kg, 5kg, 10kg, etc.)  

---

## 📁 Project Structure

```
weight-sorter/
├── wm.ino            # Main Arduino/ESP32 code
├── README.md         # Project documentation
└── images/ (optional diagrams or wiring images)
```

---

##  How It Works?

1. The ESP32 reads the raw value from the HX711 module.
2. This value is converted into **grams** using your calibration factor.
3. Based on weight:
   - **Light items** → Moves Servo 2  
   - **Medium items** → Moves Servo 1 + Servo 2  
   - **Heavy items** → Moves Servo 1 further  
4. After sorting, both servos return to **home position (0°)**.
5. You can interact with the system using Serial commands:
   - `t` → tare  
   - `c` → calibration  

---

##  Hardware Setup

### Required Components

- ESP32 Development Board  
- HX711 Load Cell Amplifier  
- Load Cell (any capacity)  
- 2× Servo Motors (SG90/MG995)  
- Jumper Wires  
- *(Optional)* External 5V supply for servos
- (A breadboard is a must!!)  

---

### Wiring Guide

#### **HX711 → ESP32**

| HX711 | ESP32 |
|-------|--------|
| VCC   | 3.3V |
| GND   | GND |
| DT    | GPIO 16 |
| SCK   | GPIO 4 |

#### **Servos → ESP32**

| Servo | Pin |
|--------|-------|
| Servo 1 | GPIO 13 |
| Servo 2 | GPIO 12 |

(⚠️ Tip: If your servos jitter or lag, use an external 5V supply.)

---

##  Calibration & Testing

### ✔️ Calibration Mode (`c`)

1. Open Serial Monitor  
2. Type `c`  
3. Remove all weight  
4. Place a known weight (e.g., **100g**)  
5. Enter the value when prompted  
6. Code calculates a **new calibration factor**

### ✔️ Tare (`t`)

Resets the current weight to **zero**.

### Tips for Best Results

- Calibrate only once properly  
- Update calibration factor inside code  
- Keep the load cell stable  
- Use external power for large servos  

---

##  How to Use the System

1. Upload the `WBSS.ino` code to ESP32  
2. Open Serial Monitor (115200 baud)  
3. Tare the scale using `t`  
4. Calibrate if needed using `c`  
5. Place an object on the load cell  
6. Watch the servos move it into the correct category  

(You can customize the thresholds in the code anytime.)

---

##  Customization Options

### Change Weight Categories
Modify your sorting logic:

```cpp
if (weight < 50) {
    // light item
}
else if (weight >= 50 && weight <= 100) {
    // medium
}
else {
    // heavy
}
```

### Change Servo Angles

```cpp
servo1.write(90);
servo2.write(150);
```

### Change Home Position

```cpp
const int START_POSITION = 0;
```

---

##  Something More

- Improper wiring of HX711 causes unstable readings  
- Servo motors require enough current  
- Ensure load cell is not moving during weight measurement  
- Keep calibration weight accurate for better precision  

---

Happy building! 🚀 
