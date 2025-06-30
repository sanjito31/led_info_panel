#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <time.h>
#include <WiFi.h>
#include <secrets.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <weather.hpp>
#include <mta.hpp>

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



// Global weather obj declaration
Weather weather;

// Global MTA obj declaration
MTA mta;
SubwayLine Fline("F");
SubwayLine Mline("M");
SubwayLine Bline("B");
SubwayLine Dline("D");

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

  // ************ INITIALIZE ALL API CALL OBJECTS HERE ************ //

  // ----------- Get Time ----------- //
  Serial.println("Getting time...");
  configTime(GMT_OFFSET, DST_OFFSET, TIME_SERVER);
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get time.");
    for(;;);
  }
  Serial.println("Time acquired");

  // ----------- Get Weather ----------- //
  const char* weatherUrl = "http://192.168.31.205:8000/api/weather";

  try {
    weather.fetch(weatherUrl);
    Serial.println("Weather data fetch successful.");
  } catch(const std::system_error &e) {
    Serial.print("Network error");
    Serial.println(e.what());
    for(;;);
  } catch(const std::runtime_error&e) {
    Serial.println(e.what());
    for(;;);
  } catch(const std::invalid_argument&e) {
    Serial.println(e.what());
    for(;;);
  }

  // ----------- Get MTA ----------- //
  const char* mtaUrl = "http://192.168.31.205:8000/api/mta";

  try {
    mta.fetch(mtaUrl);
    Serial.println("MTA data fetch successful.");
  } catch(const std::system_error &e) {
    Serial.print("Network error");
    Serial.println(e.what());
    for(;;);
  } catch(const std::runtime_error&e) {
    Serial.println(e.what());
    for(;;);
  } catch(const std::invalid_argument&e) {
    Serial.println(e.what());
    for(;;);
  }

  Fline = mta.getLine("F");
  Mline = mta.getLine("M");
  Bline = mta.getLine("B");
  Dline = mta.getLine("D");



  // ----------- Get Spotify ----------- //


  // ----------- Get F1 ----------- //
  // const char* f1Url = "http://192.168.31.205:8000/api/f1/drivers";

}

static uint32_t last = 0;
static uint32_t last_weather = 0;

void loop() {

  // ArduinoOTA.handle();

  if(getLocalTime(&timeinfo) && (millis() - last > 1000)) {

    last = millis();

    dma_display->clearScreen();
    dma_display->setTextSize(1);
    dma_display->setCursor(0,0);
    dma_display->setTextWrap(true);
    dma_display->color565(255, 255, 255);



    // if(millis() - ) {

    // }

    // if(millis() -)

    // Fline.printSubwayLine(dma_display);
    weather.printWeather(dma_display);

  }

}
