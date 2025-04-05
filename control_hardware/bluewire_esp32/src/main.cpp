/*!
 * @license     The MIT License (MIT)
 * project bluewire - https://github.com/albersgroup/bluewire
 * some code modified from: https://github.com/DFRobot/DFRobot_VisualRotaryEncoder
 */
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <MQTTClient.h>
#include <DFRobot_VisualRotaryEncoder.h>
#include <ArduinoJson.h>
#include <Preferences.h>

WebServer server(80);
DNSServer dnsServer;
WiFiClient network;
MQTTClient mqtt(256);
Preferences prefs;

// Sensor config
DFRobot_VisualRotaryEncoder_I2C sensor(0x54, &Wire);

// Wi-Fi/AP config
const char *apSSID = "bluewire_setup";
const char *apPassword = "bluewire1234";

// MQTT config
const char MQTT_BROKER_ADRRESS[] = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "esp32-test-client";
String valuePublishTopic;
String controlPublishTopic;
String subscribeTopic;
const int PUBLISH_INTERVAL = 1500;

String providedSSID, providedPassword, company, project;
unsigned long lastPublishTime = 0;
bool control = false;

// Terminal-style HTML
const char *form_html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>bluewire setup</title>
  <style>
    body { background-color: #2e2e2e; color: #00aaff; font-family: monospace; padding: 20px; }
    pre.ascii { color: #00ffff; font-size: 14px; line-height: 1.2; margin-bottom: 30px; }
    label, input { display: block; margin: 10px 0; }
    input[type="text"], input[type="password"] {
      background-color: #1e1e1e; color: #00aaff; border: 1px solid #00aaff; padding: 5px; width: 300px;
    }
    input[type="submit"] {
      margin-top: 15px; padding: 8px 20px; background-color: #1e1e1e;
      border: 1px solid #00aaff; color: #00aaff; cursor: pointer;
    }
    input[type="submit"]:hover { background-color: #00aaff; color: #1e1e1e; }
  </style>
</head>
<body>
  <pre class="ascii">

  dP       dP                              oo                   
  88       88                                                   
  88d888b. 88 dP    dP .d8888b. dP  dP  dP dP 88d888b. .d8888b. 
  88'  `88 88 88    88 88ooood8 88  88  88 88 88'  `88 88ooood8 
  88.  .88 88 88.  .88 88.  ... 88.88b.88' 88 88       88.  ... 
  88Y8888' dP `88888P' `88888P' 8888P Y8P  dP dP       `88888P' 



                bluewire setup
  </pre>

  <form action="/submit" method="POST">
    <label for="ssid">SSID:</label>
    <input type="text" id="ssid" name="ssid" required>

    <label for="password">Password:</label>
    <input type="password" id="password" name="password" required>

    <label for="company">Company:</label>
    <input type="text" id="company" name="company">

    <label for="project">Project:</label>
    <input type="text" id="project" name="project">

    <input type="submit" value="CONNECT">
  </form>
</body>
</html>
)rawliteral";

// Forward declarations
void handleRoot();
void handleFormSubmit();
void startAPMode();
void initAfterWiFi();
void connectToMQTT();
void messageHandler(String &topic, String &payload);
void sendToMQTT();
bool loadCredentials(String &ssid, String &password, String &company, String &project);
void saveCredentials(const String& ssid, const String& password, const String& company, const String& project);

void buildMQTTTopics() {
  valuePublishTopic   = company + "/" + project + "/hil/value";
  controlPublishTopic = company + "/" + project + "/hil/control";
  subscribeTopic      = company + "/" + project + "/hil/control";
}

void saveCredentials(const String& ssid, const String& password, const String& company, const String& project) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("password", password);
  prefs.putString("company", company);
  prefs.putString("project", project);
  prefs.end();
}

bool loadCredentials(String &ssid, String &password, String &company, String &project) {
  prefs.begin("wifi", true);
  ssid = prefs.getString("ssid", "");
  password = prefs.getString("password", "");
  company = prefs.getString("company", "");
  project = prefs.getString("project", "");
  prefs.end();
  return (ssid.length() > 0 && password.length() > 0);
}

void handleRoot() {
  server.send(200, "text/html", form_html);
}

void handleFormSubmit() {
  providedSSID = server.arg("ssid");
  providedPassword = server.arg("password");
  company = server.arg("company");
  project = server.arg("project");

  saveCredentials(providedSSID, providedPassword, company, project);
  server.send(200, "text/html", "<h3>Connecting to Wi-Fi...</h3>");

  delay(1000);
  WiFi.softAPdisconnect(true);
  WiFi.begin(providedSSID.c_str(), providedPassword.c_str());

  Serial.print("Connecting to ");
  Serial.println(providedSSID);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to new Wi-Fi:");
    Serial.println(WiFi.localIP());
    initAfterWiFi();
  } else {
    Serial.println("\nFailed to connect.");
    startAPMode();  // fallback
  }
}

void startAPMode() {
  WiFi.softAP(apSSID, apPassword);
  delay(100);

  IPAddress apIP = WiFi.softAPIP();
  dnsServer.start(53, "*", apIP); // Captive portal redirect

  Serial.println("Started AP mode");
  Serial.println("Connect to Wi-Fi and open http://192.168.4.1");

  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleFormSubmit);
  server.begin();
}

void connectToMQTT() {
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);
  mqtt.onMessage(messageHandler);
  Serial.print("Connecting to MQTT broker");

  int attempts = 0;
  while (!mqtt.connect(MQTT_CLIENT_ID) && attempts++ < 20) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();

  if (mqtt.connected()) {
    Serial.println("MQTT connected.");
    mqtt.subscribe(subscribeTopic.c_str());
  } else {
    Serial.println("Failed to connect to MQTT broker.");
  }
}

void messageHandler(String &topic, String &payload) {
  Serial.println("Received MQTT message:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload: " + payload);
}

void sendToMQTT() {
  char messageBuffer[10]; // Buffer size is enough for a uint16_t value and null-terminator
  uint16_t sensorValue = sensor.getEncoderValue();
  snprintf(messageBuffer, sizeof(messageBuffer), "%u", sensorValue); // Convert to string

  mqtt.publish(valuePublishTopic.c_str(), messageBuffer, sizeof(messageBuffer), true);
  Serial.println("Sent value to MQTT:");
  Serial.println(messageBuffer);
}

void initAfterWiFi() {
  // Initialize rotary encoder
  while (sensor.begin() != NO_ERR) {
    Serial.println("Sensor connection failed, retrying...");
    delay(2000);
  }

  sensor.setGainCoefficient(51);
  Serial.print("Gain set to: ");
  Serial.println(sensor.getGainCoefficient());

  connectToMQTT();
}

void checkResetButtonHold() {
  Serial.println("Hold the encoder button now to reset Wi-Fi credentials...");

  int heldTime = 0;
  const int resetHoldTime = 5000; // 5 seconds

  while (heldTime < resetHoldTime) {
    if (!sensor.detectButtonDown()) {
      Serial.println("No reset triggered.");
      return;
    }
    delay(100);
    heldTime += 100;
  }

  Serial.println("Factory reset triggered! Clearing saved credentials...");
  prefs.begin("wifi", false);
  prefs.clear();
  prefs.end();
  delay(1000);

  // Optional: blink LED or provide feedback
  ESP.restart(); // Restart into AP mode
}

void setup() {
  Serial.begin(115200);

  while (sensor.begin() != NO_ERR) {
    Serial.println("Sensor init failed, retrying...");
    delay(2000);
  }
  sensor.setGainCoefficient(51);

  checkResetButtonHold();

  String ssid, pass, comp, proj;
  if (loadCredentials(ssid, pass, comp, proj)) {
    Serial.println("Found saved Wi-Fi credentials. Attempting to connect...");
    WiFi.begin(ssid.c_str(), pass.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts++ < 20) {
      Serial.print(".");
      delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to saved Wi-Fi.");
      providedSSID = ssid;
      providedPassword = pass;
      company = comp;
      project = proj;
      buildMQTTTopics();
      initAfterWiFi();
      return;
    } else {
      Serial.println("\nWi-Fi connect failed.");
    }
  } else {
    Serial.println("No saved credentials.");
  }

  startAPMode();
}

unsigned long buttonHoldStart = 0;
bool buttonIsHeld = false;
bool buttonHeldResetTriggered = false;

void loop() {
  server.handleClient();
  dnsServer.processNextRequest();
  mqtt.loop();

  // ========== Track long press with detectButtonDown() + encoder value ==========
  static bool lastButtonDown = false;
  bool currentButtonDown = sensor.detectButtonDown();

  // detectButtonDown only fires on press, not hold — so we use encoderValue hack
  static uint16_t lastEncoderValue = sensor.getEncoderValue();
  uint16_t currentEncoderValue = sensor.getEncoderValue();
  bool encoderUnchanged = (currentEncoderValue == lastEncoderValue);
  lastEncoderValue = currentEncoderValue;

  if (currentButtonDown && !lastButtonDown) {
    // New press detected
    buttonHoldStart = millis();
    buttonIsHeld = true;
    Serial.println("Button press started");
  }

  if (buttonIsHeld) {
    if (!encoderUnchanged) {
      // User turned knob, cancel the hold
      buttonIsHeld = false;
      buttonHoldStart = 0;
    } else if (millis() - buttonHoldStart >= 5000 && !buttonHeldResetTriggered) {
      Serial.println("Button held 5s. Clearing credentials and restarting.");
      buttonHeldResetTriggered = true;

      prefs.begin("wifi", false);
      prefs.clear();
      prefs.end();
      delay(1000);

      ESP.restart();
    }
  }

  if (!currentButtonDown) {
    // Button released
    buttonIsHeld = false;
    buttonHoldStart = 0;
  }

  lastButtonDown = currentButtonDown;

  // ========== Normal encoder logic ==========
  // Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
    if (currentButtonDown) {
      sensor.setEncoderValue(0);
      control = true;
      char messageBuffer[100] = "REMOTE";
      mqtt.publish(controlPublishTopic.c_str(), messageBuffer, sizeof(messageBuffer), true);
      Serial.println("Button pressed. Sent control message.");
      Serial.println(messageBuffer);
    }

    Serial.print("Encoder Value: ");
    Serial.println(currentEncoderValue);

    if (millis() - lastPublishTime > PUBLISH_INTERVAL && control) {
      sendToMQTT();
      lastPublishTime = millis();
    }
  }

  delay(500);
}