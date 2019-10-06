#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
#define DURATION 4


uint8_t servonum = 0;
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}
void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pwm.setPWM(0, 0, pulseWidth(180));//pulseWidth(180));
  pwm.setPWM(1, 0, (int(float(map(135, 0, 180, 500, 2200)) / 1000000 * FREQUENCY * 4096)));
  pwm.setPWM(2, 0, pulseWidth(180));
  pwm.setPWM(3, 0, (int(float(660) / 1000000 * FREQUENCY * 4096)));

  for (int i = 4; i <= 11; i += 1)
    pwm.setPWM(i, 0, pulseWidth(90));
    
}
int a = 1000;
void loop() {
  if (Serial.available())
  {
    int readValue = Serial.parseInt();
    //Serial.println(readValue);
    if (readValue >= 500 && readValue <= 2600)
      a = readValue;
    Serial.println(a);
    }
    pwm.setPWM(5, 0, int(float(a) / 1000000 * FREQUENCY * 4096));
//    delay(1000);
//    delay(1000);
//    pwm.setPWM(4, 0, (int(float(2400) / 1000000 * FREQUENCY * 4096)));
}
