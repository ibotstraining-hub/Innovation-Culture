#include <ESP8266WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// ---------------- LED Matrix Settings ----------------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4   // 4-in-1 Display
#define CLK_PIN   D5    // SCK
#define DATA_PIN  D7    // MOSI
#define CS_PIN    D6    // CS

MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// ---------------- WiFi Settings ----------------
const char* ssid = "ESP_DISPLAY";
const char* password = "12345678";

WiFiServer server(80);
String message = "Welcome";

// -------------------------------------------------------

void setup() {
  Serial.begin(115200);

  // Initialize display
  Display.begin();
  Display.setIntensity(5);     // Brightness 0-15
  Display.displayClear();
  Display.displayScroll(message.c_str(), PA_CENTER, PA_SCROLL_LEFT, 50);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// -------------------------------------------------------

void loop() {
  Display.displayAnimate();

  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);

  String req = client.readStringUntil('\r');
  client.flush();

  if (req.indexOf("text=") != -1) {
    message = req.substring(req.indexOf("text=") + 5);
    message.replace("+", " ");
    message.replace("%20", " ");
    Serial.println("New Message: " + message);

    Display.displayClear();
    Display.displayScroll(message.c_str(), PA_CENTER, PA_SCROLL_LEFT, 50);
  }

  // Webpage HTML
  String webpage =
    "<html><head><title>LED Message Board</title></head>"
    "<body style='text-align:center;font-family:Arial;'>"
    "<h2>MAX7219 LED Display</h2>"
    "<form action='/' method='GET'>"
    "<input type='text' name='text' style='width:250px;height:30px;font-size:18px;'>"
    "<br><br>"
    "<input type='submit' value='Display' style='font-size:20px;padding:10px;'>"
    "</form>"
    "</body></html>";

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.print(webpage);
  client.stop();
}
