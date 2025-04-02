// Soil moisture sensor and dht aint working
// LIBRARY IMPORTS
#include <rom/rtc.h>    //for ESP32 dev board

#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "icons.h"

// More includes
#ifndef _WIFI_H 
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif 
 
#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif

// DEFINITIONS
#define DHT_TYPE DHT22
#define DRY 3357 //Originally 3600, I might have to lower it
#define WET 1457
#define TFT_BOX ILI9341_BLUE
#define TFT_OUTLINE ILI9341_YELLOW
#define TFT_TXT ILI9341_WHITE

// Pin Definitions
#define DHT_PIN 33      //14
#define MOISTURE_PIN 35 //34

#define TFT_CS   5      //16
#define TFT_RST  16     //17
#define TFT_DC   17     //5
#define TFT_MOSI 23     //23
#define TFT_SCK  18     //19
//#define TFT_LED  25
#define TFT_MISO 19     //18

#define SW1 32
// #define SW2 33

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

// Variable to hold sensor data and icons
SensorData sensorInfo;
SensorData pastInfo = {NAN, NAN, NAN, NAN, NAN, NAN, true};
const uint16_t* tempIcon = sun2;
const uint16_t* weatherIcon = sun1;
uint16_t TFT_BG = ILI9341_DARKGREEN; 
uint16_t TFT_NBG = ILI9341_BLACK; //used to be ILI9341_NAVY
uint16_t TFT_DBG = ILI9341_DARKGREEN;
bool isDayMode = true;
bool isCelsius = true;

// Object Initialization
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MQTT CLIENT CONFIG  
static const char* pubtopic      = "620162297";                    // Add your ID number here
static const char* subtopic[]    = {"620162297_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server   = "www.yanacreations.com";         // Broker IP address or Domain name as a String 
static uint16_t mqtt_port        = 1883;

// WIFI CREDENTIALS
const char* ssid       = "Robert"; // Add your Wi-Fi ssid //MonaConnect
const char* password   = "paigeee:)"; // Add your Wi-Fi password 

// // TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL;  

// // FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
// void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters );  
bool isNumber(double number);

#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif
////////////////////////////////////////////////////////////////////////////////drawWeatherIcon///////////////////////////////////////


// Function Declarations
void sensorSetup();
void displaySetup();
SensorData readSensors();
void displayDataSerial(SensorData data);
void displayTFT(SensorData data);
void drawWeatherIcon(float humidity, float pressure, float temp, int x, int y);
void drawTempIcon(float temperature, int x, int y);
void drawBox(int x, int y, String label);
void updateBox(int x, int y, float value, String unit);
void drawStars();
void updateBackground();

void setup() {
    Serial.begin(115200);
    Serial.println("Starting setup...");

    pinMode(SW1, INPUT_PULLUP);

    sensorSetup();
    Serial.println("Sensor setup complete...");

    displaySetup();
    Serial.println("Display setup complete...");

    initialize();
    Serial.println("Initialization done...");

    Serial.println("Setup complete!");
}


void loop() {
    sensorInfo = readSensors();
    displayDataSerial (sensorInfo);
    displayTFT(sensorInfo);

    vTaskDelay(1000 / portTICK_PERIOD_MS);  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vButtonCheck( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      
    for( ;; ) {
        // Add code here to check if a button(S) is pressed
        // then execute appropriate function if a button is pressed  
        if (digitalRead(SW1) == LOW) { 
          vTaskDelay(40); // Debounce delay
          Serial.println("Temperature button pressed.");
          isCelsius = !isCelsius;
          displayTemp(sensorInfo.cTemp);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);  
    }
}

void vUpdate( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
           
    for( ;; ) {
          // #######################################################
          // ## This function must PUBLISH to topic every second. ##
          // #######################################################

  	      // if (isNumber(dht.readTemperature())){
          if (sensorInfo.isValid){
              // 1. Create JSon object
              StaticJsonDocument<1000> doc;
              
              // 2. Create message buffer/array to store serialized JSON object
              char message[1100]  = {0};
              
              // 3. Add key:value pairs to JSon object based on above schema
              doc["id"]                   = "620162297";
              doc["timestamp"]            = getTimeStamp();
              doc["temperature"]          = sensorInfo.cTemp;
              doc["humidity"]             = sensorInfo.humidity;
              doc["heatIndex"]            = sensorInfo.heatIndex;
              doc["altitude"]             = sensorInfo.altitude;
              doc["pressure"]             = sensorInfo.pressure;
              doc["soilmoisturePercent"]  = sensorInfo.moisturePercent;

              // 4. Seralize / Covert JSon object to JSon string and store in message array
              serializeJson(doc, message);
               
              // 5. Publish message to a topic sobscribed to by both backend and frontend
              if (mqtt.connected()) {
                publish(pubtopic, message);
              }
          }
          

          struct tm timeinfo;
          if (!getLocalTime(&timeinfo)) {
              Serial.println("Failed to obtain time");
          } else {
            int currentHour = timeinfo.tm_hour;
            Serial.print("Current Hour: "); Serial.println(currentHour);

            // Determine day or night mode
            bool wasDayMode = isDayMode;
            isDayMode = (currentHour > 6 && currentHour <= 18);
            Serial.print("isDayMode: ");
            Serial.println(isDayMode);

            if (wasDayMode != isDayMode){
              // Update TFT background
              TFT_BG = isDayMode ? TFT_DBG : TFT_NBG;
              updateBackground();
              Serial.print("TFT Background Updated: ");
              Serial.println(isDayMode ? "Day Mode" : "Night Mode");
              pastInfo = {NAN, NAN, NAN, NAN, NAN, NAN, true};
            }
          }
          
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}               

unsigned long getTimeStamp(void) {
    // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
    time_t now;         
    time(&now); // Retrieve time[Timestamp] from system and save to &now variable
    return now;
}

void callback(char* topic, byte* payload, unsigned int length) {
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO

  Serial.printf("\nMessage received : ( topic: %s ) \n", topic);
  char* received = new char[length + 1]{ 0 };

  for (int i = 0; i < length; i++) {
    received[i] = (char)payload[i];
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n", received);

  // CONVERT MESSAGE TO JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, received); 

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
}

bool publish(const char *topic, const char *payload){   
    bool res = false;
    try{
        res = mqtt.publish(topic,payload);
        // Serial.printf("\nres : %d\n",res);
        if(!res){
            res = false;
            throw false;
        }
    }
    catch(...){
        Serial.printf("\nError (%d) >> Unable to publish message\n", res);
    }
    return res;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// My supplementary functions
void sensorSetup() {
    //DHT setup
    dht.begin();
    
    //BMP setup
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

    updateBackground();
    tft.setTextColor(TFT_TXT);
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
    int moistureValue = analogRead(MOISTURE_PIN);
    data.moisturePercent = map(moistureValue, DRY, WET, 0, 100);
    data.moisturePercent = constrain(data.moisturePercent, 0, 100);

    data.isValid = true;

    //Ensures readings logged are valid
    if (isnan(data.humidity)||isnan(data.cTemp)||(data.humidity==0)||(data.cTemp==0)){
        Serial.println("Failed to read from DHT sensor.");
        Serial.println("Reading: ");
        Serial.print("Humidity: ");
        Serial.println(data.humidity);
        Serial.print("Temp: ");
        Serial.println(data.cTemp);
        data.isValid = false;
    } if (isnan(data.pressure)||isnan(data.altitude)) {
        Serial.println("Failed to read from BMP sensor.");
        Serial.println("Reading: ");
        Serial.print("Altitude: ");
        Serial.println(data.altitude);
        Serial.print("Pressure: ");
        Serial.println(data.pressure);
        data.isValid = false;
    } if ((data.moisturePercent>DRY)||(data.moisturePercent<WET)) {
        Serial.println("Failed to read from Capacitive Soil Moisture sensor.");
        Serial.println("Reading: ");
        Serial.print("Moisture Percent: ");
        Serial.println(data.moisturePercent);
        data.isValid = false;
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
        data.cTemp = data.humidity = data.pressure = data.altitude = data.moisturePercent = -100;
    }
    
    drawWeatherIcon(data.humidity, data.pressure, data.cTemp, 30, 10); // Show weather icon
    drawTempIcon(data.cTemp, 254, 10); //might change
    
    tft.setTextColor(TFT_TXT);
    tft.setTextSize(2);

    float roundPressure = round(data.pressure*100)/100;
    float roundPastPressure = round(pastInfo.pressure*100)/100;
    float roundAltitude = round(data.altitude*100)/100;
    float roundPastAltitude = round(pastInfo.altitude*100)/100;

    // Display temperature
    if (data.cTemp != pastInfo.cTemp) {
      displayTemp(data.cTemp);
      pastInfo.cTemp = data.cTemp;
    }

    // Display humidity
    if (data.humidity != pastInfo.humidity){
      updateBox(BOX_X, BOX_Y + SPACING, data.humidity, "%");
      pastInfo.humidity = data.humidity;
    }

    // Display pressure
    if (roundPressure != roundPastPressure){
      updateBox(BOX_X, BOX_Y + 2 * SPACING, data.pressure, "hPa");
      pastInfo.pressure = data.pressure;
    }

    // Display altitude
    if (roundAltitude != roundPastAltitude){
      updateBox(BOX_X, BOX_Y + 3 * SPACING, data.altitude, "m");
      pastInfo.altitude = data.altitude;
    }

    // Display soil moisture
    if (data.moisturePercent != pastInfo.moisturePercent){
      updateBox(BOX_X, BOX_Y + 4 * SPACING, data.moisturePercent, "%");
      pastInfo.moisturePercent = data.moisturePercent;
    }

    if (data.heatIndex != pastInfo.heatIndex){
      pastInfo.heatIndex = data.heatIndex;
    }
}

void displayTemp(float cTemp){
  float temp = isCelsius ? cTemp : (cTemp*9.0 / 5.0) + 32;
  String unit = isCelsius ? "C" : "F"; 
  updateBox(BOX_X, BOX_Y, temp, unit);
}

void updateBackground(){
  tft.fillScreen(TFT_BG);
  drawStars();  // Add stars!!
  if (isDayMode) {
    tft.fillRect(0, 0, tft.width(), 50, ILI9341_BLACK);
    tft.fillRect(0, 51, tft.width(), 2, ILI9341_WHITE);
  }

  tft.setTextColor(TFT_TXT);
  tft.setTextSize(2);

  // Display temperature
  drawBox(BOX_X, BOX_Y, "Temperature:   ");

  // Display humidity
  drawBox(BOX_X, BOX_Y + SPACING, "Humidity:      ");

  // Display pressure
  drawBox(BOX_X, BOX_Y + 2 * SPACING, "Pressure:      ");

  // Display altitude
  drawBox(BOX_X, BOX_Y + 3 * SPACING, "Altitude:      ");

  // Display soil moisture
  drawBox(BOX_X, BOX_Y + 4 * SPACING, "Soil Moisture: ");
  
  tempIcon = sun2; //Changing it to this will make it reset when it gets to the function
  weatherIcon = sun1;
}

void drawBox(int x, int y, String label) {
  tft.fillRoundRect(x+1, y+1, BOX_WIDTH-2, BOX_HEIGHT-2, RADIUS, TFT_BOX);  // Dark blue background for the box
  tft.drawRoundRect(x, y, BOX_WIDTH, BOX_HEIGHT, RADIUS, TFT_OUTLINE); // Yellow border for the box
  tft.setCursor(x + 8, y + 7); // Add some padding
  tft.setTextColor(TFT_TXT);
  tft.setTextSize(2);

  tft.print(label);
}

void updateBox(int x, int y, float value, String unit) {
  tft.setCursor(x + 180, y + 7); // Add some padding
  tft.fillRect(x+180, y+7, 100, BOX_HEIGHT-10, TFT_BOX); // Clear previous value

  tft.print(value); // Display the value (e.g., "25.6")
  tft.print(unit); // Display the value (e.g., "°C")
}

void drawWeatherIcon(float humidity, float pressure, float temp, int x, int y) {
    const uint16_t* icon; 

    if (humidity < 65 && pressure > 990) {
        icon = sun2;        // ☀️ Sunny
    } else if (humidity >= 65 && humidity <= 80 && pressure >= 990) {
        icon = partlyCloud; // 🌤️ Partly Cloudy
    } else if (humidity > 80 && pressure < 990) {
        icon = cloud;       // ☁️/🌧️ Cloudy or Rainy
    } else if (humidity > 90 && temp < 20) {
        icon = foggy;       // 🌫️ Foggy
    } else {
        icon = sun2;        // Default to ☀️
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

bool isNumber(double number){       
    char item[20];
    snprintf(item, sizeof(item), "%f\n", number);
    if( isdigit(item[0]) )
      return true;
    return false; 
}