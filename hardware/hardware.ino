// LIBRARY IMPORTS
#include <rom/rtc.h>    //for ESP32 dev board

#include <DHT.h>

// DEFINITIONS
#define DHT_TYPE DHT22

// Pin Definitions
#define DHT_PIN 14

// Structure for sensor readings
struct SensorData
{
    float cTemp;
    float humidity;
    float heatIndex;
    bool isValid;
};

// Function Declarations
SensorData readSensors();
void displayData( SensorData data);

// Object Initialization
DHT dht(DHT_PIN, DHT_TYPE);


void setup() {
    Serial.begin(115200);

    dht.begin();
}

void loop() {
    SensorData info = readSensors();
    displayData (info);

    delay(2000);
}

SensorData readSensors() {
    SensorData data;
    data.humidity = dht.readHumidity();
    data.cTemp = dht.readTemperature(); //Temperature in Celsius
    data.heatIndex = dht.computeHeatIndex(data.cTemp, data.humidity, false);

    //Ensures readings logged are valid
    if (isnan(data.humidity)|| isnan(data.cTemp)){
        data.isValid = false;
    } else {
        data.isValid = true;
    }

    return data;
}

void displayData(SensorData data){
    if(!data.isValid){
        Serial.println("Failed to read from DHT sensor.");
        return;
    }
    
    Serial.print("Temperature: ");
    Serial.print(data.cTemp);
    Serial.print(" °C, Humidity: ");
    Serial.print(data.humidity);
    Serial.print(" %, Heat Index: ");
    Serial.print(data.heatIndex);
    Serial.println(" °C");
}