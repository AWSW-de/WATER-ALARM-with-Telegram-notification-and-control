// ###############################################################################################################################
// #
// # Code for "WATER ALARM with Telegram notification and control" project: https://www.printables.com/de/model/243382-water-alarm-with-telegram-notification-and-control
// #
// # Code by https://github.com/AWSW-de 
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/WATER-ALARM-with-Telegram-notification-and-control/blob/main/LICENSE
// #
// ###############################################################################################################################

// ###############################################################################################################################
// #
// # Code version:
// #
// ###############################################################################################################################
String codeversion = "1.2.4";
// #######################################################################################################################################################
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>            // https://github.com/arduino-libraries/Arduino_JSON
#include "settings.h"
// #######################################################################################################################################################
// General program settings:
String programname = "AWSW Water Alarm";
// #######################################################################################################################################################
// WiFi, Telegram and other settings:
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
bool alarmstatus = false;
// #######################################################################################################################################################
// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  // Serial.println("handleNewMessages");
  // Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester and block commands from an unauthorized user:
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    // Show the commands list:
    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "use the following commands: \n\n";
      welcome += "Use /start to show this text \n";
      welcome += "Use /testalarm to perform a test alarm \n";
      welcome += "Use /stopalarm to stop the alarm \n\n";
      welcome += "Just type any other text to test the communication\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    // Reply last send unknown command message:
    if ((text != "/start") && (text != "/testalarm") && (text != "/stopalarm")) {
      bot.sendMessage(CHAT_ID, "No valid command/text received:\n" + text, "");
    }

    // Perform a test alarm:
    if (text == "/testalarm") {
      bot.sendMessage(CHAT_ID, "Test alarm initiated. \n Use /stopalarm to end this test.", "");
      alarmstatus = true;
    }

    // Stop the alarm status:
    if (text == "/stopalarm") {
      bot.sendMessage(CHAT_ID, "Alarm stopped", "");
      alarmstatus = false;
    }

  }
}
// #######################################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("######################################");
  Serial.println("# Starting: " + programname + " - " + codeversion + " #");
  Serial.println("######################################");
  Serial.println(" ");

  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);

  Serial.print("Connecting to Wifi: ");
  Serial.println(ssid);
  WiFi.disconnect();
  delay(150);
  WiFi.setHostname(wifiHostname);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(".");
  Serial.print("Connected to Wifi: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(wifiHostname);

  // Initialize pins:
  pinMode(buzzerPin, OUTPUT);
  pinMode(contactPin, INPUT_PULLUP);

  // Send ESP online Telegram message:
  bot.sendMessage(CHAT_ID, programname + " - " + codeversion + " online", "");
  Serial.println(programname + " - " + codeversion + " online :)");
}
// #######################################################################################################################################################
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response: ");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // Check for water contact:
  int contactValue = digitalRead(contactPin);
  if (contactValue == 0) {
    alarmstatus = true;
  }

  // Set alarm:
  if (alarmstatus == true) {
    alarm();
  }

  // Wifi reconnect needed:
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Currently NOT connected to Wifi: ");
    Serial.println(ssid);
    WiFi.disconnect();
    delay(150);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println(".");
    Serial.print("Connected to Wifi: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Hostname: ");
    Serial.println(wifiHostname);
  }
}
// #######################################################################################################################################################
void alarm() {
  tone(buzzerPin, 1200, 250);
  delay(250);
  tone(buzzerPin, 800, 250);
  delay(250);
  tone(buzzerPin, 4000, 250);
  bot.sendMessage(CHAT_ID, "ALARM - " + programname + " - ALARM", "");
  Serial.println("ALARM - " + programname + " - ALARM");
}
// #######################################################################################################################################################
