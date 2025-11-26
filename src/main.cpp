#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>
#include <HardwareMapping.h>

// ============================================================================
// HARDWARE DECLARATIONS
// ============================================================================
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];
MotorPair motors;

// ============================================================================
// PID TUNING CONSTANTS
// ============================================================================
const float Kp = 0.08;      // Proportional - correction strength
const float Kd = 1.5;       // Derivative - dampens oscillation
const float Ki = 0.0;       // Integral - disabled (causes lag on sharp turns)

// ============================================================================
// SPEED SETTINGS
// ============================================================================
const int BASE_SPEED       = 120;   // Normal following speed
const int MAX_SPEED        = 255;   // Maximum motor speed
const int SHARP_TURN_SPEED = 200;   // Speed for 90° turns
const int SEARCH_SPEED     = 150;   // Speed when searching for line

// ============================================================================
// DETECTION THRESHOLDS
// ============================================================================
const int TURN_THRESHOLD   = 2500;  // Error threshold for sharp turn
const int LINE_THRESHOLD   = 500;   // Sensor value to detect black line
const int WHITE_THRESHOLD  = 500;   // Total sensor sum for "all white"
const int BLACK_COUNT      = 7;     // Sensors needed to detect cross/wide line

// ============================================================================
// TIMING SETTINGS
// ============================================================================
const unsigned long MAX_SEARCH_TIME = 500;  // Max search time (ms)

// ============================================================================
// STATE VARIABLES
// ============================================================================
int lastError = 0;
float integral = 0;
int lastDirection = 1;              // 1 = right, -1 = left (default: RIGHT)
unsigned long lineLostTime = 0;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================
void calibrateSensors(uint8_t steps);
void followLine();
void setMotors(int left, int right);
void printDebug(const char* mode, uint16_t pos, int err, int left, int right);

// ============================================================================
// SETUP
// ============================================================================
void setup()
{
  Serial.begin(9600);
  
  // Initialize motors
  motors.setLeftMotorPins(MOTOR_A_PWM, MOTOR_A_PIN_IN1, MOTOR_A_PIN_IN2, 0);
  motors.setRightMotorPins(MOTOR_B_PWM, MOTOR_B_PIN_IN1, MOTOR_B_PIN_IN2, 1);
  motors.setSTBYPin(STBY_PIN);

  // Initialize sensors
  qtr.setTypeRC();
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);
  qtr.setEmitterPin(IR_PIN);

  // Calibration sequence
  Serial.println(F("Calibration starting in 2 seconds..."));
  Serial.println(F("Move robot left/right over line!"));
  delay(2000);
  calibrateSensors(150);
  
  Serial.println(F("Ready to race!"));
  delay(500);
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop()
{
  followLine();
}

// ============================================================================
// SENSOR CALIBRATION
// ============================================================================
void calibrateSensors(uint8_t steps)
{
  Serial.println(F("\nCalibrating..."));
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (uint16_t i = 0; i < steps; i++)
  {
    qtr.calibrate();
    if (i % 10 == 0) Serial.print(".");
    delay(20);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("\nDone!"));
  
  // Print calibration results
  Serial.print(F("Min: "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  Serial.print(F("Max: "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
}

// ============================================================================
// LINE FOLLOWING ALGORITHM
// ============================================================================
void followLine()
{
  // Read sensor position (0-7000, center = 3500)
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = position - 3500;
  int absError = abs(error);
  
  // Analyze sensor readings
  int sensorsOnLine = 0;
  int sensorSum = 0;
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    sensorSum += sensorValues[i];
    if (sensorValues[i] > LINE_THRESHOLD) sensorsOnLine++;
  }
  
  bool allWhite = (sensorSum < WHITE_THRESHOLD);
  bool allBlack = (sensorsOnLine >= BLACK_COUNT);
  
  int leftSpeed, rightSpeed;
  const char* mode;
  
  // ------------------------------------------
  // CASE 1: Line lost - search (default RIGHT)
  // ------------------------------------------
  if (allWhite)
  {
    if (lineLostTime == 0) lineLostTime = millis();
    
    unsigned long searchTime = millis() - lineLostTime;
    
    if (searchTime < MAX_SEARCH_TIME)
    {
      // Turn in last known direction (default: RIGHT)
      if (lastDirection > 0) {
        leftSpeed = SEARCH_SPEED;
        rightSpeed = SEARCH_SPEED / 3;
      } else {
        leftSpeed = SEARCH_SPEED / 3;
        rightSpeed = SEARCH_SPEED;
      }
    }
    else
    {
      // Timeout - go straight slowly
      leftSpeed = BASE_SPEED / 2;
      rightSpeed = BASE_SPEED / 2;
    }
    mode = "SEARCH";
  }
  // ------------------------------------------
  // CASE 2: Cross/intersection - go straight
  // ------------------------------------------
  else if (allBlack)
  {
    leftSpeed = BASE_SPEED;
    rightSpeed = BASE_SPEED;
    lineLostTime = 0;
    mode = "CROSS";
  }
  // ------------------------------------------
  // CASE 3: Sharp turn (90°)
  // ------------------------------------------
  else if (absError > TURN_THRESHOLD)
  {
    lastDirection = (error > 0) ? 1 : -1;
    
    if (error > 0) {
      leftSpeed = SHARP_TURN_SPEED;
      rightSpeed = 0;
    } else {
      leftSpeed = 0;
      rightSpeed = SHARP_TURN_SPEED;
    }
    
    integral = 0;
    lineLostTime = 0;
    mode = "SHARP";
  }
  // ------------------------------------------
  // CASE 4: Normal PID following
  // ------------------------------------------
  else
  {
    // Remember direction for recovery
    if (absError > 500) lastDirection = (error > 0) ? 1 : -1;
    
    // PID calculation
    integral = constrain(integral + error, -5000, 5000);
    int derivative = error - lastError;
    float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
    
    leftSpeed = constrain(BASE_SPEED - correction, 0, MAX_SPEED);
    rightSpeed = constrain(BASE_SPEED + correction, 0, MAX_SPEED);
    
    lineLostTime = 0;
    mode = "PID";
  }
  
  lastError = error;
  
  // Apply motor speeds
  setMotors(leftSpeed, rightSpeed);
  
  // Debug output
  printDebug(mode, position, error, leftSpeed, rightSpeed);
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================
void setMotors(int left, int right)
{
  motors.left().forward(left);
  motors.right().forward(right);
}

void printDebug(const char* mode, uint16_t pos, int err, int left, int right)
{
  Serial.print(mode);
  Serial.print(F(" P:"));
  Serial.print(pos);
  Serial.print(F(" E:"));
  Serial.print(err);
  Serial.print(F(" L:"));
  Serial.print(left);
  Serial.print(F(" R:"));
  Serial.println(right);
}
