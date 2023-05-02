#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>

// #define RESET_COUNTER

#define SERIAL_BAUDRATE         115200

#define PIN_SERVO1              2
#define PIN_SERVO2              0

#define SERVO_MIN_PULSE_US      544
#define SERVO_MAX_PULSE_US      2400

#define RETRACTED               0
#define EXTENDED                180
#define PARTIALLY_EXTENDED      90

#define INITED_EEPROM_ADDRESS   0
#define COUNTER_EERPOM_ADDRESS  4

#define EEPROM_INITED_VALUE     0xDEADBEEF

Servo servo1;
Servo servo2;

typedef struct 
{
  int servo1_position;
  int servo2_position;
  int state_delay;
} state_t;

/**
 * @Max you can add and remove stages below, set the angles in degrees for each servo and how long it should dwell in each state.
 * Add and remove as you need. The names RETRACTED, EXTENDED and PARTIALLY_EXTENDED are just variables set above so that you can easily change
 * the range of motion without needing to change it in a few places. You can add more if you need by copying a #define ... line above and give it a new name
*/
state_t latch_test_sequence[] =
{
  (state_t){.servo1_position = RETRACTED,  .servo2_position = EXTENDED,           .state_delay = 1000},
  (state_t){.servo1_position = RETRACTED,  .servo2_position = RETRACTED,          .state_delay = 1000},
  (state_t){.servo1_position = RETRACTED,  .servo2_position = PARTIALLY_EXTENDED, .state_delay = 1000},
  (state_t){.servo1_position = EXTENDED,   .servo2_position = RETRACTED,          .state_delay = 1000},
  (state_t){.servo1_position = RETRACTED,  .servo2_position = EXTENDED,           .state_delay = 1000},
};

uint32_t iteration_counter = 0;
uint32_t eeprom_inited_variable = 0;

void setup()
{
  EEPROM.begin();
#ifdef RESET_COUNTER
  EEPROM.put(INITED_EEPROM_ADDRESS, EEPROM_INITED_VALUE);
  EEPROM.put(COUNTER_EERPOM_ADDRESS,iteration_counter);
#else
  EEPROM.get(INITED_EEPROM_ADDRESS, eeprom_inited_variable);

  if(eeprom_inited_variable == EEPROM_INITED_VALUE)
  {
    EEPROM.get(COUNTER_EERPOM_ADDRESS, iteration_counter);
  }
  else
  {
    EEPROM.put(INITED_EEPROM_ADDRESS, EEPROM_INITED_VALUE);
    EEPROM.put(COUNTER_EERPOM_ADDRESS,iteration_counter);
  }
#endif

  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("--- Latch Tester ---");

  servo1.attach(PIN_SERVO1, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
  servo2.attach(PIN_SERVO2, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
}

void loop()
{
  Serial.print("Cycle #");
  Serial.println(iteration_counter++);
  EEPROM.put(COUNTER_EERPOM_ADDRESS, iteration_counter);

  for (uint32_t sequence_index = 0; sequence_index < (sizeof(latch_test_sequence)/sizeof(state_t)); sequence_index++)
  {
    servo1.write(latch_test_sequence[sequence_index].servo1_position);
    servo2.write(latch_test_sequence[sequence_index].servo2_position);
    delay(latch_test_sequence[sequence_index].state_delay);
  }
}
