#include <WiFi.h>            // Include the Wi-Fi library for connecting to a network
#include <WebServer.h>       // Include the WebServer library for creating a web server

// Define the GPIO pins for the RGB LED
#define PIN_RED    18        // GPIO pin connected to the red LED
#define PIN_GREEN  5         // GPIO pin connected to the green LED
#define PIN_BLUE   17        // GPIO pin connected to the blue LED

// Wi-Fi credentials
const char* ssid = "Your_SSID";        // Replace with your network SSID (Wi-Fi name)
const char* password = "Your_PASSWORD";// Replace with your network password

// Create a WebServer object that listens on port 80 (HTTP)
WebServer server(80);

// Variables to store the current RGB values (0-255)
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  // Initialize serial communication at 115200 baud for debugging
  Serial.begin(115200);

  // Set the RGB LED pins as outputs
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  // Connect to the Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Wait for connection
    Serial.println("Connecting to WiFi..."); // Print status to Serial Monitor
  }
  Serial.println("Connected to WiFi");        // Print confirmation of connection
  Serial.println(WiFi.localIP());             // Print the ESP32's local IP address

  // Define the route for the root URL ("/") and associate it with the handleRoot function
  server.on("/", handleRoot);

  // Define the route for "/setRGB" and associate it with the handleSetRGB function
  server.on("/setRGB", handleSetRGB);

  // Start the web server
  server.begin();
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}

// Function to generate and serve the HTML page
void handleRoot() {
  // Create a string to hold the HTML content
  String html = "<!DOCTYPE html><html lang='en'>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 RGB Control</title></head>";
  html += "<body><h1>ESP32 RGB LED Control</h1>";

  // Create sliders for red, green, and blue values
  html += "<p>Red: <input type='range' id='red' min='0' max='255' value='" + String(redValue) + "' oninput='updateRGB()'></p>";
  html += "<p>Green: <input type='range' id='green' min='0' max='255' value='" + String(greenValue) + "' oninput='updateRGB()'></p>";
  html += "<p>Blue: <input type='range' id='blue' min='0' max='255' value='" + String(blueValue) + "' oninput='updateRGB()'></p>";

  // JavaScript to handle the slider input and send the RGB values to the server
  html += "<script>";
  html += "function updateRGB() {";
  html += "var red = document.getElementById('red').value;";
  html += "var green = document.getElementById('green').value;";
  html += "var blue = document.getElementById('blue').value;";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/setRGB?red=' + red + '&green=' + green + '&blue=' + blue, true);";
  html += "xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";

  // Send the HTML content to the client (browser)
  server.send(200, "text/html", html);
}

// Function to handle the /setRGB URL and update the RGB values
void handleSetRGB() {
  // Check if the red, green, and blue parameters are present in the request and update the variables
  if (server.hasArg("red")) {
    redValue = server.arg("red").toInt();
  }
  if (server.hasArg("green")) {
    greenValue = server.arg("green").toInt();
  }
  if (server.hasArg("blue")) {
    blueValue = server.arg("blue").toInt();
  }

  // Constrain the values to be within the range of 0-255
  redValue = constrain(redValue, 0, 255);
  greenValue = constrain(greenValue, 0, 255);
  blueValue = constrain(blueValue, 0, 255);

  // Set the PWM values for the RGB LED
  analogWrite(PIN_RED, redValue);
  analogWrite(PIN_GREEN, greenValue);
  analogWrite(PIN_BLUE, blueValue);

  // Send a plain text response to confirm the RGB values were updated
  server.send(200, "text/plain", "RGB values updated");
}
