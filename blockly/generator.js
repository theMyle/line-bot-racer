/**
 * Code Generator - Converts Blockly blocks to Arduino C++ code
 */

// Initialize code generator for C/C++
Blockly.Arduino = new Blockly.Generator("Arduino");

// Define operator precedence
Blockly.Arduino.ORDER_ATOMIC = 0;
Blockly.Arduino.ORDER_EXPONENTIATION = 1;
Blockly.Arduino.ORDER_UNARY_PLUS = 2;
Blockly.Arduino.ORDER_UNARY_MINUS = 3;
Blockly.Arduino.ORDER_UNARY_NOT = 4;
Blockly.Arduino.ORDER_EXPONENTIATION = 5;
Blockly.Arduino.ORDER_MULTIPLICATIVE = 5;
Blockly.Arduino.ORDER_ADDITIVE = 6;
Blockly.Arduino.ORDER_SHIFT = 7;
Blockly.Arduino.ORDER_RELATIONAL = 8;
Blockly.Arduino.ORDER_EQUALITY = 9;
Blockly.Arduino.ORDER_BITWISE_AND = 10;
Blockly.Arduino.ORDER_BITWISE_XOR = 11;
Blockly.Arduino.ORDER_BITWISE_OR = 12;
Blockly.Arduino.ORDER_LOGICAL_AND = 13;
Blockly.Arduino.ORDER_LOGICAL_OR = 14;
Blockly.Arduino.ORDER_CONDITIONAL = 15;
Blockly.Arduino.ORDER_ASSIGNMENT = 16;
Blockly.Arduino.ORDER_NONE = 99;

// ==================== STRUCTURE GENERATORS ====================

Blockly.Arduino["arduino_setup"] = function (block) {
  var branch = Blockly.Arduino.statementToCode(block, "SETUP_CODE");
  Blockly.Arduino.setupCode_ = branch;
  return "";
};

Blockly.Arduino["arduino_loop"] = function (block) {
  var branch = Blockly.Arduino.statementToCode(block, "LOOP_CODE");
  Blockly.Arduino.loopCode_ = branch;
  return "";
};

// ==================== ROBOT CONTROL GENERATORS ====================

Blockly.Arduino["robot_forward"] = function (block) {
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );
  return `motors.forward(${speed});\n`;
};

Blockly.Arduino["robot_backward"] = function (block) {
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );
  return `motors.backward(${speed});\n`;
};

Blockly.Arduino["robot_turn_left"] = function (block) {
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );
  return `motors.left(${speed});\n`;
};

Blockly.Arduino["robot_turn_right"] = function (block) {
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );
  return `motors.right(${speed});\n`;
};

Blockly.Arduino["robot_stop"] = function (block) {
  return `motors.stop();\n`;
};

// ==================== MOTOR CONTROL GENERATORS ====================

Blockly.Arduino["set_motor_speed"] = function (block) {
  const motor = block.getFieldValue("MOTOR");
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );

  if (motor === "LEFT") {
    return `motors.left().setSpeed(${speed});\n`;
  } else {
    return `motors.getRightMotor().setSpeed(${speed});\n`;
  }
};

Blockly.Arduino["set_left_motor"] = function (block) {
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );
  return `motors.left().setSpeed(${speed});\n`;
};

Blockly.Arduino["set_right_motor"] = function (block) {
  const speed = Blockly.Arduino.valueToCode(
    block,
    "SPEED",
    Blockly.Arduino.ORDER_ATOMIC
  );
  return `motors.getRightMotor().setSpeed(${speed});\n`;
};

// ==================== SENSOR GENERATORS ====================

Blockly.Arduino["read_sensors"] = function (block) {
  const varName = "sensorValues";
  return [`${varName}`, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino["get_sensor_value"] = function (block) {
  const sensorIndex = block.getFieldValue("SENSOR");
  return [`sensorValues[${sensorIndex}]`, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino["line_detected"] = function (block) {
  const position = block.getFieldValue("POSITION");
  let condition = "";

  switch (position) {
    case "LEFT":
      condition = "(sensorValues[0] || sensorValues[1] || sensorValues[2])";
      break;
    case "CENTER":
      condition = "(sensorValues[3] || sensorValues[4])";
      break;
    case "RIGHT":
      condition = "(sensorValues[5] || sensorValues[6] || sensorValues[7])";
      break;
  }

  return [condition, Blockly.Arduino.ORDER_LOGICAL_OR];
};

// ==================== STANDARD LOGIC GENERATORS ====================

Blockly.Arduino["controls_if"] = function (block) {
  let code = "";
  let i = 0;

  // Get first IF condition
  const firstCondition =
    Blockly.Arduino.valueToCode(block, "IF0", Blockly.Arduino.ORDER_NONE) ||
    "false";
  const firstStatement = Blockly.Arduino.statementToCode(block, "DO0");
  code += `if (${firstCondition}) {\n${firstStatement}}\n`;

  return code;
};

Blockly.Arduino["logic_compare"] = function (block) {
  const OPERATORS = {
    EQ: "==",
    NEQ: "!=",
    LT: "<",
    LTE: "<=",
    GT: ">",
    GTE: ">=",
  };

  const operator = OPERATORS[block.getFieldValue("OP")];
  const a =
    Blockly.Arduino.valueToCode(block, "A", Blockly.Arduino.ORDER_RELATIONAL) ||
    "0";
  const b =
    Blockly.Arduino.valueToCode(block, "B", Blockly.Arduino.ORDER_RELATIONAL) ||
    "0";

  return [`(${a} ${operator} ${b})`, Blockly.Arduino.ORDER_RELATIONAL];
};

Blockly.Arduino["logic_operation"] = function (block) {
  const operator = block.getFieldValue("OP") === "AND" ? "&&" : "||";
  const a =
    Blockly.Arduino.valueToCode(
      block,
      "A",
      Blockly.Arduino.ORDER_LOGICAL_AND
    ) || "false";
  const b =
    Blockly.Arduino.valueToCode(
      block,
      "B",
      Blockly.Arduino.ORDER_LOGICAL_AND
    ) || "false";

  return [`(${a} ${operator} ${b})`, Blockly.Arduino.ORDER_LOGICAL_AND];
};

Blockly.Arduino["controls_repeat_ext"] = function (block) {
  const times =
    Blockly.Arduino.valueToCode(block, "TIMES", Blockly.Arduino.ORDER_ATOMIC) ||
    "10";
  const statement = Blockly.Arduino.statementToCode(block, "DO");

  return `for (int i = 0; i < ${times}; i++) {\n${statement}}\n`;
};

Blockly.Arduino["controls_whileUntil"] = function (block) {
  const mode = block.getFieldValue("MODE");
  const condition =
    Blockly.Arduino.valueToCode(block, "BOOL", Blockly.Arduino.ORDER_NONE) ||
    "true";
  const statement = Blockly.Arduino.statementToCode(block, "DO");

  let conditionCode = condition;
  if (mode === "UNTIL") {
    conditionCode = `!(${condition})`;
  }

  // This is a statement block, not an expression, so just return the code string
  return `while (${conditionCode}) {\n${statement}}\n`;
};

Blockly.Arduino["math_number"] = function (block) {
  const num = Number(block.getFieldValue("NUM"));
  return [String(num), Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino["math_arithmetic"] = function (block) {
  const OPERATORS = {
    ADD: "+",
    MINUS: "-",
    MULTIPLY: "*",
    DIVIDE: "/",
  };

  const operator = OPERATORS[block.getFieldValue("OP")];
  const a =
    Blockly.Arduino.valueToCode(block, "A", Blockly.Arduino.ORDER_ADDITIVE) ||
    "0";
  const b =
    Blockly.Arduino.valueToCode(block, "B", Blockly.Arduino.ORDER_ADDITIVE) ||
    "0";

  return [`(${a} ${operator} ${b})`, Blockly.Arduino.ORDER_ADDITIVE];
};

Blockly.Arduino["text"] = function (block) {
  const text = block.getFieldValue("TEXT");
  return [`"${text}"`, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino["text_print"] = function (block) {
  const text =
    Blockly.Arduino.valueToCode(block, "TEXT", Blockly.Arduino.ORDER_ATOMIC) ||
    '""';
  return `Serial.println(${text});\n`;
};

// Handle variables
Blockly.Arduino["variables_get"] = function (block) {
  const varName = block.getFieldValue("VAR");
  return [varName, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino["variables_set"] = function (block) {
  const varName = block.getFieldValue("VAR");
  const value =
    Blockly.Arduino.valueToCode(
      block,
      "VALUE",
      Blockly.Arduino.ORDER_ASSIGNMENT
    ) || "0";
  return `${varName} = ${value};\n`;
};

// ==================== ENHANCED I/O GENERATORS ====================

Blockly.Arduino["serial_print_line"] = function (block) {
  const text =
    Blockly.Arduino.valueToCode(block, "TEXT", Blockly.Arduino.ORDER_ATOMIC) ||
    '""';
  return `Serial.println(${text});\n`;
};

Blockly.Arduino["serial_print_number"] = function (block) {
  const number =
    Blockly.Arduino.valueToCode(
      block,
      "NUMBER",
      Blockly.Arduino.ORDER_ATOMIC
    ) || "0";
  return `Serial.println(${number});\n`;
};

Blockly.Arduino["serial_delay"] = function (block) {
  const ms =
    Blockly.Arduino.valueToCode(block, "MS", Blockly.Arduino.ORDER_ATOMIC) ||
    "100";
  return `delay(${ms});\n`;
};

Blockly.Arduino["debug_all_sensors"] = function (block) {
  return `
  Serial.print("Sensors: ");
  for (int i = 0; i < SENSOR_COUNT; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();
`;
};

Blockly.Arduino["debug_sensor"] = function (block) {
  const sensorId = block.getFieldValue("SENSOR_ID");
  return `Serial.print("Sensor ${sensorId}: "); Serial.println(sensorValues[${sensorId}]);\n`;
};

Blockly.Arduino["debug_position"] = function (block) {
  const position = block.getFieldValue("POSITION");
  let code = "";

  switch (position) {
    case "LEFT":
      code = `Serial.println("Line at LEFT: " + String(sensorValues[0] || sensorValues[1] || sensorValues[2]));\n`;
      break;
    case "CENTER":
      code = `Serial.println("Line at CENTER: " + String(sensorValues[3] || sensorValues[4]));\n`;
      break;
    case "RIGHT":
      code = `Serial.println("Line at RIGHT: " + String(sensorValues[5] || sensorValues[6] || sensorValues[7]));\n`;
      break;
    case "ALL":
      code = `
  Serial.println("=== Line Position ===");
  Serial.println("LEFT: " + String(sensorValues[0] || sensorValues[1] || sensorValues[2]));
  Serial.println("CENTER: " + String(sensorValues[3] || sensorValues[4]));
  Serial.println("RIGHT: " + String(sensorValues[5] || sensorValues[6] || sensorValues[7]));
`;
      break;
  }

  return code;
};

Blockly.Arduino["motor_info"] = function (block) {
  const motor = block.getFieldValue("MOTOR");
  let code = "";

  switch (motor) {
    case "LEFT":
      code = `Serial.println("Left Motor Speed: " + String(motors.left().getSpeed()));\n`;
      break;
    case "RIGHT":
      code = `Serial.println("Right Motor Speed: " + String(motors.getRightMotor().getSpeed()));\n`;
      break;
    case "BOTH":
      code = `
  Serial.println("Motor Speeds - Left: " + String(motors.left().getSpeed()) + " Right: " + String(motors.getRightMotor().getSpeed()));
`;
      break;
  }

  return code;
};

Blockly.Arduino["output_status"] = function (block) {
  const message =
    Blockly.Arduino.valueToCode(
      block,
      "MESSAGE",
      Blockly.Arduino.ORDER_ATOMIC
    ) || '""';
  return `Serial.println("[STATUS] " + String(${message}));\n`;
};

/**
 * Generate full Arduino sketch from workspace
 */
function generateArduinoCode() {
  const workspace = Blockly.getMainWorkspace();

  // Initialize code generator
  Blockly.Arduino.init(workspace);

  // Generate variable declarations
  const allVariables = workspace.getAllVariables();
  let varDeclarations = "";
  allVariables.forEach((variable) => {
    const varName = variable.name;
    varDeclarations += `uint16_t ${varName} = 0;\n`;
  });

  // Reset custom code containers
  Blockly.Arduino.setupCode_ = "";
  Blockly.Arduino.loopCode_ = "";

  // Generate code for all blocks
  // This will populate setupCode_ and loopCode_ via the block generators
  // Any other blocks will return their code into orphanCode
  const orphanCode = Blockly.Arduino.workspaceToCode(workspace);

  // Build complete Arduino sketch
  const fullCode = `#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>
#include <HardwareMapping.h>

// Sensor declarations
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

// Motor declaration
MotorPair motors;

// Other vars
uint16_t SENSOR_NOISE_THRESHHOLD = 300;

// Function declaration
void normalizeSensorValues(uint16_t *sensorValues, uint8_t sensorValuesSize, uint16_t noiseThreshhold);

${varDeclarations}

void setup()
{
  Serial.begin(9600);

  // Motor initialization
  motors.setLeftMotorPins(MOTOR_A_PWM, MOTOR_A_PIN_IN1, MOTOR_A_PIN_IN2, 0);
  motors.setRightMotorPins(MOTOR_B_PWM, MOTOR_B_PIN_IN1, MOTOR_B_PIN_IN2, 1);
  motors.setSTBYPin(STBY_PIN);

  // Sensor initialization
  qtr.setTypeRC();
  qtr.setSensorPins(SENSOR_PINS, SENSOR_COUNT);
  qtr.setEmitterPin(IR_PIN);

  // User setup code
  ${Blockly.Arduino.setupCode_}
}

void loop()
{
  qtr.readLineBlack(sensorValues);
  normalizeSensorValues(sensorValues, SENSOR_COUNT, SENSOR_NOISE_THRESHHOLD);

  // User loop code
  ${Blockly.Arduino.loopCode_}
  
  // Orphan blocks (placed outside setup/loop)
  ${orphanCode}

  delay(50);
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
`;

  return fullCode;
}
