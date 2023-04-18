#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>

#define SERIAL_BAUDRATE     115200

#define PIN_SERVO1          32
#define PIN_SERVO2          33

#define SERVO_MIN_PULSE_US  100
#define SERVO_MAX_PULSE_US  2000

#define RETRACTED           0
#define EXTENDED            180
#define PARTIALLY_EXTENDED  90

Servo servo1;
Servo servo2;

typedef struct 
{
  uint32_t servo1_position;
  uint32_t servo2_position;
  uint32_t state_delay;
} state_t;

state_t latch_test_sequence[] =
{
  (state_t){.servo1_position = RETRACTED,  .servo2_position = EXTENDED,           .state_delay = 1000},
  (state_t){.servo1_position = RETRACTED,  .servo2_position = RETRACTED,          .state_delay = 1000},
  (state_t){.servo1_position = RETRACTED,  .servo2_position = PARTIALLY_EXTENDED, .state_delay = 1000},
  (state_t){.servo1_position = EXTENDED,   .servo2_position = RETRACTED,          .state_delay = 1000},
  (state_t){.servo1_position = RETRACTED,  .servo2_position = EXTENDED,           .state_delay = 1000},
};

uint32_t iteration_counter = 0;

void setup()
{
  EEPROM.begin(4);
  iteration_counter = EEPROM.read(0);

  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("--- Latch Tester ---");

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
}

void loop()
{
  Serial.print("Cycle #");
  Serial.println(iteration_counter++);
  EEPROM.write(0, iteration_counter);

  for (uint32_t sequence_index = 0; sequence_index < (sizeof(latch_test_sequence)/sizeof(state_t)); sequence_index++)
  {
    servo1.write(latch_test_sequence[sequence_index].servo1_position);
    servo2.write(latch_test_sequence[sequence_index].servo2_position);
    delay(latch_test_sequence[sequence_index].state_delay);
  }
}
