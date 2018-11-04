/********************************************************************/
// First we include the libraries
#include <WiFi.h>
#include <PubSubClient.h>
//#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//********************************************************************/
// Data wire is plugged into pin 15 on the Arduino
#define ONE_WIRE_BUS 15  //data wire connected to GPIO15
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Replace the next variables with your SSID/Password combination
const char* ssid = "Reef19";
const char* password = "fl70an73ma03da05";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.2.6";
const char* mqtt_server = "192.168.2.6";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
String Sensor_ID = "Woonkamer1";
String JSON_Message;




void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
   if (!client.connected()) {
     reconnect();
   }
   client.loop();

   long now = millis();
   if (now - lastMsg > 60000) 
   {
     lastMsg = now;

    
     // call sensors.requestTemperatures() to issue a global temperature
     // request to all devices on the bus
     /********************************************************************/
     sensors.requestTemperatures(); // Send the command to get temperature readings
     /********************************************************************/
     temperature =  sensors.getTempCByIndex(0);    

     JSON_Message="";
     JSON_Message += F("\{\"Sensor_ID\":\"");
     JSON_Message += Sensor_ID;
     JSON_Message += F("\",\"Temperature\":\"");
     JSON_Message += String(temperature,2);
     JSON_Message += F("\"\}");
     int  JSON_Message_Length = JSON_Message.length() + 1;
     char JSON_Message_Char[JSON_Message_Length];
     JSON_Message.toCharArray(JSON_Message_Char, JSON_Message_Length);
     Serial.println(JSON_Message_Char); 
     client.publish("Home/Temperature", JSON_Message_Char);
  }
}
