/**
 * Main Application Logic
 */

let workspace;

document.addEventListener("DOMContentLoaded", function () {
  // Initialize Blockly
  workspace = Blockly.inject("blocklyDiv", {
    toolbox: document.getElementById("toolbox"),
    zoom: {
      controls: true,
      wheel: true,
      startScale: 1.0,
      maxScale: 3,
      minScale: 0.3,
      scaleSpeed: 1.2,
    },
    grid: {
      spacing: 20,
      length: 3,
      colour: "#ccc",
      snap: true,
    },
    trashcan: true,
    sounds: true,
    move: {
      scrollbars: true,
      drag: true,
      wheel: true,
    },
  });

  // Initialize variable dropdown
  Blockly.Variables.flyoutCategory = Blockly.Variables.flyoutCategoryBlocks;

  // Event listeners
  document
    .getElementById("generateBtn")
    .addEventListener("click", generateAndDisplayCode);
  document
    .getElementById("copyBtn")
    .addEventListener("click", copyCodeToClipboard);
  document.getElementById("clearBtn").addEventListener("click", clearWorkspace);
  document
    .getElementById("downloadBtn")
    .addEventListener("click", downloadCode);
  document
    .getElementById("exportBtn")
    .addEventListener("click", exportToArduino);

  // Load saved workspace
  loadWorkspace();

  // Generate code on load to show template
  generateAndDisplayCode();

  // Auto-save workspace every 5 seconds
  setInterval(autoSaveWorkspace, 5000);
});

/**
 * Generate and display the Arduino code
 */
function generateAndDisplayCode() {
  try {
    const code = generateArduinoCode();
    document.getElementById("codeOutput").textContent = code;
  } catch (error) {
    console.error("Error generating code:", error);
    document.getElementById(
      "codeOutput"
    ).textContent = `Error: ${error.message}`;
  }
}

/**
 * Copy generated code to clipboard
 */
function copyCodeToClipboard() {
  const codeOutput = document.getElementById("codeOutput");
  const code = codeOutput.textContent;

  if (!code || code.startsWith("Error")) {
    alert("No valid code to copy");
    return;
  }

  navigator.clipboard
    .writeText(code)
    .then(() => {
      showNotification("Code copied to clipboard! ✓");
    })
    .catch((err) => {
      console.error("Failed to copy:", err);
      alert("Failed to copy code");
    });
}

/**
 * Clear all blocks from workspace
 */
function clearWorkspace() {
  if (confirm("Clear all blocks? This cannot be undone.")) {
    workspace.clear();
    generateAndDisplayCode();
  }
}

/**
 * Download generated code as .cpp file
 */
function downloadCode() {
  const code = document.getElementById("codeOutput").textContent;

  if (!code || code.startsWith("Error")) {
    alert("No valid code to download");
    return;
  }

  const element = document.createElement("a");
  element.setAttribute(
    "href",
    "data:text/plain;charset=utf-8," + encodeURIComponent(code)
  );
  element.setAttribute("download", "main.cpp");
  element.style.display = "none";

  document.body.appendChild(element);
  element.click();
  document.body.removeChild(element);

  showNotification("Code downloaded! ⬇️");
}

/**
 * Export generated code directly to src/main.cpp
 */
function exportToArduino() {
  const code = document.getElementById("codeOutput").textContent;

  if (!code || code.startsWith("Error")) {
    alert("No valid code to export");
    return;
  }

  // Show loading state
  const btn = event.target;
  const originalText = btn.textContent;
  btn.textContent = "⏳ Exporting...";
  btn.disabled = true;

  // Send to server
  fetch("/api/export", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ code: code }),
  })
    .then((response) => response.json())
    .then((data) => {
      if (data.status === "success") {
        showNotification(`✅ ${data.message}`);
        console.log(`📝 File: ${data.file}`);
        console.log(`📊 Size: ${data.size} bytes`);
        console.log(`📝 Next: ${data.next_steps[0]}`);
      } else {
        showNotification(`❌ Error: ${data.message}`);
      }
    })
    .catch((error) => {
      console.error("Export failed:", error);
      showNotification("❌ Export failed - check console");
    })
    .finally(() => {
      btn.textContent = originalText;
      btn.disabled = false;
    });
}

/**
 * Auto-save workspace to server
 */
function autoSaveWorkspace() {
  if (!workspace) return;

  try {
    const xml = Blockly.Xml.workspaceToDom(workspace);
    const xmlText = Blockly.Xml.domToText(xml);

    fetch("/api/workspace/save", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ workspace: xmlText }),
    }).catch((error) => {
      console.debug("Auto-save skipped (server may be unavailable)");
    });
  } catch (error) {
    console.debug("Auto-save error:", error);
  }
}

/**
 * Load workspace from server
 */
function loadWorkspace() {
  if (!workspace) return;

  fetch("/api/workspace/load")
    .then((response) => response.json())
    .then((data) => {
      if (data.workspace) {
        try {
          const xml = Blockly.Xml.textToDom(data.workspace);
          Blockly.Xml.domToWorkspace(xml, workspace);
          console.log("✅ Workspace loaded from server");
          generateAndDisplayCode();
        } catch (error) {
          console.error("Failed to load workspace:", error);
        }
      }
    })
    .catch((error) => {
      console.debug("Workspace load skipped:", error);
    });
}

/**
 * Show temporary notification
 */
function showNotification(message) {
  const notification = document.createElement("div");
  notification.className = "copy-success";
  notification.textContent = message;

  document.body.appendChild(notification);

  setTimeout(() => {
    notification.remove();
  }, 3000);
}

// Real-time code generation when blocks change
if (workspace) {
  workspace.addChangeListener(function (event) {
    if (
      event.type === Blockly.Events.BLOCK_CREATE ||
      event.type === Blockly.Events.BLOCK_DELETE ||
      event.type === Blockly.Events.BLOCK_CHANGE
    ) {
      generateAndDisplayCode();
    }
  });
}
