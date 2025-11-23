#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>


// WiFi Credentials

const char* ssid = "Galaxy A13";
const char* password = "onetonine";


// ThingSpeak Details

String apiKey = "YOUR_THINGSPEAK_API_KEY";
const char* server = "http://api.thingspeak.com/update";


// Sensor Pins

int flamePin = 34;   // Analog input
int smokePin = 35;   // Analog input

// GPS Setup (NEO-6M)

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);   // UART1
int RXPin = 16;  // GPS TX → ESP32 RX
int TXPin = 17;  // GPS RX → ESP32 TX


// Threshold Values

int flameThreshold = 1500;
int smokeThreshold = 2000;

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Forest Fire Detection Drone Starting ---");

  SerialGPS.begin(9600, SERIAL_8N1, RXPin, TXPin);

  // WiFi Connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {

  // Read Sensors
  int flameValue = analogRead(flamePin);
  int smokeValue = analogRead(smokePin);

  // Decode GPS data
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  float latitude = gps.location.lat();
  float longitude = gps.location.lng();

  // Detection logic
  bool fireDetected = flameValue < flameThreshold;
  bool smokeDetected = smokeValue > smokeThreshold;

  
  // SERIAL MONITOR OUTPUT

 
  Serial.println(" 🔥 FOREST FIRE DRONE STATUS");
  Serial.println("--------------------------------");

  Serial.print("Flame Sensor Value : ");
  Serial.println(flameValue);

  Serial.print("Smoke Sensor Value : ");
  Serial.println(smokeValue);

  Serial.print("Fire Detected      : ");
  Serial.println(fireDetected ? "YES" : "NO");

  Serial.print("Smoke Detected     : ");
  Serial.println(smokeDetected ? "YES" : "NO");

  Serial.print("GPS Latitude       : ");
  Serial.println(gps.location.isValid() ? String(latitude, 6) : "No Fix");

  Serial.print("GPS Longitude      : ");
  Serial.println(gps.location.isValid() ? String(longitude, 6) : "No Fix");

  Serial.println("==============================\n");

  
  // SEND TO THINGSPEAK
 
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(flameValue);
    url += "&field2=" + String(smokeValue);
    url += "&field3=" + String(latitude, 6);
    url += "&field4=" + String(longitude, 6);
    url += "&field5=" + String(fireDetected);
    url += "&field6=" + String(smokeDetected);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("ThingSpeak Update Status: SUCCESS (" + String(httpCode) + ")");
    } else {
      Serial.println("ThingSpeak Update FAILED");
    }

    http.end();
  }

  delay(15000);  // ThingSpeak 15-sec rule
}
