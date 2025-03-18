// LIBRARY IMPORTS
#include <rom/rtc.h>    //for ESP32 dev board

#include <DHT.h>
#include <Adafruit_BMP280.h>

// DEFINITIONS
#define DHT_TYPE DHT22
#define MOISTURE_MAX 3600 //I might have to lower it
#define MOISTURE_MIN 1457

// Pin Definitions
#define DHT_PIN 14
#define MOISTURE_PIN 34

// Object Initialization
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;

// Structure for sensor readings
struct SensorData
{
    float cTemp;            //DHT
    float humidity;         //DHT
    float heatIndex;        //DHT
    float pressure;         //BMP
    float altitude;         //BMP
    float moisturePercent;  //Capacitive Soil Moisture Sensor
    bool  isValid;          //to tell if the data retrieved is valid
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
    
    pinMode(MOISTURE_PIN, INPUT);
}

void loop() {
    SensorData info = readSensors();
    displayData (info);

    delay(1000);
}

SensorData readSensors() {
    SensorData data;
    data.humidity = dht.readHumidity();
    data.cTemp = dht.readTemperature(); //Temperature in Celsius
    data.heatIndex = dht.computeHeatIndex(data.cTemp, data.humidity, false);
    data.pressure = bmp.readPressure() / 100.0; // Convert from Pa to hPa
    data.altitude = bmp.readAltitude(1013.25);  // Standard pressure at sea level

    // data.moisturePercent = analogRead(MOISTURE_PIN);
    int moistureValue = analogRead(MOISTURE_PIN);
    data.moisturePercent = map(moistureValue, 3600, 1457, 0, 100);

    //Ensures readings logged are valid
    if (isnan(data.humidity)||isnan(data.cTemp)){
        Serial.println("Failed to read from DHT sensor.");
        data.isValid = false;
    } else if (isnan(data.pressure)||isnan(data.altitude)) {
        Serial.println("Failed to read from BMP sensor.");
        data.isValid = false;
    // } else if ((data.moisturePercent>MOISTURE_MAX)||(data.moisturePercent<MOISTURE_MIN)) {
    //     Serial.println("Failed to read from Capacitive Soil Moisture sensor.");
    //     data.isValid = false;
    } else {
        data.isValid = true;
    }

    return data;
}

void displayData(SensorData data){
    if(!data.isValid){
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
    Serial.print(" m, Soil Moisture: ");
    Serial.print(data.moisturePercent);
    Serial.println("%");
    Serial.println("");
}