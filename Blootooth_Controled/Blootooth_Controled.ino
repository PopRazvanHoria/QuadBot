#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
#define DURATION 4

uint8_t servonum = 0;
bool ok = 0;
bool flag = 0;
double state;
int c = 2;

void setup()
{
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pwm.setPWM(0, 0, pulseWidth(180));
  pwm.setPWM(1, 0, (int(float(500) / 1000000 * FREQUENCY * 4096)));
  pwm.setPWM(2, 0, pulseWidth(180));
  pwm.setPWM(3, 0, (int(float(660) / 1000000 * FREQUENCY * 4096)));

  for (int i = 4; i <= 11; i += 1)
    pwm.setPWM(i, 0, pulseWidth(90));
}
// Aseaza toate membrele intr-o pozitie initiala
void prep()
{
  if (flag == 0)
  {
    pwm.setPWM(0, 0, pulseWidth(180));
    pwm.setPWM(1, 0, (int(float(500) / 1000000 * FREQUENCY * 4096)));
    pwm.setPWM(2, 0, pulseWidth(180));
    pwm.setPWM(3, 0, (int(float(660) / 1000000 * FREQUENCY * 4096)));

    for (int i = 4; i <= 11; i += 1)
      pwm.setPWM(i, 0, pulseWidth(90));
  }
}
//
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}
void gradeChange(int numServo, int grade)
{
  pwm.setPWM(numServo, 0, pulseWidth(grade));
  pwm.setPWM(numServo + 4, 0, pulseWidth(180 - grade));
  pwm.setPWM(numServo + 6, 0, pulseWidth(grade));
  pwm.setPWM(numServo + 2, 0, pulseWidth(180 - grade));
}
void moveTwoLegs(int numServo, int fromPos, int toPos, bool ok = 0)
{
  int duration = DURATION ;
  int gradeMove = 1;
  unsigned long period = millis();
  if (fromPos < toPos)
  {
    for (int grade = fromPos ; grade <= toPos ;)
      if (millis() - period > duration)
      {
        gradeChange(numServo, grade);
        period = millis();
        grade += gradeMove;

      }
  }
  else
  {
    for (int grade = fromPos ; grade >= toPos ;)
      if (millis() - period > duration)
      {
        gradeChange(numServo, grade);
        period = millis();
        grade -= gradeMove;
      }
  }
}
void gradeCChange1(int numServo, int grade)
{
  pwm.setPWM(numServo, 0, pulseWidth(grade));
  //pwm.setPWM(numServo+4, 0, pulseWidth(180-grade));

  //pwm.setPWM(numServo+6, 0, pulseWidth(grade));
  pwm.setPWM(numServo + 2, 0, pulseWidth(180 - grade));
}
void gradeCChange2(int numServo, int grade)
{
  //pwm.setPWM(numServo, 0, pulseWidth(grade));
  pwm.setPWM(numServo + 4, 0, pulseWidth(180 - grade));

  pwm.setPWM(numServo + 6, 0, pulseWidth(grade));
  //pwm.setPWM(numServo+2, 0, pulseWidth(180-grade));
}
void moveCTwoLegs(int numServo, int fromPos, int toPos, bool ok = 0)
{
  int duration = DURATION ;
  int gradeMove = 1;
  unsigned long period = millis();
  if (fromPos < toPos)
  {
    for (int grade = fromPos ; grade <= toPos ;)
      if (millis() - period > duration)
      {
        gradeCChange1(numServo, grade);
        if (ok == 0)
          gradeCChange2(numServo, map(grade, fromPos, toPos, fromPos, 150));
        period = millis();
        grade += gradeMove;

      }
  }
  else
  {
    for (int grade = fromPos ; grade >= toPos ;)
      if (millis() - period > duration)
      {
        gradeCChange1(numServo, grade);
        if (ok == 0)
          gradeCChange2(numServo, map(grade, fromPos, toPos, fromPos, 30));
        period = millis();
        grade -= gradeMove;
      }
  }
}
void moveCCTwoLegs(int numServo, int fromPos, int toPos, bool ok = 0)
{
  int duration = DURATION ;
  int gradeMove = 1;
  unsigned long period = millis();
  if (fromPos < toPos)
  {
    for (int grade = fromPos ; grade <= toPos ;)
      if (millis() - period > duration)
      {
        gradeCChange1(numServo, grade);
        if (ok == 0)
          gradeCChange2(numServo, map(grade, fromPos, toPos, 30, toPos));
        period = millis();
        grade += gradeMove;

      }
  }
  else
  {
    for (int grade = fromPos ; grade >= toPos ;)
      if (millis() - period > duration)
      {
        gradeCChange1(numServo, grade);
        if (ok == 0)
          gradeCChange2(numServo, map(grade, fromPos, toPos, 150, toPos));
        period = millis();
        grade -= gradeMove;
      }
  }
}
int fla = 0;
int stare;
void loop() {
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.flush();
    Serial.println(state);
    fla = 0;
    if ( state == '1' || state == '0')
    {
      stare = state;
      Serial.println(state);
    }

    if (stare == '0')
    {
      prep();
      flag = 1;
    }

  }

  if (stare == '1')
  {

    for (int i = 0; i <= 2; i++)
    {
      flag = 0;
      if (ok == 0)
      {
        //1
        moveTwoLegs(4, 90, 55);
        ok = 1;
      }
      //2
      moveTwoLegs(5, 90, 55);
      //3
      moveTwoLegs(4, 55, 90);
      //4
      moveCTwoLegs(5, 55, 90);
      //5
      moveCTwoLegs(5, 90, 125, 1);
      //6
      moveTwoLegs(4, 90, 125);
      //7
      moveCCTwoLegs(5, 125, 90);
      //8
      moveCTwoLegs(4, 125, 90);
      //9
      moveCTwoLegs(4, 90, 55, 1);
    }
    prep();
  }
  else if (stare == '0')
  {
    prep();
    flag = 1;
    c = 2;
  }

}
