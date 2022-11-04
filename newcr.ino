// done:
// uid can be read and sent (serial print)


// current issues:
// contents cannot be read (every block returns 5)
// or writen (for increment)
// rsa dont work


#include <SPI.h>
#include <MFRC522.h>
#include <rsa.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 8
const unsigned int UID_SIZE = 4;
MFRC522 mfrc522(SS_PIN, RST_PIN );

unsigned long long pyn;
struct {
  unsigned long long d;
  unsigned long long n;
} privkey;

//make code to send reader id
// but comment it out as it is a prototype

void setup() {  
  // put your setup code here, to run once:
  Serial.begin(230400);
  //mfrc522.PCD_Init(); //Init RFID reader
  pinMode(BUZZER_PIN, OUTPUT);

  //Setup serial keys
  //serial read doesnt take integers, how much does it read then?
  bool sloop = true;
  while(Serial.available() <= 0) ;
  char buff[64];
  Serial.readBytes(buff, sizeof(buff));
  pyn = atoi(buff);
  memset(buff, 0, sizeof(buff));

  while(Serial.available() <= 0) ;
  Serial.readBytes(buff, sizeof(buff));
  privkey.d = atoi(buff);
  memset(buff, 0, sizeof(buff));

  while(Serial.available() <= 0) ;
  Serial.readBytes(buff, sizeof(buff));
  privkey.n = atoi(buff);
  memset(buff, 0, sizeof(buff));
  
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  unsigned int uid = mfrc522.uid.uidByte[0];

  for(int i=1; i<UID_SIZE; i++) {
    uid = uid << 8;
    uid |= mfrc522.uid.uidByte[i];
  }
  
  mfrc522.PICC_HaltA();

  // ISSUE: always returns 5
  //byte readbyte = mfrc522.MIFARE_Read(1, &buffRead, 128);
  Serial.println(uid);
  tone(BUZZER_PIN, 1000);
  delay(100);
  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);

  char buff[64];
  snprintf(buff, 64, "%lu", pyn);
  Serial.println(buff);
  snprintf(buff, 64, "%lu", privkey.d);
  Serial.println(buff);
  snprintf(buff, 64, "%lu", privkey.n);
  Serial.println(buff);
  
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  //mfrc522.PICC_HaltA();
}
