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

typedef struct 
{
  uint8_t solenoid1_position;
  uint8_t solenoid2_position;
  uint32_t state_delay;
} state_t;

state_t latch_test_sequence[] =
{
  (state_t){.solenoid1_position = RETRACTED,  .solenoid2_position = EXTENDED,           .state_delay = 1000},
  (state_t){.solenoid1_position = RETRACTED,  .solenoid2_position = RETRACTED,          .state_delay = 1000},
  (state_t){.solenoid1_position = RETRACTED,  .solenoid2_position = PARTIALLY_EXTENDED, .state_delay = 1000},
  (state_t){.solenoid1_position = EXTENDED,   .solenoid2_position = RETRACTED,          .state_delay = 1000},
  (state_t){.solenoid1_position = RETRACTED,  .solenoid2_position = EXTENDED,           .state_delay = 1000},
};

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
  Serial.println(iteration_counter++);

  for (uint32_t sequence_index = 0; sequence_index < (sizeof(latch_test_sequence)/sizeof(state_t)); sequence_index++)
  {
    analogWrite(PIN_SOLENOID1, latch_test_sequence[sequence_index].solenoid1_position);
    analogWrite(PIN_SOLENOID2, latch_test_sequence[sequence_index].solenoid2_position);
    delay(latch_test_sequence[sequence_index].state_delay);
  }
}
