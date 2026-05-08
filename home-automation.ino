//final voltura code 


#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <Preferences.h>

BluetoothSerial SerialBT;
HardwareSerial sim800(1);
Preferences prefs;

// Relay Pins (Active LOW)
#define RELAY1 18
#define RELAY2 19
#define RELAY3 21
#define RELAY4 22

// Buzzer
#define BUZZER 26

// Variables
String senderNumber = "";
String btCommand = "";
bool networkReady = false;
bool simPresent = false;
unsigned long lastNetworkCheck = 0;
unsigned long lastSimCheck = 0;
unsigned long startupTime = 0;

// ───────────────────────────────
// ✅ Startup beep
// ───────────────────────────────
void startupBeep() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}

// ───────────────────────────────
// ✅ Steady network beep
// ───────────────────────────────
void networkBeep() {
  digitalWrite(BUZZER, HIGH);
  delay(2000);
  digitalWrite(BUZZER, LOW);
}

// ───────────────────────────────
// ✅ Setup
// ───────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("⚙️ Booting Voltura4 Controller...");

  SerialBT.begin("voltura4");

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Default OFF
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  // Restore saved relay states
  prefs.begin("relay", true);
  digitalWrite(RELAY1, prefs.getBool("r1", false) ? LOW : HIGH);
  digitalWrite(RELAY2, prefs.getBool("r2", false) ? LOW : HIGH);
  digitalWrite(RELAY3, prefs.getBool("r3", false) ? LOW : HIGH);
  digitalWrite(RELAY4, prefs.getBool("r4", false) ? LOW : HIGH);
  prefs.end();

  startupBeep();

  // Attempt to init SIM800L
  sim800.begin(9600, SERIAL_8N1, 16, 17);
  delay(100);
  sim800.println("ATE0");
  delay(100);
  sim800.println("AT+CMGF=1");
  delay(100);
  sim800.println("AT+CNMI=2,2,0,0,0");
  delay(100);

  // Clear all messages
  sim800.println("AT+CMGD=1,4");
  delay(500);
  while (sim800.available()) sim800.read();

  // Detect if SIM800L is responding
  simPresent = false;
  sim800.println("AT");
  delay(200);
  if (sim800.available()) {
    String resp = sim800.readString();
    if (resp.indexOf("OK") >= 0) simPresent = true;
  }

  if (simPresent) Serial.println("✅ SIM800L Detected");
  else Serial.println("⚠️ SIM800L Not Detected, Bluetooth Only Mode");

  startupTime = millis();
}

// ───────────────────────────────
// ✅ Loop
// ───────────────────────────────
void loop() {
  // Bluetooth always active
  checkForBluetooth();

  // SMS check only if SIM present
  if (simPresent && millis() - lastSimCheck > 2000) {
    checkForSMS();
    lastSimCheck = millis();
  }

  // Network beep / ready SMS only if SIM present
  if (simPresent && !networkReady && millis() - lastNetworkCheck > 3000) {
    checkNetwork();
    lastNetworkCheck = millis();
  }
}

// ───────────────────────────────
// ✅ Bluetooth command handler
// ───────────────────────────────
void checkForBluetooth() {
  while (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '\n' || c == '\r') continue;
    btCommand += c;
    delay(5);
  }

  if (btCommand.length() > 0) {
    btCommand.trim();
    btCommand.toUpperCase();
    handleRelayCommand(btCommand, false);
    btCommand = "";
  }
}

// ───────────────────────────────
// ✅ SMS command handler
// ───────────────────────────────
void checkForSMS() {
  if (sim800.available()) {
    String line = sim800.readStringUntil('\n');
    line.trim();
    if (line.startsWith("+CMT:")) {
      int a = line.indexOf('"');
      int b = line.indexOf('"', a + 1);
      senderNumber = line.substring(a + 1, b);
      senderNumber.replace("+", "");

      String body = sim800.readStringUntil('\n');
      body.trim();
      body.toUpperCase();

      handleRelayCommand(body, true);
      while (sim800.available()) sim800.read();
    }
  }
}

// ───────────────────────────────
// ✅ Network check
// ───────────────────────────────
void checkNetwork() {
  sim800.println("AT+CREG?");
  delay(150);
  String resp = "";
  while (sim800.available()) resp += sim800.readString();
  if (resp.indexOf("+CREG: 0,1") >= 0 || resp.indexOf("+CREG: 0,5") >= 0) {
    networkReady = true;
    networkBeep();
    sendReadySMS();
  }
}

// ───────────────────────────────
// ✅ Relay commands with descriptive feedback
// ───────────────────────────────
void handleRelayCommand(String cmd, bool isSMS) {
  prefs.begin("relay", false);

  String msg = "";

  if (cmd == "ON1") {
    if (digitalRead(RELAY1) == LOW) msg = "Port 1 is already ON";
    else { digitalWrite(RELAY1, LOW); prefs.putBool("r1", true); msg = "Port 1 turned ON"; }
  }
  else if (cmd == "OFF1") {
    if (digitalRead(RELAY1) == HIGH) msg = "Port 1 is already OFF";
    else { digitalWrite(RELAY1, HIGH); prefs.putBool("r1", false); msg = "Port 1 turned OFF"; }
  }
  else if (cmd == "ON2") {
    if (digitalRead(RELAY2) == LOW) msg = "Port 2 is already ON";
    else { digitalWrite(RELAY2, LOW); prefs.putBool("r2", true); msg = "Port 2 turned ON"; }
  }
  else if (cmd == "OFF2") {
    if (digitalRead(RELAY2) == HIGH) msg = "Port 2 is already OFF";
    else { digitalWrite(RELAY2, HIGH); prefs.putBool("r2", false); msg = "Port 2 turned OFF"; }
  }
  else if (cmd == "ON3") {
    if (digitalRead(RELAY3) == LOW) msg = "Port 3 is already ON";
    else { digitalWrite(RELAY3, LOW); prefs.putBool("r3", true); msg = "Port 3 turned ON"; }
  }
  else if (cmd == "OFF3") {
    if (digitalRead(RELAY3) == HIGH) msg = "Port 3 is already OFF";
    else { digitalWrite(RELAY3, HIGH); prefs.putBool("r3", false); msg = "Port 3 turned OFF"; }
  }
  else if (cmd == "ON4") {
    if (digitalRead(RELAY4) == LOW) msg = "Port 4 is already ON";
    else { digitalWrite(RELAY4, LOW); prefs.putBool("r4", true); msg = "Port 4 turned ON"; }
  }
  else if (cmd == "OFF4") {
    if (digitalRead(RELAY4) == HIGH) msg = "Port 4 is already OFF";
    else { digitalWrite(RELAY4, HIGH); prefs.putBool("r4", false); msg = "Port 4 turned OFF"; }
  }
  else if (cmd == "ALLON") {
    if (digitalRead(RELAY1) == LOW && digitalRead(RELAY2) == LOW &&
        digitalRead(RELAY3) == LOW && digitalRead(RELAY4) == LOW) msg = "All ports are already ON";
    else {
      digitalWrite(RELAY1, LOW); digitalWrite(RELAY2, LOW);
      digitalWrite(RELAY3, LOW); digitalWrite(RELAY4, LOW);
      prefs.putBool("r1", true); prefs.putBool("r2", true);
      prefs.putBool("r3", true); prefs.putBool("r4", true);
      msg = "All ports turned ON";
    }
  }
  else if (cmd == "ALLOFF") {
    if (digitalRead(RELAY1) == HIGH && digitalRead(RELAY2) == HIGH &&
        digitalRead(RELAY3) == HIGH && digitalRead(RELAY4) == HIGH) msg = "All ports are already OFF";
    else {
      digitalWrite(RELAY1, HIGH); digitalWrite(RELAY2, HIGH);
      digitalWrite(RELAY3, HIGH); digitalWrite(RELAY4, HIGH);
      prefs.putBool("r1", false); prefs.putBool("r2", false);
      prefs.putBool("r3", false); prefs.putBool("r4", false);
      msg = "All ports turned OFF";
    }
  }
  else if (cmd == "STATUS") {
    msg = "Port Status:\n";
    msg += "Port 1: " + String(prefs.getBool("r1") ? "ON" : "OFF") + "\n";
    msg += "Port 2: " + String(prefs.getBool("r2") ? "ON" : "OFF") + "\n";
    msg += "Port 3: " + String(prefs.getBool("r3") ? "ON" : "OFF") + "\n";
    msg += "Port 4: " + String(prefs.getBool("r4") ? "ON" : "OFF");
  }

  reply(msg, isSMS);
  prefs.end();
}

// ───────────────────────────────
// ✅ Reply
// ───────────────────────────────
void reply(String msg, bool isSMS) {
  if (isSMS && simPresent) sendSMS(senderNumber, msg);
  else SerialBT.println(msg);
}

// ───────────────────────────────
// ✅ Ready SMS
// ───────────────────────────────
void sendReadySMS() {
  if (!simPresent) return;
  sendSMS("6386512365", "✅ Voltura4 is READY & NETWORK OK");
}

// ───────────────────────────────
// ✅ Send SMS
// ───────────────────────────────
void sendSMS(String number, String text) {
  sim800.println("AT+CMGS=\"" + number + "\"");
  delay(100);
  sim800.print(text);
  delay(100);
  sim800.write(26);
  delay(300);
}