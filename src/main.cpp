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
const float Kp = 8;   // Proportional - correction strength
const float Kd = 100; // Derivative - dampens oscillation
const float Ki = 0.0; // Integral - disabled (causes lag on sharp turns)

// ============================================================================
// SPEED SETTINGS
// ============================================================================
const int BASE_SPEED = 255; // Normal following speed
const int MAX_SPEED = 255;  // Maximum motor speed
const int TURN_SPEED = 150; // Speed for 90° turns

// ============================================================================
// DETECTION THRESHOLDS
// ============================================================================
const int LINE_THRESHOLD = 500; // Sensor value to detect black line

// ============================================================================
// TIMING SETTINGS
// ============================================================================
const int DELAY_BEFORE_TURN = 100; // Delay before executing turn (ms)
const int FORWARD_DIST_CM = 5;     // Distance to move at intersection
const int TIME_PER_CM = 17;        // Time to move 1cm at base speed (calibrate!)

// ============================================================================
// STATE VARIABLES
// ============================================================================
int lastError = 0;
int lastDirection = 1; // 1 = right, -1 = left (default: RIGHT)

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================
void calibrateSensors(uint8_t steps);
void followLine();
void setMotors(int left, int right);
void stopMotors();
uint8_t readSensorBits();
void turnRight();
void turnLeft();
void moveForward(int distanceCm);
void printDebug(const char *mode, uint8_t bits, int err, int left, int right);

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
    if (i % 10 == 0)
      Serial.print(".");
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

  // Convert to bit pattern for turn detection
  uint8_t bitSensor = readSensorBits();

  // Calculate error and PID terms
  float error = 3500 - position; // Positive = line is left, negative = line is right
  int derivative = error - lastError;

  // Detect turn direction using simple bit checks
  // Left 4 sensors = bits 7-4, Right 4 sensors = bits 3-0
  uint8_t leftSide = bitSensor >> 4;    // Upper 4 bits
  uint8_t rightSide = bitSensor & 0x0F; // Lower 4 bits

  // If left side has more sensors on, it's a left turn
  // If right side has more sensors on, it's a right turn
  if (leftSide >= 0x0E && rightSide <= 0x01)
  {
    lastDirection = -1; // Left turn detected
  }
  else if (rightSide >= 0x0E && leftSide <= 0x01)
  {
    lastDirection = 1; // Right turn detected
  }

  int leftSpeed, rightSpeed;
  const char *mode;

  // ------------------------------------------
  // CASE 1: All white - execute turn
  // ------------------------------------------
  if (bitSensor == 0)
  {
    // Line lost - execute turn in last known direction
    digitalWrite(LED_BUILTIN, HIGH);
    delay(DELAY_BEFORE_TURN);

    if (lastDirection > 0)
    {
      turnRight();
      mode = "TURN-R";
    }
    else
    {
      turnLeft();
      mode = "TURN-L";
    }

    digitalWrite(LED_BUILTIN, LOW);
    lastError = 0;
    return; // Exit early, turn handles its own motor control
  }
  // ------------------------------------------
  // CASE 2: All black - intersection handling
  // ------------------------------------------
  else if (bitSensor == 0xFF)
  {
    digitalWrite(LED_BUILTIN, HIGH);

    // Move forward to clear the intersection
    moveForward(FORWARD_DIST_CM);

    // Read sensors again to determine what's ahead
    qtr.readLineBlack(sensorValues);
    bitSensor = readSensorBits();

    // Check if still all black (stop point)
    while (bitSensor == 0xFF)
    {
      stopMotors();
      qtr.readLineBlack(sensorValues);
      bitSensor = readSensorBits();
    }

    // Determine action based on what we see
    if (bitSensor == 0)
    {
      // T-intersection: no line ahead, turn right (default)
      lastDirection = 1;
    }
    else
    {
      // Cross intersection: line continues, go straight
      lastDirection = 0; // Will use PID
    }

    digitalWrite(LED_BUILTIN, LOW);
    mode = "CROSS";
    leftSpeed = BASE_SPEED;
    rightSpeed = BASE_SPEED;
  }
  // ------------------------------------------
  // CASE 3: Normal PID following
  // ------------------------------------------
  else
  {
    // PID calculation
    float correction = (Kp * error) + (Kd * derivative);

    // Note: error positive = line is left, so right motor needs more speed
    leftSpeed = constrain(BASE_SPEED + correction, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(BASE_SPEED - correction, -MAX_SPEED, MAX_SPEED);

    mode = "PID";
  }

  lastError = error;

  // Apply motor speeds (handle reverse)
  if (leftSpeed >= 0)
  {
    motors.getLeftMotor().forward(leftSpeed);
  }
  else
  {
    motors.getLeftMotor().backward(-leftSpeed);
  }

  if (rightSpeed >= 0)
  {
    motors.getRightMotor().forward(rightSpeed);
  }
  else
  {
    motors.getRightMotor().backward(-rightSpeed);
  }

  // Debug output
  printDebug(mode, bitSensor, (int)error, leftSpeed, rightSpeed);
}

// ============================================================================
// SENSOR READING - Convert to bit pattern
// ============================================================================
uint8_t readSensorBits()
{
  uint8_t bits = 0;
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    if (sensorValues[i] > LINE_THRESHOLD)
    {
      bits |= (1 << (7 - i)); // MSB = leftmost sensor
    }
  }
  return bits;
}

// ============================================================================
// TURN FUNCTIONS
// ============================================================================
void turnRight()
{
  // Turn right until center sensor (sensor 3 or 4) detects line
  qtr.readLineBlack(sensorValues);
  while (sensorValues[4] < LINE_THRESHOLD)
  {
    motors.getLeftMotor().forward(TURN_SPEED);
    motors.getRightMotor().backward(TURN_SPEED);
    qtr.readLineBlack(sensorValues);
  }
  lastDirection = 1;
}

void turnLeft()
{
  // Turn left until center sensor detects line
  qtr.readLineBlack(sensorValues);
  while (sensorValues[3] < LINE_THRESHOLD)
  {
    motors.getLeftMotor().backward(TURN_SPEED);
    motors.getRightMotor().forward(TURN_SPEED);
    qtr.readLineBlack(sensorValues);
  }
  lastDirection = -1;
}

// ============================================================================
// MOVEMENT FUNCTIONS
// ============================================================================
void moveForward(int distanceCm)
{
  unsigned long moveTime = distanceCm * TIME_PER_CM;
  unsigned long startTime = millis();

  while (millis() - startTime < moveTime)
  {
    motors.getLeftMotor().forward(BASE_SPEED);
    motors.getRightMotor().forward(BASE_SPEED);
  }
}

void stopMotors()
{
  motors.stop();
}

void setMotors(int left, int right)
{
  motors.getLeftMotor().forward(left);
  motors.getRightMotor().forward(right);
}

void printDebug(const char *mode, uint8_t bits, int err, int left, int right)
{
  Serial.print(mode);
  Serial.print(F(" B:"));
  // Print binary
  for (int i = 7; i >= 0; i--)
  {
    Serial.print(bitRead(bits, i));
  }
  Serial.print(F(" E:"));
  Serial.print(err);
  Serial.print(F(" L:"));
  Serial.print(left);
  Serial.print(F(" R:"));
  Serial.println(right);
}
