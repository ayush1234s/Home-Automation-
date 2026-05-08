# ⚡ Voltura4 - ESP32 Smart Home Automation System

Voltura4 is a powerful ESP32-based Smart Home Automation System that allows users to control home appliances from anywhere using SMS commands or nearby using Bluetooth.

This project combines:
- 📶 GSM-based remote control
- 📱 Bluetooth automation
- ⚡ Relay appliance switching
- 🔔 Buzzer notifications
- 💾 Memory state saving
- 📡 Network monitoring

The system is designed for reliable real-world home automation and can operate even without internet connectivity.

---

# 🚀 Features

✅ Control appliances from anywhere using SMS  
✅ Bluetooth control for nearby operation  
✅ 4-channel relay control  
✅ Appliance state memory after power cut  
✅ Network detection system  
✅ Startup and network buzzer indication  
✅ Status checking using SMS/Bluetooth  
✅ ALL ON / ALL OFF commands  
✅ SIM800L GSM support  
✅ ESP32-based fast processing  

---

# 📱 Control Methods

## 1. GSM / SMS Control
The user can control appliances remotely from anywhere by sending SMS commands to the SIM card inserted into the SIM800L module.

Example:

| SMS Command | Action |
|---|---|
| ON1 | Turn ON Relay 1 |
| OFF1 | Turn OFF Relay 1 |
| ON2 | Turn ON Relay 2 |
| OFF2 | Turn OFF Relay 2 |
| ON3 | Turn ON Relay 3 |
| OFF3 | Turn OFF Relay 3 |
| ON4 | Turn ON Relay 4 |
| OFF4 | Turn OFF Relay 4 |
| ALLON | Turn ON all appliances |
| ALLOFF | Turn OFF all appliances |
| STATUS | Check relay status |

---

## 2. Bluetooth Control
The system also supports Bluetooth communication using ESP32 Bluetooth Serial.

Bluetooth Name:

```text
voltura4
```

Using any Bluetooth Terminal App, the user can send commands like:

```text
ON1
OFF1
STATUS
```

---

# 🧠 How the System Works

```text
Phone
   ↓
SMS / Bluetooth
   ↓
ESP32 Controller
   ↓
Relay Module
   ↓
Home Appliances
```

The ESP32 receives commands through:
- SIM800L GSM Module
- Bluetooth Serial

Then it processes the command and controls the relay module connected to home appliances.

---

# 🛠 Components Used

| Component | Quantity | Purpose |
|---|---|---|
| ESP32 Dev Board | 1 | Main microcontroller |
| SIM800L GSM Module | 1 | SMS communication |
| 4-Channel Relay Module | 1 | Appliance switching |
| Buzzer | 1 | Sound indications |
| Power Supply | 1 | Power for the system |
| Jumper Wires | Multiple | Connections |
| Home Appliances | Optional | Load devices |
| Mobile Phone | 1 | Sending commands |

---

# 🔍 Component Explanation

## 🔹 ESP32
ESP32 is the main brain of the project.

Functions:
- Reads Bluetooth commands
- Reads SMS commands
- Controls relay outputs
- Stores relay memory states
- Handles logic processing

---

## 🔹 SIM800L GSM Module
SIM800L allows the system to communicate over the mobile network.

Functions:
- Receives SMS commands
- Sends status messages
- Detects network connectivity

This enables worldwide remote control without internet.

---

## 🔹 Relay Module
The relay module acts as an electronic switch.

Functions:
- Turns AC appliances ON/OFF
- Provides electrical isolation
- Controls high-voltage devices safely

Supported devices:
- Lights
- Fans
- Sockets
- Chargers
- Other appliances

---

## 🔹 Buzzer
The buzzer provides audio feedback.

Functions:
- Startup indication
- Network connection indication
- System alerts

---

# ⚡ Relay Working Logic

The relay module used in this project is:

```text
Active LOW Relay
```

Meaning:

| Signal | Relay State |
|---|---|
| LOW | ON |
| HIGH | OFF |

---

# 💾 EEPROM / Memory Feature

The project stores relay states using ESP32 Preferences memory.

Example:
- Light is ON
- Power cut occurs
- System restarts
- Light automatically restores to ON state

This makes the system reliable for real-world automation.

---

# 📡 Network Detection System

The system continuously checks GSM network availability.

When network becomes available:
- Buzzer gives indication
- Ready SMS is sent automatically

---

# 🔔 Buzzer Indications

| Indication | Meaning |
|---|---|
| 3 Startup Beeps | System Boot Successful |
| Long Beep | GSM Network Connected |

---

# 🔌 Pin Configuration

| Device | ESP32 Pin |
|---|---|
| Relay 1 | GPIO 18 |
| Relay 2 | GPIO 19 |
| Relay 3 | GPIO 21 |
| Relay 4 | GPIO 22 |
| Buzzer | GPIO 26 |
| SIM800L RX | GPIO 16 |
| SIM800L TX | GPIO 17 |

---

# 📂 Libraries Used

```cpp
#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <Preferences.h>
```

| Library | Purpose |
|---|---|
| BluetoothSerial | Bluetooth communication |
| HardwareSerial | SIM800L serial communication |
| Preferences | Save relay states |

---

# 📲 Example SMS Commands

Turn ON Relay 1:

```text
ON1
```

Turn OFF Relay 2:

```text
OFF2
```

Check appliance status:

```text
STATUS
```

Turn OFF all appliances:

```text
ALLOFF
```

---

# 🌍 Advantages of This Project

✅ Works without internet  
✅ Long-distance control using GSM  
✅ Fast Bluetooth response  
✅ Power failure recovery  
✅ Low cost automation solution  
✅ Expandable system architecture  
✅ Suitable for smart homes  

---

# 🔮 Future Improvements

- Mobile App Integration
- Voice Assistant Support


---
