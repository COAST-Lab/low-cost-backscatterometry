/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Russe/OneDrive/Desktop/Coastlab/Turbidity/src/Turbidity.ino"
//This program monitors light while giving a time stamp and recording the data
//to an SD card

#include <Adafruit_AS7341.h>
#include <Adafruit_AW9523.h>
//  #include <Wire.h>
#include "RTClibrary.h"
//  #include <SdCardLogHandlerRK.h>
#include <SdFat.h>

void setup();
void loop();
#line 11 "c:/Users/Russe/OneDrive/Desktop/Coastlab/Turbidity/src/Turbidity.ino"
Adafruit_AS7341 as7341;
Adafruit_AW9523 aw;
RTC_PCF8523 rtc;
File myFile;
SdFat SD;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int SD_CHIP_SELECT = D5;
uint8_t LedPin = 0;

//  SdCardPrintHandler printToCard(SD, SD_CHIP_SELECT, SPI_FULL_SPEED);

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

  SD.begin(SD_CHIP_SELECT);
 
  File file = SD.open("data.csv", FILE_WRITE);
  if (file) {
    file.println("LED Status,Time,F1 415nm,F2 445nm,F3 480nm,F4 515nm,F5 555nm,F6 590nm,F7 630nm,F8 680nm,Clear,NIR"); // Headers
    file.close();
  } else {
    Serial.println(F("Error opening file!"));
  }
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
  // printToCard.println("LED ON");
  // printToCard.println();
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

File file = SD.open("data.csv", FILE_WRITE);
  if (file) {
    //  print readings
     
    //file.println("LED Status,Time,F1 415nm,F2 445nm,F3 480nm,F4 515nm,F5 555nm,F6 590nm,F7 630nm,F8 680nm,Clear,NIR"); // Headers
    file.print("LED ON");
    file.print(',');
    file.print(now.year(), DEC);
    file.print('/');
    file.print(now.month(), DEC);
    file.print('/');
    file.print(now.day(), DEC);
    file.print(" (");
    file.print(daysOfTheWeek[now.dayOfTheWeek()]);
    file.print(") ");
    file.print(now.hour(), DEC);
    file.print(':');
    file.print(now.minute(), DEC);
    file.print(':');
    file.print(now.second(), DEC);
    file.print(',');
    file.print(counts[0]);
    file.print(',');
    file.print(counts[1]);
    file.print(',');
    file.print(counts[2]);
    file.print(',');
    file.print(counts[3]);
    file.print(',');
    file.print(counts[6]);
    file.print(',');
    file.print(counts[7]);
    file.print(',');
    file.print(counts[8]);
    file.print(',');
    file.print(counts[9]);
    file.print(',');
    file.print(counts[10]);
    file.print(',');
    file.print(counts[11]);
    file.print(',');

  aw.digitalWrite(LedPin, LOW);
  Serial.println("LED OFF");
  Serial.println();
file.println("LED OFF");
    file.close();

  } else {
    Serial.println(F("Error opening file!"));
  }


  
  delay(500);

  //aw.digitalWrite(LedPin, LOW);
  //Serial.println("LED OFF");
  //Serial.println();

  //File file = SD.open("data.csv", FILE_WRITE);
  //if (file) {
  //  file.println("LED OFF"); // Headers
  //  file.close();

  //} else {
  //  Serial.println(F("Error opening file!"));
  //}
  
  

  delay(100);
}