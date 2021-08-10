
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SerialCom.h"
#include "Types.h"

// Config.h is not used at the moment but included for future development
// #include "Config.h"

particleSensorState_t state;

// Update these with values suitable for your network
const char* ssid = "Your WLan ssid";
const char* password = "Your WPA key";
const char* mqtt_server = "Your MQTT server ip address only";

// MQTT topic and message string array
const char* topic = "domoticz/in";
char msg[50];

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float statePM25 = 0;

void setup_wifi() {
  
  // First we set the ESP in STATION mode, effectively disabling AP (MIXED) mode  
  WiFi.mode(WIFI_STA); 
  delay(10);
  // Let's connect to a WiFi network
  Serial.println();
  Serial.print("Connecting to wireless network:");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
    
  while ( WiFi.status() != WL_CONNECTED ) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected successfully!");
  Serial.println("Obtained IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to MQTT server...");
    // Attempting to connect to MQTT server using a unique client ID.
    // If you use multiple sensors of the same type, make sure you use
    // a unique client ID for each sensor e.g. _idx_850
    if (client.connect("arduinoClient_air_quality_sensor_idx_850")) {
      Serial.print("connected\n");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

    Serial.begin(115200);
    setup_wifi();
    SerialCom::setup();
    client.setServer(mqtt_server, 1883);

    Serial.println("\n");
    Serial.println("Hello from IKEA VINDRIKTNING particle sensor");
    Serial.printf("Core Version: %s\n", ESP.getCoreVersion().c_str());
    Serial.printf("Boot Version: %u\n", ESP.getBootVersion());
    Serial.printf("Boot Mode: %u\n", ESP.getBootMode());
    Serial.printf("CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
    
    delay(3000);
}

void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  SerialCom::handleUart(state);
  client.loop();

  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    //Get average value from sensor as calculated by SerialCom.h
    statePM25 = state.avgPM25;
    // Build MQTT message payload. Change idx value e.g. 850, to 
    // match the idx of the virtual device in Domoticz
    String dataMsg = "{\"idx\":850,";
    dataMsg.concat("\"nvalue\":0,");
    dataMsg.concat("\"svalue\":\"");
    dataMsg.concat(statePM25);
    dataMsg.concat("\"}");
    dataMsg.toCharArray(msg, dataMsg.length()+1);
   
    //Publish payload to MQTT broker
    client.publish(topic, msg);     
    Serial.print("MQTT payload: ");
    Serial.println(dataMsg); 
  }
}
