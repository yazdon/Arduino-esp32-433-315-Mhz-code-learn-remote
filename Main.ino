
#include <RCSwitch.h>
#include <EEPROM.h>

#define BUTTON1_PIN 21
#define BUTTON2_PIN 17
#define BUTTON3_PIN 16

long a, b, c;
RCSwitch mySwitch = RCSwitch();
bool learningMode = false;
unsigned long startLearningTime = 0;
void setup() {
  Serial.begin(9600);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);

  EEPROM.begin(512);
  mySwitch.enableTransmit(22);
  mySwitch.enableReceive(digitalPinToInterrupt(23));  // گیرنده روی GPIO 23
  // Read codes from EEPROM}
}
void loop() {
  if (digitalRead(BUTTON1_PIN) == LOW && digitalRead(BUTTON2_PIN) == LOW && digitalRead(BUTTON3_PIN) == HIGH) {
    delay(7000);
    if (digitalRead(BUTTON1_PIN) == LOW && digitalRead(BUTTON2_PIN) == LOW && digitalRead(BUTTON3_PIN) == HIGH) {
      Serial.print("learnmode on ");
      startLearningTime = millis();
      while (1) {
        if (millis() - startLearningTime > 10000) {
          Serial.println("Exiting learning mode due to timeout.");  // پیام خروج از حالت یادگیری را چاپ کنید
          break;
        }

        if (mySwitch.available()) {
          a = mySwitch.getReceivedValue();
          b = mySwitch.getReceivedValue();
          c = mySwitch.getReceivedValue();
          mySwitch.resetAvailable();
          if (digitalRead(BUTTON1_PIN) == LOW) {
            EEPROM.put(15, a);
            EEPROM.commit();
          }
          if (digitalRead(BUTTON2_PIN) == LOW) {
            EEPROM.put(30, b);
            EEPROM.commit();
          }
          if (digitalRead(BUTTON3_PIN) == LOW) {
            EEPROM.put(45, c);
            EEPROM.commit();
          }
        }
      }
    }
  }

  if (digitalRead(BUTTON1_PIN) == LOW) {
    EEPROM.get(15, a);
    mySwitch.send(a, 24);
    Serial.println(a);
    delay(50);
  }
  if (digitalRead(BUTTON2_PIN) == LOW) {
    EEPROM.get(30, b);
    mySwitch.send(b, 24);
    Serial.println(b);
    delay(50);
  }
  if (digitalRead(BUTTON3_PIN) == LOW) {
    EEPROM.get(45, c);
    mySwitch.send(c, 24);
    Serial.println(c);
    delay(50);
  }
}
