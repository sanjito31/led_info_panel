#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <time.h>
#include <WiFi.h>
#include <secrets.h>

// RGB Panel Info
#define PANEL_W         64
#define PANEL_H         32
#define PANEL_CHAIN     1

// Serial BAUD Rate
#define BAUD            115200

// WiFi Network Info
const char* SSID        = WIFI_SSID;
const char* PASS        = WIFI_PASS;
int WIFI_CONN           = 0;

// Time 
struct tm timeinfo;
const char* TIME_SERVER = "pool.ntp.org";
const int DST_OFFSET    = 3600;
const long GMT_OFFSET   = -(5 * 3600);

// MatrixPanel_I2S_DMA display
MatrixPanel_I2S_DMA *dma_display    = nullptr;

// Setup 
void setup() {
  // Start
  Serial.begin(BAUD);
  Serial.println("Starting...");

  // Connect WiFi
  Serial.println("Trying WiFi...");
  WiFi.begin(SSID, PASS);
  pinMode(LED_BUILTIN, OUTPUT);
  
  int cycle = 0;
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(500);
    if(cycle++ >= 50) { // 500ms * 50 = 25 seconds
      Serial.println("WiFi connection timed out.");
      for(;;);
    }
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("WiFi connected!");

  // Get Time
  configTime(GMT_OFFSET, DST_OFFSET, TIME_SERVER);
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get time.");
    for(;;);
  }

  //display.println(&timeinfo, "%I:%M:%S");
  // Initialize RGB LED Matrix Display
  HUB75_I2S_CFG mxconfig(
    PANEL_W,
    PANEL_H,
    PANEL_CHAIN
  );
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(90);
  dma_display->clearScreen();

}

void loop() {

  if(getLocalTime(&timeinfo)) {
    dma_display->clearScreen();
    dma_display->setTextSize(2);
    dma_display->setCursor(0,0);
    dma_display->setTextWrap(true);
    dma_display->setTextColor(0);
    dma_display->println(&timeinfo, "%I:%M:%S");
  }
  delay(1000);

}
