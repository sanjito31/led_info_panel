#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

class Weather {
    public:
        // Constructors
        Weather();
        Weather(String temp, String maxTemp, String minTemp, String desc);

        // Main functions
        void    fetch(const char* url);
        void    printWeather(MatrixPanel_I2S_DMA* display);

        // Setters
        void    setTemp(String temp);
        void    setMaxTemp(String maxTemp);
        void    setMinTemp(String minTemp);
        void    setDescription(String desc);


        // Getters
        String  getTemp(void);
        String  getMaxTemp(void);
        String  getMinTemp(void);
        String  getDescription(void);
    
    private:
        String temp_;
        String maxTemp_;
        String minTemp_;
        String description_;
};