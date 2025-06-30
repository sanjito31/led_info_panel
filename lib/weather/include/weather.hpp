#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class Weather {
    public:
        // Constructors
        Weather();
        Weather(String temp, String maxTemp, String minTemp, String desc);

        // Main functions
        void    fetch(const char* url);

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