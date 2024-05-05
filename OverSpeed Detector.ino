#include <NewPing.h>
#include <ThingSpeak.h> // Include the ThingSpeak library
#include <ESP8266WiFi.h>
#include <LiquidCrystal.h> // Include the LiquidCrystal library
#include <WiFiClientSecure.h> 
#include <UniversalTelegramBot.h>

#define trigPin D6
#define echoPin D5
#define redLedPin D7
#define greenLedPin D6
#define buzzerPin D7 // Define buzzer pin
#define SOUND_SPEED 34300 // Speed of sound in cm/s at 20 degrees Celsius
#define DISTANCE_THRESHOLD 500 // Threshold distance in centimeters to trigger speed calculation (adjust as needed)
#define MIN_SPEED_THRESHOLD 10 // Minimum speed threshold in cm/s
#define FOR_LIGHT 50 // Minimum speed threshold in cm/s
#define MIN_DISTANCE_THRESHOLD 500 // Minimum distance threshold in centimeters to trigger red LED

NewPing sonar(trigPin, echoPin);

// ThingSpeak Channel Settings
unsigned long channelID = 2526286; // Replace with your ThingSpeak channel ID
const char *writeAPIKey = "W7CXCNXXADZ8MSL5"; // Replace with your ThingSpeak write API key

const char* ssid = "Amai Mask";     // Your WiFi SSID
const char* password = "01554333427"; // Your WiFi password

// const char *host = "api.telegram.org";
// const int httpsPort = 443;
// const char *botToken = "7181653196:AAEl8JBZKPTkIWCl1MoEuS3ldaenYjillNw";
// const char *chatID = "7181653196";
WiFiClient client; // Create a WiFi client object

#define BOTtoken "7181653196:AAEl8JBZKPTkIWCl1MoEuS3ldaenYjillNw" // Enter the bottoken you got from botfather
#define CHAT_ID "6960041740" // Enter your chatID you got from chatid bot


X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure clnt;
UniversalTelegramBot bot(BOTtoken, clnt);


LiquidCrystal lcd(D0, D1, D2, D3, D4, D8); // Define LCD pins

unsigned long previousTime = 0;
float previousDistance = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  configTime(0, 0, "pool.ntp.org");      
  clnt.setTrustAnchors(&cert);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);  // Set red LED pin as output
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH); // Ensure red LED is initially off
  digitalWrite(buzzerPin, LOW); // Ensure buzzer is initially off
  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");
  ThingSpeak.begin(client); // Initialize ThingSpeak client with the WiFi client
  delay(50);
}

void loop() {
  unsigned long currentTime = millis();
  float distance = sonar.ping_cm(); // Distance in centimeters

  if (distance < DISTANCE_THRESHOLD && distance > 0) {
    if (previousDistance == 0) {
      // First measurement
      previousDistance = distance;
      previousTime = currentTime;
    } else {
      // Calculate speed
      unsigned long elapsedTime = currentTime - previousTime;
      float speed = abs(previousDistance - distance) / (float)elapsedTime; // Speed in cm/s
      speed *= 2000; // Convert speed to more readable units
      // Check if speed exceeds minimum threshold
      if (speed > MIN_SPEED_THRESHOLD) {
        // Print overspeed and speed to LCD
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
        Serial.print("Speed: ");
        Serial.print(speed);
        Serial.println(" cm/s");
      }

      // Check if distance is less than minimum threshold and speed is greater than or equal to the threshold for turning on the light
      if (distance < MIN_DISTANCE_THRESHOLD && speed >= FOR_LIGHT) {
        // Turn on red LED
        digitalWrite(redLedPin, HIGH);
        // Turn on buzzer
        digitalWrite(greenLedPin, LOW);
        delay(100);
        digitalWrite(buzzerPin, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Overspeed:");
        lcd.setCursor(0, 1);
        lcd.print(speed);
        lcd.print(" cm/s");
        bot.sendMessage(CHAT_ID, "Red light is on!\nSpeed: " + String(speed) + " cm/s\nDistance: " + String(distance) + " cm", "");


      } else {
        // Turn off red LED
        digitalWrite(redLedPin, LOW);
        // Turn off buzzer
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(buzzerPin, LOW);
      }
      
      // Send speed and distance data to ThingSpeak
      ThingSpeak.writeField(channelID, 1, distance, writeAPIKey);
      ThingSpeak.writeField(channelID, 2, speed, writeAPIKey);

      // Update previous values
      previousDistance = distance;
      previousTime = currentTime;
    }
  } else {
    // Reset previous values if the car is too far away or if the distance is invalid
    previousDistance = 0;
    digitalWrite(redLedPin, LOW); // Turn off red LED if the car is too far away
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(buzzerPin, LOW); // Turn off buzzer if the car is too far away
    lcd.clear(); // Clear LCD display
  }

  delay(10); // Adjust as needed depending on the expected speed of the car
}

