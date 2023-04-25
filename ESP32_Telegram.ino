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

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Set the GPIO you want to control. In our case, we’ll control GPIO 2 (built-in LED) and its state is LOW by default
const int ledPin = 2;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  
  // It checks the available messages
  for (int i=0; i<numNewMessages; i++) {
    /* Get the chat ID for that particular message and store it in the chat_id variable.
    The chat ID allows us to identify who sent the message.*/
    String chat_id = String(bot.messages[i].chat_id);
    
    /* If the chat_id is different from your chat ID (CHAT_ID),
    it means that someone (that is not you) has sent a message to your bot.
    If that’s the case, ignore the message and wait for the next message.*/
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    /* Otherwise, it means that the message was sent from a valid user,
    so we’ll save it in the text variable and check its content. */

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    // The from_name variable saves the name of the sender
    String from_name = bot.messages[i].from_name;

    /* If it receives the /start message, we’ll send the valid commands to control the ESP32/ESP8266.
    This is useful if you happen to forget what are the commands to control your board.*/
    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    /* If it receives the /led_on message,
    turn the LED on and send a message confirming we’ve received the message.
    Also, update the ledState variable with the new state.*/
    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    /*Finally, if the received message is /state, check the current GPIO state and send a message accordingly.*/
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  // Set the LED as an output and set it to LOW when the ESP first starts:
  digitalWrite(ledPin, ledState);
  
  // Initialize Wi-Fi and connect the ESP to your local network with the SSID and password defined earlier.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  // check for new messages every second.
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    // When a new message arrives, call the handleNewMessages function.
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
