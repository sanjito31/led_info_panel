#include "weather.hpp"

// No arg Constructor
Weather::Weather(){
    setTemp("NA");
    setMaxTemp("NA");
    setMinTemp("NA");
    setDescription("NA");
}

// Constructor with args
Weather::Weather(String temp, String maxTemp, String minTemp, String desc) {
    setTemp(temp);
    setMaxTemp(maxTemp);
    setMinTemp(minTemp);
    setDescription(desc);
}

// fetch weather via URL and update internal variables
void Weather::fetch(const char* url) {
    // HTTP Client
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if(httpCode > 0 ) {
        if(httpCode == HTTP_CODE_OK) {
            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, http.getStream());

            if(!err) {
                this->setTemp(doc["temp"].as<String>());
                this->setDescription(doc["description"].as<String>());
                this->setMaxTemp(doc["temp_max"].as<String>());
                this->setMinTemp(doc["temp_min"].as<String>());
            } else {
                throw std::invalid_argument(std::string("JSON parse failed: ") + err.c_str());
                // Serial.println("JSON parse failed: ");
                // Serial.println(err.c_str());
                // return false;
            }
        } else {
            throw std::runtime_error(std::string("HTTP status error: ") + std::to_string(httpCode));
            // Serial.printf("Server responded %d\n", httpCode);
            // return false;
        } 
    } else {
        throw std::system_error(std::make_error_code(std::errc::network_unreachable),
        "HTTP GET failed at network layer");
        // Serial.printf("HTTP GET failed: %s\n", http.errorToString(httpCode).c_str());
        // return false;
    }
    http.end();
}

void Weather::printWeather(MatrixPanel_I2S_DMA* display) {

    display->print(this->getTemp());
    display->println("F");
    
    display->print("H: ");
    display->print(this->getMaxTemp());
    display->println("F");

    display->print("L: ");
    display->print(this->getMinTemp());
    display->println("F");

    display->setTextWrap(false);
    display->println(this->getDescription());
}

// Setters
void    Weather::setTemp(String temp) {
    this->temp_ = temp;
}

void    Weather::setMaxTemp(String maxTemp){
    this-> maxTemp_ = maxTemp;
}

void    Weather::setMinTemp(String minTemp){
    this->minTemp_ = minTemp;
}

void    Weather::setDescription(String desc){
    this->description_ = desc;
}


// Getters
String  Weather::getTemp(void) {
    return temp_;
}

String  Weather::getMaxTemp(void) {
    return maxTemp_;
}

String  Weather::getMinTemp(void) {
    return minTemp_;
}
String  Weather::getDescription(void) {
    return description_;
}