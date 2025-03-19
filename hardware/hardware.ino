// LIBRARY IMPORTS
#include <rom/rtc.h>    //for ESP32 dev board

#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "icons.h"

// DEFINITIONS
#define DHT_TYPE DHT22
#define DRY 3557 //Originally 3600, I might have to lower it
#define WET 1457
#define TFT_BG ILI9341_BLACK //ILI9341_NAVY
#define TFT_BOX ILI9341_BLUE
#define TFT_OUTLINE ILI9341_YELLOW
#define TFT_TXT ILI9341_WHITE
#define ICON_SIZE 36

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

//Box size definitions
#define BOX_X      5
#define BOX_Y      60
#define BOX_WIDTH  310
#define BOX_HEIGHT 30
#define SPACING    35  // Vertical spacing between boxes
#define RADIUS     10

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

// Variable to hold icons
const uint16_t* tempIcon = sun2;
const uint16_t* weatherIcon = sun1;

// Object Initialization
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

// Function Declarations
void sensorSetup();
void displaySetup();
SensorData readSensors();
void displayDataSerial(SensorData data);
void displayTFT(SensorData data);
void drawWeatherIcon(float humidity, float pressure, float temp, int x, int y);
void drawTempIcon(float temperature, int x, int y);
void drawBox(int x, int y, String label, float value, String unit);
void drawStars();


void setup() {
    Serial.begin(115200);
    sensorSetup();
    displaySetup();
}

void loop() {
    SensorData info = readSensors();
    displayDataSerial (info);
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

    tft.fillScreen(TFT_BG);
    tft.setTextColor(TFT_TXT);
    tft.setTextSize(2);

    drawStars();  // Add stars!!
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

void displayDataSerial(SensorData data){
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
    if(!data.isValid){
        data.cTemp = data.humidity = data.pressure = data.moisturePercent = 0;
    }
    
    
    drawWeatherIcon(data.humidity, data.pressure, data.cTemp, 30, 10); // Show weather icon
    drawTempIcon(data.cTemp, 254, 10); //might change
    tft.setTextColor(TFT_TXT);

    // Display temperature
    drawBox(BOX_X, BOX_Y, "Temperature:   ", data.cTemp, "C");

    // Display humidity
    drawBox(BOX_X, BOX_Y + SPACING, "Humidity:      ", data.humidity, "%");

    // Display pressure
    drawBox(BOX_X, BOX_Y + 2 * SPACING, "Pressure:      ", data.pressure, "hPa");

    // Display altitude
    drawBox(BOX_X, BOX_Y + 3 * SPACING, "Altitude:      ", data.altitude, " m");

    // Display soil moisture
    drawBox(BOX_X, BOX_Y + 4 * SPACING, "Soil Moisture: ", data.moisturePercent, "%");
}

void drawBox(int x, int y, String label, float value, String unit) {
  tft.fillRoundRect(x+1, y+1, BOX_WIDTH-2, BOX_HEIGHT-2, RADIUS, TFT_BOX);  // Dark blue background for the box
  tft.drawRoundRect(x, y, BOX_WIDTH, BOX_HEIGHT, RADIUS, TFT_OUTLINE); // White border for the box
  tft.setCursor(x + 8, y + 7); // Add some padding
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  tft.print(label);
  tft.print(value); // Display the value (e.g., "25.6")
  tft.print(unit); // Display the value (e.g., "°C")
}

void drawWeatherIcon(float humidity, float pressure, float temp, int x, int y) {
    const uint16_t* icon; 

    if (humidity < 65 && pressure > 990) {
        icon = sun2;       // ☀️ Sunny
    } else if (humidity >= 65 && humidity <= 80 && pressure >= 990) {
        icon = partlyCloud; // 🌤️ Partly Cloudy
    } else if (humidity > 80 && pressure < 990) {
        icon = cloud;     // ☁️/🌧️ Cloudy or Rainy
    } else if (humidity > 90 && temp < 20) {
        icon = foggy;     // 🌫️ Foggy
    } else {
        icon = sun2;      // Default to ☀️
    }
    // return "❓";  // Unknown condition

    // Draw the corresponding icon
    if (icon != weatherIcon) {
        tft.fillRect(x, y, ICON_SIZE, ICON_SIZE, TFT_BG);
        tft.drawRGBBitmap(x, y, icon, ICON_SIZE, ICON_SIZE);
    }
    weatherIcon = icon;
}

void drawTempIcon(float temperature, int x, int y) {
    const uint16_t* icon; 

    if (temperature < 20) {
        icon = cold;        // 🥶 Cool
    } else if (temperature >= 20 && temperature < 25) {
        icon = partlyCloud; // 🌤️ Warm
    } else if (temperature >= 25 && temperature < 30) {
        icon = sun1;        // 🌞 Hot
    } else {
        icon = fire;        // 🔥 Very Hot
    // } else {
    //     icon = sun1;      // Default to ☀️
    }
    // return "❓";  // Unknown condition

    // Draw the corresponding icon
    if (icon != tempIcon) {
        tft.fillRect(x, y, ICON_SIZE, ICON_SIZE, TFT_BG);
        tft.drawRGBBitmap(x, y, icon, ICON_SIZE, ICON_SIZE);
    }
    tempIcon = icon;
}

void drawStars() {
    for (int i = 0; i < 100; i++) {
        int x = random(tft.width());
        int y = random(tft.height());
        tft.drawPixel(x, y, ILI9341_WHITE);  // White stars
    }
}
