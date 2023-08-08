#include <CO2Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ThingSpeak.h"

CO2Sensor co2Sensor(4, 0.99, 100);

String URL = "http://172.20.10.2/FYP_project/CO2.php";

const char* ssid = "Dan";
const char* password = "abcabc123";

WiFiServer server(80);

WiFiClient client;

//-----ThingSpeak channel details

unsigned long myChannelNumber = 3;

const char* myWriteAPIKey = "QWYVT8CC71ELDUNG";

//----- Timer variables

unsigned long lastTime = 0;

unsigned long timerDelay = 1000;

int co2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  connectWiFi();
  co2Sensor.calibrate();

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  delay(15000);

  Load_CO2_Data();
  ThingSpeak.setField(3, co2);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  }

  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  lastTime = millis();
  String postData = "CO2=" + String(co2);

  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("URL : ");
  Serial.println(URL);
  Serial.print("Data: ");
  Serial.println(postData);
  Serial.print("httpCode ");
  Serial.println(httpCode);
  Serial.print("payload : ");
  Serial.println(payload);
  Serial.println("----------------------------------------------------");
  delay(5000);
}
void Load_CO2_Data() {
  //-----------------------------------------------------------
  co2 = co2Sensor.read();
  //-----------------------------------------------------------
  // Check if any reads failed.
  if (isnan(co2)) {
    Serial.println("Failed to read from CO2 sensor!");
    co2 = 0;
  }
  //-----------------------------------------------------------
  co2 = co2Sensor.read();
  Serial.printf("CO2: %d ppm\n", co2);
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}