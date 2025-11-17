#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>

constexpr uint8_t SENSOR_COUNT = 8;
constexpr uint8_t SENSOR_PINS[SENSOR_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9};

QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

void setup()
{
  Serial.begin(9600);

  qtr.setTypeRC(); // digital
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);

  Serial.println("Calibrating...");
  for (auto i = 0; i < 150; i++)
  {
    qtr.calibrate();
    delay(20);
    Serial.print(".");
  }
  Serial.println("\nDone calbrating!!");
  delay(1000);

  // sample motor setup
  // Motor left = Motor(1, 2, 3, 4);
  // Motor right = Motor(1, 2, 3, 4);
  // MotorPair Motors = MotorPair(left, right);
}

void loop()
{
  // for (auto i = 0; i < SENSOR_COUNT; i++)
  // {
  //   sensorValues[i] = digitalRead(SENSOR_PINS[i]);
  // }

  qtr.readLineBlack(sensorValues);

  for (auto o : sensorValues)
  {
    Serial.print(o);
    Serial.print(" ");
  }

  delay(50);
  Serial.println();
}