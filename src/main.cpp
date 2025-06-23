#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <time.h>
#include <WiFi.h>
#include <secrets.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

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

// // Weather Global Vars
// const char* WEATHER_SERVER_URL = "http://192.168.31.208:5001/api";

class WeatherInfo {
  public:
    String temp;
    String temp_max;
    String temp_min;
    String description;

    bool fetch(const char* url) {
      // HTTP Client
      HTTPClient http;
      http.begin(url);
      int httpCode = http.GET();
      if(httpCode > 0 ) {
        if(httpCode == HTTP_CODE_OK) {
          JsonDocument doc;
          DeserializationError err = deserializeJson(doc, http.getStream());

          if(!err) {
            temp           = doc["temp"].as<String>();
            description    = doc["description"].as<String>();
            temp_max       = doc["temp_max"].as<String>();
            temp_min       = doc["temp_min"].as<String>();
          } else {
            Serial.println("JSON parse failed: ");
            Serial.println(err.c_str());
            return false;
          }
        } else {
          Serial.printf("Server responded %d\n", httpCode);
          return false;
        } 
      } else {
        Serial.printf("HTTP GET failed: %s\n", http.errorToString(httpCode).c_str());
        return false;
      }
      http.end();

      return true;
    }
};

// Global weather obj declaration
WeatherInfo weather;

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
  Serial.println("\nWi-Fi connected. IP = " + WiFi.localIP().toString());

  // Get Time
  Serial.println("Getting time...");
  configTime(GMT_OFFSET, DST_OFFSET, TIME_SERVER);
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get time.");
    for(;;);
  }
  Serial.println("Time acquired");

  // Initialize RGB LED Matrix Display
  Serial.println("Initializing display...");
  HUB75_I2S_CFG mxconfig(
    MATRIX_WIDTH,
    MATRIX_HEIGHT,
    CHAIN_LENGTH
  );
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(50);
  dma_display->clearScreen();

  // Get weather info
  if(weather.fetch(SELF_HOSTED_API)) {
    Serial.println("Weather data fetch successful.");
  }
}

static uint32_t last = 0;
static uint32_t last_weather = 0;

void loop() {

  // ArduinoOTA.handle();

  if(getLocalTime(&timeinfo) && (millis() - last > 1000)) {
    last = millis();
    // Serial.println(&timeinfo, "%I:%M:%S");
    Serial.println(weather.temp);
    dma_display->clearScreen();
    dma_display->setTextSize(1);
    dma_display->setCursor(0,0);
    dma_display->setTextWrap(true);
    dma_display->color565(255, 255, 255);
    // dma_display->println(&timeinfo, "%I:%M:%S");
    dma_display->print(weather.temp);
    dma_display->println("F");
    dma_display->print("H:");
    dma_display->println(weather.temp_max);
    dma_display->print("L:");
    dma_display->println(weather.temp_min);
    dma_display->println(weather.description);
  }

}
