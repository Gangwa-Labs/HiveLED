#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "glenmore";
const char* password = "3013651953";
// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.1.177";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient hexControllerClient;
PubSubClient client(hexControllerClient);
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("hexControllerClient")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("HiveSwitch");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  int magicNumber;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Wire.beginTransmission(8);
  magicNumber = messageTemp.toInt(); //num value of incoming topic message
  Serial.println();
  if (magicNumber == 1) { 
    Serial.println("on");
    byte messageSend[] = {1,0,0,2,4};
    for(int i = 0; i < sizeof(messageSend); i++){
     Wire.write(messageSend[i]); 
    }
  }
  if (magicNumber == 0) {
    Serial.println("off");
    byte messageSend[] = {5,23,5,7};
    for(int i = 0; i < sizeof(messageSend); i++){
     Wire.write(messageSend[i]); 
    }
  }
  Wire.endTransmission();
  Serial.println();
}

void setup() {
  Serial.begin(9600); /* begin serial for debug */
  Wire.begin(D2, D1); /* join i2c bus with SDA=D2 and SCL=D1 of NodeMCU */
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
