#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>

constexpr uint8_t SENSOR_COUNT = 8;
constexpr uint8_t SENSOR_PINS[SENSOR_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9};
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

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
  // MotorPair Motors = MotorPair(1, 1, 1, 1, 2, 3, 4, 5);

  // sensor setup
  qtr.setTypeRC(); // digital
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);
  calibrateSensors(150);

  // other stuff
}

void loop()
{
  qtr.readLineBlack(sensorValues);

  for (auto o : sensorValues)
  {
    Serial.print(o);
    Serial.print(" ");
  }

  delay(50);
  Serial.println();
}