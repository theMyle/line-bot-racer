#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>
#include <HardwareMapping.h>

QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

Motor motorLeft = Motor(MOTOR_A_PWM, MOTOR_A_PIN_IN1, MOTOR_A_PIN_IN2, 0);
Motor motorRight = Motor(MOTOR_B_PWM, MOTOR_B_PIN_IN1, MOTOR_B_PIN_IN2, 1);
MotorPair motorPair = MotorPair(motorLeft, motorRight, STBY_PIN);

uint16_t SENSOR_THRESHHOLD = 300;

void calibrateSensors(uint8_t steps)
{
  Serial.println("\nCalibrating\n");
  for (auto i = 0; i < steps; i++)
  {
    qtr.calibrate();
    delay(20);
    Serial.print(".");
  }
  Serial.println("\nCalibration Finished!");
}

void setup()
{
  Serial.begin(9600);

  // motor setup
  motorPair.initMotors();

  // sensor setup
  qtr.setTypeRC(); // digital
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);
  qtr.setEmitterPin(IR_PIN);
  // calibrateSensors(150);

  // other stuff
}

void loop()
{
  qtr.readLineBlack(sensorValues);

  for (auto &s : sensorValues)
  {
    if (s < SENSOR_THRESHHOLD)
    {
      s = 0;
      continue;
    }

    if (s > SENSOR_THRESHHOLD)
    {
      s = 1;
    }
  }

  for (auto o : sensorValues)
  {
    Serial.print(o);
    Serial.print(" ");
  }

  delay(50);
  Serial.println();
}