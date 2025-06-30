#include "mta.hpp"

// SubwayLine
SubwayLine::SubwayLine(const String& name)
    : lineName_(name)
{}

// Setters
void                SubwayLine::setLineName(String name) { lineName_ = name; }
void                SubwayLine::setStationName(String name) { stationName_ = name; }

void                SubwayLine::addUptownWait(int wait) { uptownWaits_.push_back(wait); }
void                SubwayLine::addDowntownWait(int wait) { downtownWaits_.push_back(wait); }
void                SubwayLine::clearUptownWaits() { uptownWaits_.clear(); }
void                SubwayLine::clearDowntownWaits() { downtownWaits_.clear(); }

// Getters
String&             SubwayLine::getLineName() { return lineName_; }
String&             SubwayLine::getStationName() { return stationName_; }
std::vector<int>&   SubwayLine::getUptownWaits() { return uptownWaits_; }
std::vector<int>&   SubwayLine::getDowntownWaits() { return downtownWaits_; }

// MTA

SubwayLine& MTA::getLine(const String& name) {
    return lines_.at(name);  // throws if missing
}


void MTA::fetch(const char* url) {
    // HTTP Client
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if(httpCode > 0 ) {
        if(httpCode == HTTP_CODE_OK) {
            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, http.getStream());
            // JsonObject root = doc.as<JsonObject>();

            if(!err) {
                // this->setTemp(doc["temp"].as<String>());

                for(auto& sl : lines_) {
                    const String name = sl.first;
                    SubwayLine& line = sl.second;

                    JsonObject js = doc[name].as<JsonObject>();

                    // Set line name
                    line.setLineName(name);
                    // Set line station
                    line.setStationName(js["station"].as<String>());
                    // Clear previous and add new uptown waits as ints
                    line.clearUptownWaits();
                    for(int v: js["uptown_waits"].as<JsonArray>()) { line.addUptownWait(v); }
                    // Clear previous and add new downtown waits as ints
                    line.clearDowntownWaits();
                    for(int v: js["downtown_waits"].as<JsonArray>()) { line.addDowntownWait(v); }

                }
                
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

    return;
}