#include <NewPing.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>
#include <WiFiClientSecure.h> 
#include <UniversalTelegramBot.h>

#define trigPin D6
#define echoPin D5
#define FOR_LIGHT 50
#define buzzerPin D7 
#define redLedPin D7
#define greenLedPin D6
#define SOUND_SPEED 34300 
#define MIN_SPEED_THRESHOLD 10
#define DISTANCE_THRESHOLD 500
#define MIN_DISTANCE_THRESHOLD 500 

// ThingSpeak Channel
unsigned long channelID = 2526286;
const char *writeAPIKey = "W7CXCNXXADZ8MSL5";

// WiFi
const char* ssid = "Amai Mask";
const char* password = "01554333427";

//Telegram Bot
#define CHAT_ID "6960041740" 
#define BOTtoken "7181653196:AAEl8JBZKPTkIWCl1MoEuS3ldaenYjillNw"

WiFiClient client;
WiFiClientSecure clnt;
NewPing sonar(trigPin, echoPin);
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(BOTtoken, clnt);
LiquidCrystal lcd(D0, D1, D2, D3, D4, D8); 

float previousDistance = 0;
unsigned long previousTime = 0;

void setup() 
{
  Serial.begin(9600);

  lcd.begin(16, 2);

  configTime(0, 0, "pool.ntp.org");      
  clnt.setTrustAnchors(&cert);

  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  digitalWrite(redLedPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(greenLedPin, HIGH);

  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(50);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");
  ThingSpeak.begin(client);
  delay(50);
}

void loop() 
{
  unsigned long currentTime = millis();
  float distance = sonar.ping_cm();

  if (distance < DISTANCE_THRESHOLD && distance > 0) 
  {
    if (previousDistance == 0) 
    {
      previousDistance = distance;
      previousTime = currentTime;
    } 
    else 
    {
      unsigned long elapsedTime = currentTime - previousTime;
      float speed = abs(previousDistance - distance) / (float)elapsedTime;
      speed *= 2000;

      if (speed > MIN_SPEED_THRESHOLD) 
      {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
        Serial.print("Speed: ");
        Serial.print(speed);
        Serial.println(" cm/s");
      }

      if (distance < MIN_DISTANCE_THRESHOLD && speed >= FOR_LIGHT) 
      {
        digitalWrite(redLedPin, HIGH);
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(greenLedPin, LOW);
        delay(100);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Overspeed:");
        lcd.setCursor(0, 1);
        lcd.print(speed);
        lcd.print(" cm/s");
        bot.sendMessage(CHAT_ID, "Red light is on!\nSpeed: " + String(speed) + " cm/s\nDistance: " + String(distance) + " cm", "");
      } 
      else 
      {
        digitalWrite(redLedPin, LOW);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(greenLedPin, HIGH);
      }
      
      ThingSpeak.writeField(channelID, 1, distance, writeAPIKey);
      ThingSpeak.writeField(channelID, 2, speed, writeAPIKey);

      previousDistance = distance;
      previousTime = currentTime;
    }
  } 
  else 
  {
    lcd.clear();
    previousDistance = 0;
    digitalWrite(redLedPin, LOW); 
    digitalWrite(buzzerPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }
  delay(10);
}

