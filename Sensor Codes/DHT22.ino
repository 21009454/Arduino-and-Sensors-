#include<WiFi.h>
#include<HTTPClient.h>

#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht22(DHTPIN, DHTTYPE);

String URL = "http://10.175.9.92/DHT22_project/test_data.php";

const char* ssid = "Dan";
const char* password = "abcabc123";

int temperature = 0;
int humidity = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  dht22.begin();

  connectWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
  Load_DHT22_Data();
  String postData = "temperature" + String(temperature) + "&humidity=" + String(humidity);

  HTTPClient http;
  http.begin(URL);

  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("URL : "); Serial.println(URL);
  Serial.print("Data: "); Serial.println(postData);
  Serial.print("httpCode "); Serial.println(httpCode);
  Serial.print("payload : "); Serial.println(payload);
  Serial.println("----------------------------------------------------");
  delay(1500);

}
void Load_DHT22_Data() {
  //-----------------------------------------------------------
  temperature = dht22.readTemperature(); //Celsius
  humidity = dht22.readHumidity();
  //-----------------------------------------------------------
  // Check if any reads failed.
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0;
    humidity = 0;
  }
  //-----------------------------------------------------------
  Serial.printf("Temperature: %d °C\n", temperature);
  Serial.printf("Humidity: %d %%\n", humidity);
}

void connectWiFi(){
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
  
    Serial.print("connected to : "); Serial.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());

}
