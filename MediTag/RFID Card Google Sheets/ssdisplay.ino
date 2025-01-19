#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// Pins for RFID module
#define RST_PIN D3
#define SS_PIN  D4
#define BUZZER_PIN D8  // Define buzzer pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Wi-Fi credentials
#define WIFI_SSID "Rajlakshmi"      // Replace with your Wi-Fi SSID
#define WIFI_PASSWORD "pune2005"    // Replace with your Wi-Fi password

// Google Script Web App URL (base part only)
const String baseURL = "https://script.google.com/macros/s/AKfycbwPKTx8Qjz84z3mIe5A7aAnCZH2rYmBpG6gYngr_Xns6t6UcldhaUdHSwrfe1FqtrmwKQ/exec";

// Function declarations
String readRFIDData(int startBlock, int numBlocks);
String urlencode(String str);
void connectToWiFi();
void ringBuzzer();

unsigned long buzzerStartTime = 0;
bool buzzerActive = false;

void setup() {
  Serial.begin(9600); // Initialize serial communications with PC
  SPI.begin();        // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522 Module

  pinMode(BUZZER_PIN, OUTPUT); // Initialize buzzer pin as output

  connectToWiFi(); // Connect to Wi-Fi
  Serial.println("Scan an RFID card or NFC tag...");
}

void loop() {
  // Reconnect to Wi-Fi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // Check for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Activate the buzzer for 0.5 seconds
    ringBuzzer();

    // Debug output for tag type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.print("Card type: ");
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Reading data from the tag (READ BLOCKS IN SEQUENCE)
    String medicine = readRFIDData(4, 2);  // Reading Medicine Name from Blocks 4 and 5 (8 bytes total)
    String email = readRFIDData(6, 8);     // Reading Email from Blocks 6 to 10 (20 bytes total)

    Serial.print("Blocks 4-5 (Medicine Name): ");
    Serial.println(medicine); // Debug output to check data from Blocks 4-5

    Serial.print("Blocks 6-10 (Email): ");
    Serial.println(email); // Debug output to check data from Blocks 6-10

    if (medicine != "" && email != "") {
      Serial.println("Medicine Name: " + medicine);
      Serial.println("Email: " + email);

      // Send the data to Google Sheets
      if (WiFi.status() == WL_CONNECTED) {
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure(); // Disable SSL certificate check

        HTTPClient https;

        // URL encode the parameters to handle special characters in email, name, etc.
        String url = baseURL + "?medicine=" + urlencode(medicine) + "&email=" + urlencode(email);

        Serial.print("Requesting URL: ");
        Serial.println(url); // Debug: Print the URL to check its correctness

        if (https.begin(*client, url)) {
          int httpCode = https.GET();
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

          // Handling HTTP response codes
          if (httpCode == HTTP_CODE_OK) {
            String payload = https.getString();
            Serial.println("Server Response: " + payload);
          } else if (httpCode == HTTP_CODE_FOUND) {
            // Redirection handling
            String newLocation = https.header("Location");
            Serial.println("Redirection detected to: " + newLocation);
            if (https.begin(*client, newLocation)) {
              int redirectHttpCode = https.GET();
              Serial.printf("[HTTPS] Redirect GET... code: %d\n", redirectHttpCode);
              if (redirectHttpCode == HTTP_CODE_OK) {
                String payload = https.getString();
                Serial.println("Server Response after redirection: " + payload);
              }
              https.end();
            }
          } else {
            Serial.printf("Error in HTTP request: %d\n", httpCode);
          }
          https.end();  // End the HTTP request
        } else {
          Serial.println("Failed to begin HTTP connection");
        }
      } else {
        Serial.println("Wi-Fi is not connected. Unable to send data.");
      }
    }

    // Halt PICC
    mfrc522.PICC_HaltA();
  }

  // Manage buzzer state
  if (buzzerActive && (millis() - buzzerStartTime >= 500)) { // 500 ms for 0.5 seconds
    digitalWrite(BUZZER_PIN, LOW); // Turn buzzer off after 0.5 seconds
    buzzerActive = false;
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

String readRFIDData(int startBlock, int numBlocks) {
  String data = "";
  byte buffer[18]; // 16 bytes buffer for MIFARE cards, for NFC Type 2 adjust accordingly
  byte size = sizeof(buffer);

  for (int block = startBlock; block < startBlock + numBlocks; block++) {
    MFRC522::StatusCode status = mfrc522.MIFARE_Read(block, buffer, &size);
    if (status == MFRC522::STATUS_OK) {
      for (byte i = 0; i < 4; i++) {
        if (buffer[i] != 0) { // Ignore null characters
          data += (char)buffer[i];
        }
      }
    } else {
      Serial.print("Reading Block failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return "";
    }
  }
  return data;
}


String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}

void ringBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer on
  buzzerStartTime = millis();
  buzzerActive = true;
}

