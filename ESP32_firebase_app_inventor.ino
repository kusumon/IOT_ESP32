#include <DHT.h>
#include <DHT_U.h>

#include <FirebaseESP32.h>
#include <WiFi.h>

#define WIFI_SSID "Novi" // Change the name of your WIFI
#define WIFI_PASSWORD "kusumon97" // Change the password of your WIFI

#define FIREBASE_HOST "esp32-sensor-reading-e3d4c-default-rtdb.firebaseio.com" // Database URL
#define FIREBASE_Authorization_key "qnmbJTqKH9DlESRKEw4aqNsh58QfCnAop7TLZ0LF" // Database Secrets

#define DHTPIN 4    

#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

FirebaseData firebaseData;
FirebaseJson json;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);
}

void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();  
  
  // check sensor availability
  if (isnan(hum) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C");
  Serial.print(" Humidity: ");
  Serial.print(hum);
  Serial.print("%");
  Serial.println();

  // sending data to firebase
  Firebase.setFloat(firebaseData, "/ESP32_APP/TEMPERATURE", temp); //jika tipe data integer ubah setFloat menjadi setInt
  Firebase.setFloat(firebaseData, "/ESP32_APP/HUMIDITY", hum); //jika tipe data integer ubah setFloat menjadi setInt
  delay(200);
}
