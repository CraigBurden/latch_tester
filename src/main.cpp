#include <Arduino.h>

#define SERIAL_BAUDRATE     115200

#define PIN_SOLENOID1       1
#define PIN_SOLENOID2       2

#define RETRACTED           255
#define EXTENDED            0
#define PARTIALLY_EXTENDED  127

#define STAGE1_DELAY        1000
#define STAGE2_DELAY        1000
#define STAGE3_DELAY        1000
#define STAGE4_DELAY        1000
#define STAGE5_DELAY        1000

void state_1(void);
void state_2(void);
void state_3(void);
void state_4(void);
void state_5(void);

int iteration_counter = 1;

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("--- Latch Tester ---");

  pinMode(PIN_SOLENOID1, OUTPUT);
  pinMode(PIN_SOLENOID2, OUTPUT);

  analogWrite(PIN_SOLENOID1, EXTENDED);
  analogWrite(PIN_SOLENOID2, EXTENDED);
}

void loop()
{
  Serial.print("Cycle #");
  Serial.print(iteration_counter++);
  Serial.println(" (");

  Serial.print("1-");
  state_1();
  delay(STAGE1_DELAY);

  Serial.print("2-");
  state_2();
  delay(STAGE2_DELAY);

  Serial.print("3-");
  state_3();
  delay(STAGE3_DELAY);

  Serial.print("4-");
  state_4();
  delay(STAGE4_DELAY);
  
  Serial.print("5");
  state_5();
  delay(STAGE5_DELAY);

  Serial.println(")");
}

void state_1(void)
{
  analogWrite(PIN_SOLENOID1, RETRACTED);
  analogWrite(PIN_SOLENOID2, EXTENDED);
}

void state_2(void)
{
  analogWrite(PIN_SOLENOID1, RETRACTED);
  analogWrite(PIN_SOLENOID2, RETRACTED);
}

void state_3(void)
{
  analogWrite(PIN_SOLENOID1, RETRACTED);
  analogWrite(PIN_SOLENOID2, PARTIALLY_EXTENDED);
}

void state_4(void)
{
  analogWrite(PIN_SOLENOID1, EXTENDED);
  analogWrite(PIN_SOLENOID2, RETRACTED);
}

void state_5(void)
{
  analogWrite(PIN_SOLENOID1, RETRACTED);
  analogWrite(PIN_SOLENOID2, EXTENDED);
}
