// done:
// uid can be read and sent (serial print)


//credit:
// functions from the rfid library were edited and added into this scripts
// mfrc522 library was also used


#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 8
#define R_LED_PIN 12
#define G_LED_PIN 11

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
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);

  //Setup serial keys
  //serial read doesnt take integers, how much does it read then?
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

  Serial.println(uid);
  tone(BUZZER_PIN, 1000);
  delay(100);
  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);

/*
// if authorization checks out
  digitalWrite(G_LED_PIN, HIGH);
  delay(300);
  digitalWrite(G_LED_PIN, LOW);

//if authorization does not check out
  digitalWrite(R_LED_PIN, HIGH);
  delay(300);
  digitalWrite(R_LED_PIN, LOW);
*/

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
