/*!
 * @license     The MIT License (MIT)
 * project bluewire - https://github.com/albersgroup/bluewire
 * some code modified from: https://github.com/DFRobot/DFRobot_VisualRotaryEncoder
 */
 #include <DFRobot_VisualRotaryEncoder.h>
 #include <WiFi.h>
 #include <MQTTClient.h>
 #include <ArduinoJson.h>
 
 /**
  * Instantiate an object to drive our sensor;
  * Set address according to encoder DIP switch:
  * | 1 | 2 | ADDR |
  * |---|---|------|
  * | 0 | 0 | 0x54 |
  * | 0 | 1 | 0x55 |
  * | 1 | 0 | 0x56 |
  * | 1 | 1 | 0x57 |
  */
 
 DFRobot_VisualRotaryEncoder_I2C sensor(/*i2cAddr = */0x54, /*i2cBus = */&Wire);
  
 // Network credentials
 const char* ssid = "AA Guest";
 const char* password = "deliver21value";
 
 const char MQTT_BROKER_ADRRESS[] = "test.mosquitto.org";
 const int MQTT_PORT = 1883;
 const char MQTT_CLIENT_ID[] = "esp32-test-client";
 const char VALUE_PUBLISH_TOPIC[] = "aero/albers/hil/value";
 const char CONTROL_PUBLISH_TOPIC[] = "aero/albers/hil/control";
 const char SUBSCRIBE_TOPIC[] = "aero/albers/hil/control";
 const int PUBLISH_INTERVAL = 5000;  // 5 seconds
 
 WiFiClient network;
 MQTTClient mqtt = MQTTClient(256);
 
 unsigned long lastPublishTime = 0;
 
 bool control = false;
 
 void connectToMQTT();
 void sendToMQTT();
 void messageHandler(String &topic, String &payload);
  
 void setup() {
  
     Serial.begin(115200);
 
     WiFi.mode(WIFI_STA);
     WiFi.begin(ssid, password);
     Serial.println("\nConnecting to network...");
 
     while(WiFi.status() != WL_CONNECTED){
         Serial.print(".");
         delay(100);
     }
 
     Serial.println("\nConnected to network.");
     Serial.print("Local IP: ");
     Serial.println(WiFi.localIP());
 
     // Initialize the sensor
     while(NO_ERR != sensor.begin()) {
         Serial.println("Communication with device failed, please check connection");
         delay(3000);
     }
     Serial.println("Begin ok!");
 
     /**
      * Set the current gain factor of the encoder, accuracy value to rotate one detent
      * accuracy range：1~51，the minimum is 1 (light up one LED about every 2.5 turns), the maximum is 51 (light up one LED every one detent rotation)
      * gainValue range[1, 51], the setting is invalid when out of range.
      */
     sensor.setGainCoefficient(51);// The currently set accuracy value: 51
 
     /**
      * Get the current gain factor of the encoder, accuracy value to rotate one detent
      * accuracy range：1~51，the minimum is 1 (light up one LED about every 2.5 turns), the maximum is 51 (light up one LED every one detent rotation)
      * return value range: 1-51
      */
     uint8_t gainCoefficient = sensor.getGainCoefficient();
     Serial.print("Encoder current gain coefficient: ");
     Serial.println(gainCoefficient);
 
     Serial.println();
     delay(1000);
 
     connectToMQTT();
 }
 
 void loop() {
   /**
    * Detect if the button is pressed
    * return true when the button pressed，otherwise, return false
    */
   if(sensor.detectButtonDown()) {
     /**
      * Set the encoder count value
      * value range[0, 1023], the setting is invalid when out of range
      * In this example, set the encoder value to zero when detecting the button pressed, and you can see all the LEDs that light up before turning off
      */
     sensor.setEncoderValue(0);
     control = true;
 
     char messageBuffer[100] = "REMOTE";
 
     mqtt.publish(CONTROL_PUBLISH_TOPIC, messageBuffer, sizeof(messageBuffer), true);
 
     Serial.println("ESP32 - sent to MQTT:");
     Serial.println(messageBuffer);
   }
 
   /**
    * Get the encoder current count 
    * return value range： 0-1023
    */
   uint16_t encoderValue = sensor.getEncoderValue();
   Serial.print("The encoder current counts: ");
   Serial.println(encoderValue);
 
   Serial.println();
 
   mqtt.loop();
 
   if (millis() - lastPublishTime > PUBLISH_INTERVAL && control == true) {
     sendToMQTT();
     lastPublishTime = millis();
   }
 
   delay(1000);
 }
 
 void connectToMQTT() {
   // Connect to the MQTT broker
   mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);
 
   // Create a handler for incoming messages
   mqtt.onMessage(messageHandler);
 
   Serial.print("ESP32 - Connecting to MQTT broker");
 
   while (!mqtt.connect(MQTT_CLIENT_ID)) {
     Serial.print(".");
     delay(100);
   }
   Serial.println();
 
   if (!mqtt.connected()) {
     Serial.println("ESP32 - MQTT broker Timeout!");
     return;
   }
 
   // Subscribe to a topic, the incoming messages are processed by messageHandler() function
   if (mqtt.subscribe(SUBSCRIBE_TOPIC))
     Serial.print("ESP32 - Subscribed to the topic: ");
   else
     Serial.print("ESP32 - Failed to subscribe to the topic: ");
 
   Serial.println(SUBSCRIBE_TOPIC);
   Serial.println("ESP32 - MQTT broker Connected!");
 }
 
 void sendToMQTT() {
 
   char messageBuffer[10]; // Buffer size is enough for a uint16_t value and null-terminator
   uint16_t sensorValue = sensor.getEncoderValue();
   snprintf(messageBuffer, sizeof(messageBuffer), "%u", sensorValue); // Convert to string
 
   mqtt.publish(VALUE_PUBLISH_TOPIC, messageBuffer, sizeof(messageBuffer), true);
 
   Serial.println("ESP32 - sent to MQTT:");
   Serial.println(messageBuffer);
 }
 
 void messageHandler(String &topic, String &payload) {
   Serial.println("ESP32 - received from MQTT:");
   Serial.println("- topic: " + topic);
   Serial.println("- payload:");
   Serial.println(payload);
 }