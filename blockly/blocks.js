/**
 * Custom Blockly Blocks for Line-Tracing Robot
 */

// ==================== ARDUINO STRUCTURE BLOCKS ====================

Blockly.Blocks["arduino_setup"] = {
  init: function () {
    this.appendDummyInput().appendField("Arduino Setup (Runs once)");
    this.appendStatementInput("SETUP_CODE").setCheck(null);
    this.setColour("#ffcc00");
    this.setTooltip("Code here runs once at startup");
    this.setHelpUrl("");
  },
};

Blockly.Blocks["arduino_loop"] = {
  init: function () {
    this.appendDummyInput().appendField("Arduino Loop (Runs forever)");
    this.appendStatementInput("LOOP_CODE").setCheck(null);
    this.setColour("#ffcc00");
    this.setTooltip("Code here runs repeatedly");
    this.setHelpUrl("");
  },
};

// ==================== ROBOT CONTROL BLOCKS ====================

Blockly.Blocks["robot_forward"] = {
  init: function () {
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Move Forward at Speed");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0099cc");
    this.setTooltip("Move robot forward");
    this.setHelpUrl("");
  },
};

Blockly.Blocks["robot_backward"] = {
  init: function () {
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Move Backward at Speed");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0099cc");
    this.setTooltip("Move robot backward");
  },
};

Blockly.Blocks["robot_turn_left"] = {
  init: function () {
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Turn Left at Speed");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0099cc");
    this.setTooltip("Turn robot left");
  },
};

Blockly.Blocks["robot_turn_right"] = {
  init: function () {
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Turn Right at Speed");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0099cc");
    this.setTooltip("Turn robot right");
  },
};

Blockly.Blocks["robot_stop"] = {
  init: function () {
    this.appendDummyInput().appendField("Stop Robot");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0099cc");
    this.setTooltip("Stop all motors");
  },
};

// ==================== MOTOR CONTROL BLOCKS ====================

Blockly.Blocks["set_motor_speed"] = {
  init: function () {
    this.appendDummyInput()
      .appendField("Set Motor Speed:")
      .appendField(
        new Blockly.FieldDropdown([
          ["Left", "LEFT"],
          ["Right", "RIGHT"],
        ]),
        "MOTOR"
      );
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Speed (0-255)");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0066cc");
    this.setTooltip("Set individual motor speed");
  },
};

Blockly.Blocks["set_left_motor"] = {
  init: function () {
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Set Left Motor Speed");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0066cc");
  },
};

Blockly.Blocks["set_right_motor"] = {
  init: function () {
    this.appendValueInput("SPEED")
      .setCheck("Number")
      .appendField("Set Right Motor Speed");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0066cc");
  },
};

// ==================== SENSOR BLOCKS ====================

Blockly.Blocks["read_sensors"] = {
  init: function () {
    this.appendDummyInput().appendField("Read All Sensors");
    this.setOutput(true, "Array");
    this.setColour("#ff6600");
    this.setTooltip("Read all 8 line sensors");
  },
};

Blockly.Blocks["get_sensor_value"] = {
  init: function () {
    this.appendDummyInput()
      .appendField("Get Sensor")
      .appendField(
        new Blockly.FieldDropdown([
          ["0", "0"],
          ["1", "1"],
          ["2", "2"],
          ["3", "3"],
          ["4", "4"],
          ["5", "5"],
          ["6", "6"],
          ["7", "7"],
        ]),
        "SENSOR"
      )
      .appendField("Value");
    this.setOutput(true, "Number");
    this.setColour("#ff6600");
    this.setTooltip("Get value of specific sensor (0-1)");
  },
};

Blockly.Blocks["line_detected"] = {
  init: function () {
    this.appendDummyInput()
      .appendField("Line Detected at")
      .appendField(
        new Blockly.FieldDropdown([
          ["Left", "LEFT"],
          ["Center", "CENTER"],
          ["Right", "RIGHT"],
        ]),
        "POSITION"
      );
    this.setOutput(true, "Boolean");
    this.setColour("#ff6600");
    this.setTooltip("Check if line is detected at position");
  },
};

// ==================== STANDARD BLOCKLY BLOCKS (Auto-generated) ====================

Blockly.Blocks["controls_if"] = {
  init: function () {
    this.appendValueInput("IF0").setCheck("Boolean").appendField("if");
    this.appendStatementInput("DO0").appendField("do");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ff6b6b");
    this.setTooltip("If condition is true, execute block");
    this.elseifCount_ = 0;
    this.elseCount_ = 0;
  },
};

Blockly.Blocks["logic_compare"] = {
  init: function () {
    this.appendValueInput("A").setCheck("Number");
    this.appendDummyInput().appendField(
      new Blockly.FieldDropdown([
        ["=", "EQ"],
        ["≠", "NEQ"],
        ["<", "LT"],
        ["≤", "LTE"],
        [">", "GT"],
        ["≥", "GTE"],
      ]),
      "OP"
    );
    this.appendValueInput("B").setCheck("Number");
    this.setOutput(true, "Boolean");
    this.setColour("#ff6b6b");
    this.setTooltip("Compare two numbers");
  },
};

Blockly.Blocks["logic_operation"] = {
  init: function () {
    this.appendValueInput("A").setCheck("Boolean");
    this.appendDummyInput().appendField(
      new Blockly.FieldDropdown([
        ["and", "AND"],
        ["or", "OR"],
      ]),
      "OP"
    );
    this.appendValueInput("B").setCheck("Boolean");
    this.setOutput(true, "Boolean");
    this.setColour("#ff6b6b");
    this.setTooltip("Logical AND / OR operation");
  },
};

Blockly.Blocks["controls_repeat_ext"] = {
  init: function () {
    this.appendDummyInput().appendField("repeat");
    this.appendValueInput("TIMES").setCheck("Number");
    this.appendDummyInput().appendField("times");
    this.appendStatementInput("DO");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ffaa00");
    this.setTooltip("Repeat block N times");
  },
};

Blockly.Blocks["controls_whileUntil"] = {
  init: function () {
    this.appendDummyInput().appendField(
      new Blockly.FieldDropdown([
        ["while", "WHILE"],
        ["until", "UNTIL"],
      ]),
      "MODE"
    );
    this.appendValueInput("BOOL").setCheck("Boolean");
    this.appendStatementInput("DO");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ffaa00");
  },
};

Blockly.Blocks["math_number"] = {
  init: function () {
    this.appendDummyInput().appendField(new Blockly.FieldNumber("0"), "NUM");
    this.setOutput(true, "Number");
    this.setColour("#cc99ff");
  },
};

Blockly.Blocks["math_arithmetic"] = {
  init: function () {
    this.appendValueInput("A").setCheck("Number");
    this.appendDummyInput().appendField(
      new Blockly.FieldDropdown([
        ["+", "ADD"],
        ["−", "MINUS"],
        ["×", "MULTIPLY"],
        ["÷", "DIVIDE"],
      ]),
      "OP"
    );
    this.appendValueInput("B").setCheck("Number");
    this.setOutput(true, "Number");
    this.setColour("#cc99ff");
  },
};

Blockly.Blocks["text"] = {
  init: function () {
    this.appendDummyInput().appendField(new Blockly.FieldTextInput(""), "TEXT");
    this.setOutput(true, "String");
    this.setColour("#999999");
  },
};

Blockly.Blocks["text_print"] = {
  init: function () {
    this.appendValueInput("TEXT").appendField("Print");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#999999");
  },
};

// ==================== ENHANCED I/O BLOCKS ====================

Blockly.Blocks["serial_print_line"] = {
  init: function () {
    this.appendValueInput("TEXT").appendField("Print Line:");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#999999");
    this.setTooltip("Send text to Serial Monitor with newline");
  },
};

Blockly.Blocks["serial_print_number"] = {
  init: function () {
    this.appendValueInput("NUMBER")
      .setCheck("Number")
      .appendField("Print Number:");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#999999");
    this.setTooltip("Send number to Serial Monitor");
  },
};

Blockly.Blocks["serial_delay"] = {
  init: function () {
    this.appendValueInput("MS")
      .setCheck("Number")
      .appendField("Wait (milliseconds):");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ff9999");
    this.setTooltip("Pause program execution");
  },
};

Blockly.Blocks["debug_all_sensors"] = {
  init: function () {
    this.appendDummyInput().appendField("Debug: Print All Sensors");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ff6600");
    this.setTooltip("Print all 8 sensor values to Serial");
  },
};

Blockly.Blocks["debug_sensor"] = {
  init: function () {
    this.appendDummyInput()
      .appendField("Debug: Sensor")
      .appendField(
        new Blockly.FieldDropdown([
          ["0", "0"],
          ["1", "1"],
          ["2", "2"],
          ["3", "3"],
          ["4", "4"],
          ["5", "5"],
          ["6", "6"],
          ["7", "7"],
        ]),
        "SENSOR_ID"
      );
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ff6600");
    this.setTooltip("Print specific sensor value");
  },
};

Blockly.Blocks["debug_position"] = {
  init: function () {
    this.appendDummyInput()
      .appendField("Debug: Line Position")
      .appendField(
        new Blockly.FieldDropdown([
          ["Left", "LEFT"],
          ["Center", "CENTER"],
          ["Right", "RIGHT"],
          ["All", "ALL"],
        ]),
        "POSITION"
      );
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ff6600");
    this.setTooltip("Print detected line position");
  },
};

Blockly.Blocks["motor_info"] = {
  init: function () {
    this.appendDummyInput()
      .appendField("Debug: Motor Speed -")
      .appendField(
        new Blockly.FieldDropdown([
          ["Left", "LEFT"],
          ["Right", "RIGHT"],
          ["Both", "BOTH"],
        ]),
        "MOTOR"
      );
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#0066cc");
    this.setTooltip("Print motor speed values");
  },
};

Blockly.Blocks["output_status"] = {
  init: function () {
    this.appendValueInput("MESSAGE").appendField("Status:");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour("#ffaa00");
    this.setTooltip("Output status message");
  },
};
