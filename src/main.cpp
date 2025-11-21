#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>
#include <HardwareMapping.h>

// sensor declarations
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

// motor declaration
MotorPair motors;

// other vars
uint16_t SENSOR_NOISE_THRESHHOLD = 300;

void calibrateSensors(uint8_t steps);
void normalizeSensorValues(uint16_t *sensorValues, uint8_t size, uint16_t noiseThreshhold);

void setup()
{
  Serial.begin(9600);

  // motor initialization
  motors.setLeftMotorPins(MOTOR_A_PWM, MOTOR_A_PIN_IN1, MOTOR_A_PIN_IN2, 0);
  motors.setRightMotorPins(MOTOR_B_PWM, MOTOR_B_PIN_IN1, MOTOR_B_PIN_IN2, 1);
  motors.setSTBYPin(STBY_PIN);

  // sensor initialization
  qtr.setTypeRC(); // digital
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);
  qtr.setEmitterPin(IR_PIN);
  // calibrateSensors(150);

  // other stuff
}

void loop()
{
  qtr.readLineBlack(sensorValues);
  normalizeSensorValues(sensorValues, SENSOR_COUNT, SENSOR_NOISE_THRESHHOLD);

  for (auto o : sensorValues)
  {
    Serial.print(o);
    Serial.print(" ");
  }

  delay(50);
  Serial.println();
}

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

void normalizeSensorValues(uint16_t *sensorValues, uint8_t sensorValuesSize, uint16_t noiseThreshhold)
{
  for (auto i = 0; i < sensorValuesSize; i++)
  {
    if (sensorValues[i] <= noiseThreshhold)
    {
      sensorValues[i] = 0;
    }
    else
    {
      sensorValues[i] = 1;
    }
  }
}