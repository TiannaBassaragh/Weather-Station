// LIBRARY IMPORTS
#include <rom/rtc.h>    //for ESP32 dev board

#include <DHT.h>
#include <Adafruit_BMP280.h>

// DEFINITIONS
#define DHT_TYPE DHT22

// Pin Definitions
#define DHT_PIN 14

// Object Initialization
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;

// Structure for sensor readings
struct SensorData
{
    float cTemp;        //DHT
    float humidity;     //DHT
    float heatIndex;    //DHT
    float pressure;     //BMP
    float altitude;     //BMP
    bool isValid;       //to tell if the data retrieved is valid
};

// Function Declarations
SensorData readSensors();
void displayData(SensorData data);



void setup() {
    Serial.begin(115200);

    dht.begin();

    if (!bmp.begin(0x76)) {
        Serial.println("Could not find valid BMP280 sensor.");
        while(1);
    }

    bmp.setSampling(
        Adafruit_BMP280::MODE_NORMAL, 
        Adafruit_BMP280::SAMPLING_X16,      // Most accurate temperature reading
        Adafruit_BMP280::SAMPLING_X16,      // Best pressure accuracy
        Adafruit_BMP280::FILTER_X16,        // Best smoothing to ignore small fluctuations in the air
        Adafruit_BMP280::STANDBY_MS_500);   // Able to update every second
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
    data.pressure = bmp.readPressure() / 100.0; // Convert from Pa to hPa
    data.altitude = bmp.readAltitude(1013.25);  // Standard pressure at sea level

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
    Serial.print(" °C, Pressure: ");
    Serial.print(data.pressure);
    Serial.print(" hPa, Altitude: ");
    Serial.print(data.altitude);
    Serial.println(" m");
}