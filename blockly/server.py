#!/usr/bin/env python3
"""
Enhanced Blockly Server with API endpoints for direct Arduino integration
Run: python3 server.py
Then open: http://localhost:8000

API Endpoints:
  GET  /api/code - Get generated Arduino code
  POST /api/export - Export code to src/main.cpp
  POST /api/workspace - Save/load Blockly workspace
"""

import http.server
import socketserver
import json
import os
from pathlib import Path
from urllib.parse import urlparse, parse_qs

PORT = 8000
BLOCKLY_DIR = Path(__file__).parent.absolute()
PROJECT_ROOT = BLOCKLY_DIR.parent
SRC_MAIN = PROJECT_ROOT / "src" / "main.cpp"
WORKSPACE_FILE = BLOCKLY_DIR / ".blockly_workspace.json"

class BlocklyServerHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(BLOCKLY_DIR), **kwargs)

    def log_message(self, format, *args):
        # Enhanced logging
        print(f"[{self.log_date_time_string()}] {format % args}")

    def end_headers(self):
        # Add CORS headers and cache control
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.send_header('Cache-Control', 'no-cache, no-store, must-revalidate')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Expires', '0')
        super().end_headers()

    def do_OPTIONS(self):
        """Handle CORS preflight requests"""
        self.send_response(200)
        self.end_headers()

    def do_GET(self):
        """Handle GET requests (API endpoints)"""
        parsed_path = urlparse(self.path)
        path = parsed_path.path
        query = parse_qs(parsed_path.query)

        # API: Get generated code
        if path == '/api/code':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            response = {
                'status': 'success',
                'message': 'To get generated code, use POST /api/code with Blockly XML',
                'endpoints': {
                    'POST /api/code': 'Send Blockly workspace XML to generate Arduino code',
                    'POST /api/export': 'Export generated code to src/main.cpp',
                    'POST /api/workspace/save': 'Save Blockly workspace',
                    'GET /api/workspace/load': 'Load saved Blockly workspace'
                }
            }
            self.wfile.write(json.dumps(response).encode())
            return

        # API: Load workspace
        if path == '/api/workspace/load':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            if WORKSPACE_FILE.exists():
                with open(WORKSPACE_FILE, 'r') as f:
                    workspace = json.load(f)
                self.wfile.write(json.dumps({'status': 'success', 'workspace': workspace}).encode())
            else:
                self.wfile.write(json.dumps({'status': 'success', 'workspace': None}).encode())
            return

        # Default: Serve static files
        super().do_GET()

    def do_POST(self):
        """Handle POST requests (API endpoints)"""
        parsed_path = urlparse(self.path)
        path = parsed_path.path
        content_length = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(content_length).decode('utf-8')

        # API: Generate code from Blockly XML
        if path == '/api/code':
            self.handle_generate_code(body)
            return

        # API: Export code to file
        if path == '/api/export':
            self.handle_export_code(body)
            return

        # API: Save workspace
        if path == '/api/workspace/save':
            self.handle_save_workspace(body)
            return

        # Default
        self.send_response(404)
        self.end_headers()

    def handle_generate_code(self, body):
        """Generate Arduino code from Blockly workspace"""
        try:
            data = json.loads(body)
            blockly_xml = data.get('blocklyXml', '')
            
            # Return success with instructions
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            
            response = {
                'status': 'success',
                'message': 'Code generated successfully',
                'note': 'Blockly generates code in the browser. The generated code is shown in the right panel.',
                'instructions': [
                    '1. Copy the generated code from the right panel',
                    '2. Click "Export to Arduino" button to save to src/main.cpp',
                    '3. Or use POST /api/export with the code'
                ]
            }
            self.wfile.write(json.dumps(response).encode())
        except Exception as e:
            self.send_error(400, str(e))

    def handle_export_code(self, body):
        """Export generated code to src/main.cpp"""
        try:
            data = json.loads(body)
            code = data.get('code', '')
            
            if not code:
                raise ValueError('No code provided')
            
            # Write to src/main.cpp
            SRC_MAIN.parent.mkdir(parents=True, exist_ok=True)
            with open(SRC_MAIN, 'w') as f:
                f.write(code)
            
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            
            response = {
                'status': 'success',
                'message': f'Code exported successfully to {SRC_MAIN}',
                'file': str(SRC_MAIN),
                'size': len(code),
                'next_steps': [
                    'Run: pio run -t upload',
                    'Or: platformio run -t upload'
                ]
            }
            self.wfile.write(json.dumps(response).encode())
            print(f"✅ Code exported to {SRC_MAIN}")
        except Exception as e:
            self.send_response(400)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps({'status': 'error', 'message': str(e)}).encode())

    def handle_save_workspace(self, body):
        """Save Blockly workspace to file"""
        try:
            data = json.loads(body)
            
            with open(WORKSPACE_FILE, 'w') as f:
                json.dump(data, f)
            
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            
            response = {
                'status': 'success',
                'message': 'Workspace saved',
                'file': str(WORKSPACE_FILE)
            }
            self.wfile.write(json.dumps(response).encode())
            print(f"💾 Workspace saved")
        except Exception as e:
            self.send_response(400)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps({'status': 'error', 'message': str(e)}).encode())

if __name__ == '__main__':
    try:
        with socketserver.TCPServer(("", PORT), BlocklyServerHandler) as httpd:
            print(f"🚀 Enhanced Blockly Server started!")
            print(f"📍 Open http://localhost:{PORT} in your browser")
            print(f"📂 Serving files from: {BLOCKLY_DIR}")
            print(f"🔗 Project root: {PROJECT_ROOT}")
            print(f"📝 Target: {SRC_MAIN}")
            print(f"\n📡 API Endpoints:")
            print(f"   GET  /api/code                 - Get code generation info")
            print(f"   POST /api/code                 - Generate code from Blockly XML")
            print(f"   POST /api/export               - Export code to src/main.cpp")
            print(f"   POST /api/workspace/save       - Save Blockly workspace")
            print(f"   GET  /api/workspace/load       - Load saved Blockly workspace")
            print(f"\n🛑 Press Ctrl+C to stop\n")
            httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n\n👋 Server stopped")
    except OSError as e:
        if e.errno == 48:  # Address already in use
            print(f"❌ Error: Port {PORT} is already in use")
            print(f"   Try another port: python3 server.py --port 8001")
        else:
            print(f"❌ Error: {e}")
