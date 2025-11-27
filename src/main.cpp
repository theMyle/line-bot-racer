#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>
#include <HardwareMapping.h>

// sensor declarations
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];
uint16_t SENSOR_NOISE_THRESHHOLD = 500;

// motor declaration
MotorPair motors;

const float Kp = 18.0f; // 0.15
const float Ki = 0.0f;
const float Kd = 0.0f; // 1.0

const uint8_t MAX_SPD = 180;
const uint8_t BASE_SPD = 120;

float leftMotorFloatAccumulator = BASE_SPD;
float rightMotorFloatAccumulator = BASE_SPD;

// function definitions
void calibrateSensors(uint8_t steps);
void normalizeSensorValues(uint16_t *sensorValues, uint8_t size, uint16_t noiseThreshhold);
void printSensorValues(uint16_t *sensorValues, uint8_t sensorValuesSize);
float calculateErrorDigital(uint16_t *sensorValues, const int8_t *sensorBias, uint8_t size);

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
  calibrateSensors(100);

  // other stuff
  Serial.println("Running motors");
}

void loop()
{
  qtr.readLineBlack(sensorValues);
  normalizeSensorValues(sensorValues, SENSOR_COUNT, SENSOR_NOISE_THRESHHOLD);

  // calculate error
  float error = calculateErrorDigital(sensorValues, SENSOR_BIAS, SENSOR_COUNT);

  // calculate PID

  // Proportional
  float P = Kp * error;

  // Derivative
  float D = 0.0f;

  float pidOutput = P + D;

  int leftSpeed = (int)(BASE_SPD - pidOutput);
  int rightSpeed = (int)(BASE_SPD + pidOutput);

  leftSpeed = constrain(leftSpeed, 0, MAX_SPD);
  rightSpeed = constrain(rightSpeed, 0, MAX_SPD);

  // drive motors
  motors.getLeftMotor().forward(leftSpeed);
  motors.getRightMotor().forward(rightSpeed);
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
  motors.forward(150);
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

void printSensorValues(uint16_t *sensorValues, uint8_t sensorValuesSize)
{
  for (auto i = 0; i < sensorValuesSize; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }

  Serial.println("");
}

float calculateErrorDigital(uint16_t *sensorValues, const int8_t *sensorBias, uint8_t size)
{
  int weightedSum = 0;
  int total = 0;

  for (int i = 0; i < size; i++)
  {
    if (sensorValues[i])
    {
      weightedSum += sensorBias[i];
      total++;
    }
  }

  if (total == 0)
    return 0;

  return (float)weightedSum / total;
}