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

// PID constants for line following
const float Kp = 0.15;    // Proportional gain - how aggressively to correct
const float Kd = 1.0;     // Derivative gain - dampens oscillation
const float Ki = 0.0001;  // Integral gain - eliminates steady-state error

// PID variables
int lastError = 0;
float integral = 0;

// Motor speeds
const int baseSpeed = 160;
const int maxSpeed = 240;
const int turnSpeed = 180;

// Function declarations
void calibrateSensors(uint8_t steps);
void followLine();



void setup()
{
  Serial.begin(9600);
  
  motors.setLeftMotorPins(MOTOR_A_PWM, MOTOR_A_PIN_IN1, MOTOR_A_PIN_IN2, 0);
  motors.setRightMotorPins(MOTOR_B_PWM, MOTOR_B_PIN_IN1, MOTOR_B_PIN_IN2, 1);
  motors.setSTBYPin(STBY_PIN);

  qtr.setTypeRC();
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);
  qtr.setEmitterPin(IR_PIN);

  Serial.println("Starting calibration in 2 seconds...");
  Serial.println("Move robot left and right over the line!");
  delay(2000);
  calibrateSensors(150);
  
  Serial.println("Ready to race!");
  delay(500);
}

void loop()
{
  followLine();
}

void calibrateSensors(uint8_t steps)
{
  Serial.println("\nCalibrating...");
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (uint16_t i = 0; i < steps; i++)
  {
    qtr.calibrate();
    
    // Print progress dots
    if (i % 10 == 0)
      Serial.print(".");
    
    delay(20);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("\nCalibration complete!");
  
  // Print calibration values for debugging
  Serial.println("Min values:");
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  Serial.println("Max values:");
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
}

void followLine()
{
  // Read calibrated sensor values and get position (0-7000 for 8 sensors)
  // Position: 0 = far left, 3500 = center, 7000 = far right
  uint16_t position = qtr.readLineBlack(sensorValues);
  
  // Calculate error from center position (3500 for 8 sensors)
  // Negative error = line is to the left, turn left
  // Positive error = line is to the right, turn right
  int error = position - 3500;
  
  // PID calculation
  integral += error;
  
  // Anti-windup: prevent integral from growing too large
  integral = constrain(integral, -10000, 10000);
  
  int derivative = error - lastError;
  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;
  
  // Calculate motor speeds with correction
  // If line is on RIGHT (positive error), LEFT motor goes faster to turn right
  // If line is on LEFT (negative error), RIGHT motor goes faster to turn left
  int leftSpeed = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;
  
  // Constrain speeds to valid range (no reverse)
  leftSpeed = constrain(leftSpeed, 0, maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, maxSpeed);
  
  // Apply motor speeds
  motors.left().forward(leftSpeed);
  motors.right().forward(rightSpeed);
  
  // Debug output - uncomment to tune PID
  Serial.print("Pos: ");
  Serial.print(position);
  Serial.print(" | Err: ");
  Serial.print(error);
  Serial.print(" | Corr: ");
  Serial.print(correction);
  Serial.print(" | L: ");
  Serial.print(leftSpeed);
  Serial.print(" | R: ");
  Serial.println(rightSpeed);
}
