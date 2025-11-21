# Blockly Visual Programmer for Line-Tracing Robot

A web-based visual programming interface to generate Arduino code for your line-tracing robot using Google Blockly with **direct Arduino integration**.

## 📁 Files

- **index.html** - Main web interface with Blockly editor
- **styles.css** - UI styling and layout
- **blocks.js** - Custom Blockly block definitions for robot control
- **generator.js** - Code generator to convert blocks to Arduino C++
- **app.js** - Main application logic and event handlers
- **server.py** - Enhanced Python HTTP server with API endpoints

## 🚀 Quick Start

### Option 1: Run Python Server (Recommended)

```bash
cd blockly
python3 server.py
```

Then open: **http://localhost:8000** in your browser

### Option 2: Run with Live Server (VS Code)

Install the "Live Server" extension, then right-click `index.html` → "Open with Live Server"

### Option 3: Run with Node.js

```bash
npx http-server
```

## 🎮 How to Use

### Workflow: From Blockly to Arduino

1. **Design in Blockly** - Drag and drop blocks onto the workspace
2. **Watch code generate** - Code updates in real-time on the right panel
3. **Export directly** - Click **"🚀 Export to Arduino"** button
4. **Automatic update** - Code is saved to `src/main.cpp`
5. **Upload to robot** - Run: `pio run -t upload`

### Controls

- **⚡ Generate Code** - Manually regenerate code
- **📋 Copy Code** - Copy to clipboard
- **🗑️ Clear Blocks** - Remove all blocks (⚠️ no undo!)
- **⬇️ Download** - Save as `main.cpp` file locally
- **🚀 Export to Arduino** - Save directly to `src/main.cpp` ⭐ **NEW!**

## 🔗 API Endpoints

The server provides REST API endpoints for programmatic access:

```
GET  /api/code                 - Get code generation info
POST /api/code                 - Generate code from Blockly XML
POST /api/export               - Export code to src/main.cpp
POST /api/workspace/save       - Save Blockly workspace
GET  /api/workspace/load       - Load saved Blockly workspace
```

### Example: Export code programmatically

```bash
curl -X POST http://localhost:8000/api/export \
  -H "Content-Type: application/json" \
  -d '{"code":"#include <Arduino.h>\n..."}'
```

## 📦 Block Categories

### 🤖 Robot Control

- Move Forward at Speed
- Move Backward at Speed
- Turn Left at Speed
- Turn Right at Speed
- Stop Robot

### ⚙️ Motors

- Set Motor Speed (Left/Right)
- Set Left Motor Speed
- Set Right Motor Speed

### 📊 Sensors

- Read All Sensors
- Get Sensor Value (0-7)
- Line Detected (Left/Center/Right)

### 🧠 Logic

- If conditions
- Comparisons (=, ≠, <, >, ≤, ≥)
- AND/OR operations

### 🔁 Loops

- Repeat N times
- While/Until loops

### 🧮 Math & Text

- Numbers
- Arithmetic operations
- Text/Print statements

## 💾 Auto-Save Feature

Your Blockly workspace is automatically saved to the server every 5 seconds. When you refresh the page, your blocks are automatically restored!

## ✨ Features

- ✨ Real-time code generation
- 📋 One-click copy to clipboard
- ⬇️ Download as .cpp file
- 🚀 **Direct export to Arduino** - NEW!
- 💾 **Auto-save workspace** - NEW!
- 🎨 Color-coded block categories
- 📱 Responsive design
- 🔧 Extensible block system
- 🔗 REST API for automation

## 🔄 Complete Workflow Example

1. Open http://localhost:8000
2. Drag a "Move Forward" block to workspace
3. Set speed to 200
4. Click **"🚀 Export to Arduino"**
5. See confirmation: "✅ Code exported successfully to /path/to/src/main.cpp"
6. In terminal, run: `pio run -t upload`
7. Your robot moves forward!

## 🛠️ Customization

To add more blocks:

1. Define the block in `blocks.js`
2. Create a generator in `generator.js`
3. Add to the toolbox in `index.html`

## 📝 Generated Code Structure

The system generates code like this:

```cpp
#include <Arduino.h>
#include <QTRSensors.h>
#include <motor/Motor.h>
#include <motor/MotorPair.h>
#include <HardwareMapping.h>

// Sensor declarations
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

// Motor declaration
MotorPair motors;

void setup() {
  Serial.begin(9600);
  // Motor/sensor initialization...
}

void loop() {
  qtr.readLineBlack(sensorValues);
  normalizeSensorValues(sensorValues, SENSOR_COUNT, SENSOR_NOISE_THRESHHOLD);

  // YOUR BLOCKLY CODE HERE
  if (sensorValues[3] || sensorValues[4]) {
    motors.forward(200);
  }

  delay(50);
}

void normalizeSensorValues(...) { ... }
```

## 🐛 Troubleshooting

**Q: "Export to Arduino" button doesn't work**
A: Make sure the server is running (`python3 server.py`)

**Q: Generated code has errors**
A: Check the console (F12) for details, verify motor/sensor classes

**Q: Blocks are gone after refresh**
A: Workspace auto-save may have issues - check server terminal for errors

**Q: Port 8000 already in use**
A: Try a different port: `python3 -m http.server 8001`
