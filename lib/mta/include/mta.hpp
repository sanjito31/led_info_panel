#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <map>

class SubwayLine {
    public:
        // Constructor
        explicit SubwayLine(const String& name);

        // Setters
        void                setLineName(String name);
        void                setStationName(String name);

        // Setters for Arrays (Add and Clears)
        void                addUptownWait(int wait);
        void                addDowntownWait(int wait);
        void                clearUptownWaits();
        void                clearDowntownWaits();

        // Getters
        String&             getLineName();
        String&             getStationName();
        std::vector<int>&   getUptownWaits();
        std::vector<int>&   getDowntownWaits();


    private:
        String              lineName_;
        String              stationName_;
        std::vector<int>    uptownWaits_;
        std::vector<int>    downtownWaits_;

};

class MTA {
    public:
        // Constructor
        MTA()
            : lines_{
                {"B", SubwayLine("B")},
                {"D", SubwayLine("D")},
                {"F", SubwayLine("F")},
                {"M", SubwayLine("M")}
            }
        {}

        // Main fetch method, api call done here
        void fetch(const char* url);

        // getter
        SubwayLine& getLine(const String& name);
    
    private:
        std::map<String, SubwayLine> lines_;

};