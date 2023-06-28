//This program monitors light while giving a time stamp and recording the data
//to an SD card

#include <Adafruit_AS7341.h>
#include <Adafruit_AW9523.h>
#include <Wire.h>
#include "RTClibrary.h"
#include <SdCardLogHandlerRK.h>
#include <SdFat.h>

Adafruit_AS7341 as7341;
Adafruit_AW9523 aw;
RTC_PCF8523 rtc;
File myFile;
SdFat SD;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int SD_CHIP_SELECT = D5;
uint8_t LedPin = 0;

SdCardPrintHandler printToCard(SD, SD_CHIP_SELECT, SPI_FULL_SPEED);

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);

  // Wait for communication with the host computer serial monitor
  while (!Serial) {
    delay(1);
  }
  
  if (!as7341.begin()){
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }
  
  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

    if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

    if (! aw.begin(0x58)) {
    Serial.println("AW9523 not found? Check wiring!");
    while (1) delay(10);  // halt forever
  }

  Serial.println("AW9523 found!");
  aw.pinMode(LedPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();



//save timestamp to sd card
    printToCard.print(now.year(), DEC);
    printToCard.print('/');
    printToCard.print(now.month(), DEC);
    printToCard.print('/');
    printToCard.print(now.day(), DEC);
    printToCard.print(" (");
    printToCard.print(daysOfTheWeek[now.dayOfTheWeek()]);
    printToCard.print(") ");
    printToCard.print(now.hour(), DEC);
    printToCard.print(':');
    printToCard.print(now.minute(), DEC);
    printToCard.print(':');
    printToCard.print(now.second(), DEC);
    printToCard.println();


  uint16_t readings[12];
  float counts[12];

  if (!as7341.readAllChannels(readings)){
    Serial.println("Error reading all channels!");
    return;
  }

  for(uint8_t i = 0; i < 12; i++) {
    if(i == 4 || i == 5) continue;
    // we skip the first set of duplicate clear/NIR readings
    // (indices 4 and 5)
    counts[i] = as7341.toBasicCounts(readings[i]);
  }

  aw.digitalWrite(LedPin, HIGH);
  Serial.println("LED ON");
  Serial.println();
  printToCard.println("LED ON");
  printToCard.println();
  delay(100);

  Serial.print("F1 415nm : ");
  Serial.println(counts[0]);
  Serial.print("F2 445nm : ");
  Serial.println(counts[1]);
  Serial.print("F3 480nm : ");
  Serial.println(counts[2]);
  Serial.print("F4 515nm : ");
  Serial.println(counts[3]);
  Serial.print("F5 555nm : ");
  // again, we skip the duplicates  
  Serial.println(counts[6]);
  Serial.print("F6 590nm : ");
  Serial.println(counts[7]);
  Serial.print("F7 630nm : ");
  Serial.println(counts[8]);
  Serial.print("F8 680nm : ");
  Serial.println(counts[9]);
  Serial.print("Clear    : ");
  Serial.println(counts[10]);
  Serial.print("NIR      : ");
  Serial.println(counts[11]);

  Serial.println();
  
//save data to SD card

  printToCard.print("F1 415nm : ");
  printToCard.println(counts[0]);
  printToCard.print("F2 445nm : ");
  printToCard.println(counts[1]);
  printToCard.print("F3 480nm : ");
  printToCard.println(counts[2]);
  printToCard.print("F4 515nm : ");
  printToCard.println(counts[3]);
  printToCard.print("F5 555nm : ");
  // again, we skip the duplicates  
  printToCard.println(counts[6]);
  printToCard.print("F6 590nm : ");
  printToCard.println(counts[7]);
  printToCard.print("F7 630nm : ");
  printToCard.println(counts[8]);
  printToCard.print("F8 680nm : ");
  printToCard.println(counts[9]);
  printToCard.print("Clear    : ");
  printToCard.println(counts[10]);
  printToCard.print("NIR      : ");
  printToCard.println(counts[11]);
  printToCard.println();
  
  delay(500);

  aw.digitalWrite(LedPin, LOW);
  Serial.println("LED OFF");
  Serial.println();
  printToCard.println("LED OFF");
  printToCard.println();

  delay(100);
}
