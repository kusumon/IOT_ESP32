/*Mengimpor beberapa library yang akan digunakan pada program ini,
yaitu WiFiClientSecure untuk mengatur koneksi ke server Telegram,
UniversalTelegramBot untuk membuat bot, dan ArduinoJson untuk mengolah data JSON.*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// Initialize Telegram BOT input your Bot Token (Get from Botfather)
#define BOTtoken "TOKEN YANG DIDAPATKAN DARI TELEGRAM BOTFATHER"

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "ID TELEGRAM YANG DIDAPATKAN DARI TELEGRAM IDBot"

// Create a new WiFi client with WiFiClientSecure
WiFiClientSecure client;

// Create a bot with the token and client defined earlier
UniversalTelegramBot bot(BOTtoken, client);

int ledPin = 2; // pin for the LED
const int motionSensor = 27; // Define the GPIO that PIR the motion sensor is connected to.
/* The motionDetected boolean variable is used to indicate whether motion was detected or not.
It is set to false by default. */
bool motionDetected = false;

// Indicates when motion is detected
/* The detectsmovement() function is a callback function that will be executed when motion is detected.
In this case, it simply changes the state of the motionDetected variable to true. */
void IRAM_ATTR detectsMovement() {
  motionDetected = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  // PIR Motion Sensor mode INPUT
  pinMode(motionSensor, INPUT);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Initialize Wi-Fi and connect the ESP to your local network with the SSID and password defined earlier.
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // send a message to indicate that the Bot has started up
  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

//In the loop(), check the state of the motionDetected variable.
void loop() {
/* If it’s true, it means that motion was detected.
So, send a message to your Telegram account indicating that motion was detected. */
  if(motionDetected){
    // Sending a message to the bot
    bot.sendMessage(CHAT_ID, "Motion detected!!", "");
    Serial.println("Motion Detected");
    // Finally, after sending the message, set the motionDetected variable to false, so it can detect motion again.
    motionDetected = false;
    digitalWrite(ledPin, LOW);
  }
}
