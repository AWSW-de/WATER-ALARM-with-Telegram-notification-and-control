// #######################################################################################################################################################

// Wifi settings - replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* wifiHostname = "AWSW-Water-Alarm";

// #######################################################################################################################################################

// Buzzer pin: 
#define buzzerPin D5

// Contact pin:
#define contactPin D2

// #######################################################################################################################################################

// Telegram new BOT creation:
// In Telegram search user "BotFather" and use the "Start" button
// Then type: "/newbot" 
// Set a name for the new bot when requested
// Set a username for the new bot when requested
// Check the "Done! Congratulations" message for the line after "Use this token to access the HTTP API:" and add this line to the setting above here:
#define BOTtoken "XXXXXXXXXX:YYYYYYYYYYYYYYY-ZZZZZZZZZZZZZZZZZZZZ"

// #######################################################################################################################################################

// Your chat id for your own Telegram messanger
// In Telegram search user "IDBot" and use the "Start" button
// Then type: "/getid"
// Copy your own Telegram ID and add this number to the setting above here:
#define CHAT_ID "1234512345"

// #######################################################################################################################################################

// If you like to have a menu with all the available commands just edit the created bot using BotFather again:
// - In Telegram search user "BotFather" and use the "Start" button
// - /mybots
// - Select your bot
// - Select "Edit bot"
// - Select "Edit Commands"
// - Follow the instruction to set up the menu entries - example (send without the "//"):
// start - to show this text
// testalarm - to perform a test alarm
// stopalarm - to stop the alarm

// #######################################################################################################################################################
