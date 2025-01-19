#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;  // RST pin on ESP8266
constexpr uint8_t SS_PIN = D4;   // SDA/SS pin on ESP8266

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create instance of the MFRC522 class

int blockNum = 4;                  // Starting block number to read/write
byte medicine[8] = {"Rabix"};       // 8 bytes allocated for medicine name
byte email[30] = {"rajlakshmidesai78@gmail.com"}; // 20 bytes allocated for email

void setup() {
  Serial.begin(115200);           // Initialize serial communications with PC
  SPI.begin();                    // Initialize SPI bus
  mfrc522.PCD_Init();             // Initialize MFRC522 Module
  Serial.println("Scan an RFID NFC Type 2 Tag to write data...");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println("\n*Card Detected*");

  // Clear existing data on the tag before writing new data
  Serial.println("Clearing data on the tag...");
  clearTagData(blockNum, 9);  // Clear 9 blocks starting from block 4 to block 12
  Serial.println("Data cleared successfully.");

  // Writing new data to the tag
  Serial.println("Writing data to the tag...");
  writeDataToTag(blockNum, medicine, sizeof(medicine));  // blockNum = 4
  writeDataToTag(blockNum + 2, email, sizeof(email)); // blockNum + 2 = 6

  // Halt the PICC
  mfrc522.PICC_HaltA();
}

void clearTagData(int startBlock, int numberOfBlocks) {
  for (int block = startBlock; block < startBlock + numberOfBlocks; block++) {
    byte blockData[4] = {0x00, 0x00, 0x00, 0x00}; // Clear block with zeros
    MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(block, blockData, 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Clearing Block failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
}

void writeDataToTag(int blockStart, byte *data, int length) {
  int blocksNeeded = (length + 3) / 4;  // Number of 4-byte blocks needed
  for (int i = 0; i < blocksNeeded; i++) {
    byte blockData[4] = {0};  // Initialize block data with zeros
    for (int j = 0; j < 4; j++) {
      int index = i * 4 + j;
      blockData[j] = (index < length) ? data[index] : 0;
    }
    MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(blockStart + i, blockData, 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Writing to Block failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
  Serial.println("Data written successfully");
}
