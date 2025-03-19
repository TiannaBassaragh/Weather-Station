// LIBRARY IMPORTS
#include <rom/rtc.h>    //for ESP32 dev board

#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// DEFINITIONS
#define DHT_TYPE DHT22
#define DRY 3600 //I might have to lower it
#define WET 1457

// Pin Definitions
#define DHT_PIN 14
#define MOISTURE_PIN 34

#define TFT_CS   16
#define TFT_RST  17
#define TFT_DC   5
#define TFT_MOSI 23
#define TFT_SCK  19
#define TFT_LED  25
#define TFT_MISO 18

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

// Object Initialization
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

// Function Declarations
void sensorSetup();
void displaySetup();
SensorData readSensors();
void displayDataLocal(SensorData data);
String getWeatherIcon(float temperature);
void dipsplayTFT(SensorData data);


void setup() {
    Serial.begin(115200);
    sensorSetup();
    displaySetup();
}

void loop() {
    SensorData info = readSensors();
    displayDataLocal (info);
    displayTFT(info);

    delay(1000);
}

void sensorSetup() {
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

void displaySetup() {
    tft.begin();
    tft.setRotation(3);  // Adjust orientation if needed
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
}

SensorData readSensors() {
    SensorData data;

    //DHT22 sensor
    data.humidity = dht.readHumidity();
    data.cTemp = dht.readTemperature(); //Temperature in Celsius
    data.heatIndex = dht.computeHeatIndex(data.cTemp, data.humidity, false);
    
    //BMP280 sensor
    data.pressure = bmp.readPressure() / 100.0; // Convert from Pa to hPa
    data.altitude = bmp.readAltitude(1013.25);  // Standard pressure at sea level

    //Soil mositure sensor
    // data.moisturePercent = analogRead(MOISTURE_PIN);
    int moistureValue = analogRead(MOISTURE_PIN);
    data.moisturePercent = map(moistureValue, DRY, WET, 0, 100);
    data.moisturePercent = constrain(data.moisturePercent, 0, 100);

    //Ensures readings logged are valid
    if (isnan(data.humidity)||isnan(data.cTemp)||(data.humidity==0)||(data.cTemp==0)){
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

void displayDataLocal(SensorData data){
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

void displayTFT(SensorData data){
    tft.fillScreen(ILI9341_BLACK);  // Clear screen
    tft.setCursor(10, 20);
    
    tft.setTextColor(ILI9341_CYAN);
    tft.print("Temp: ");
    tft.setTextColor(ILI9341_WHITE);
    tft.print(data.cTemp);
    tft.println(" C");

    tft.setTextColor(ILI9341_CYAN);
    tft.print("Humidity: ");
    tft.setTextColor(ILI9341_WHITE);
    tft.print(data.humidity);
    tft.println(" %");

    tft.setTextColor(ILI9341_CYAN);
    tft.print("Pressure: ");
    tft.setTextColor(ILI9341_WHITE);
    tft.print(data.pressure);
    tft.println(" hPa");

    tft.setTextColor(ILI9341_CYAN);
    tft.print("Altitude: ");
    tft.setTextColor(ILI9341_WHITE);
    tft.print(data.altitude);
    tft.println(" m");

    tft.setTextColor(ILI9341_CYAN);
    tft.print("Moisture: ");
    tft.setTextColor(ILI9341_WHITE);
    tft.print(data.moisturePercent);
    tft.println(" %");
}

String getWeatherIcon(float temperature){  // Function to get the weather icon based on temperature range
    if (temperature < 20) {
        return "🌬️";  // Cool
    } else if (temperature >= 20 && temperature < 25) {
        return "🌤";  // Warm
    } else if (temperature >= 25 && temperature < 30) {
        return "🌞";  // Hot
    } else {
        return "🔥";  // Very Hot
    }
}