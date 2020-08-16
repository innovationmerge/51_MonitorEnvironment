//iNNovationMerge
//https://www.innovationmerge.com/
// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
// Declare hardware pins
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Imports 
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include "MCUFRIEND_kbv.h"

MCUFRIEND_kbv tft;

#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "FreeDefaultFonts.h"

// Code for DHT11 Temperature and humidity sensor.
#include "DHT.h"    // including the library of DHT11 temperature and humidity sensor
#define DHTPIN 12      // Selecting the pin at which we have connected DHT11
#define DHTTYPE DHT11 // Selecting the type of DHT sensors
DHT dht ( DHTPIN, DHTTYPE ) ;

void setup(void);
void loop(void);
void DisplayFont(void);


// Assign human-readable names to some common 16-bit color values:
//https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define DARKGREEN 0x33E3
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410
#define ORANGE  0xEC40

int sensorPin = A5;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
String LuxValue = "";

void setup(void) {
  Serial.begin(9600);             // Start Serial monitor
  dht.begin ( ) ;                 // DHT11 sensor will start working
  uint16_t ID = tft.readID();     // read TFT ID
  if (ID == 0xD3D3) ID = 0x9481;  //force ID if write-only display
  tft.begin(ID);                  //Start TFT at ID 
  tft.setRotation(1);             //Rotate display
  tft.fillScreen(WHITE);          //Fill white background to display
  WelcomeScreen();                //Call Welcome screen
  tft.fillScreen(WHITE);          //Clear and Fill white background and text to display
  DisplayTextString(100, 5, 3,DARKGREEN,"iNNovation");
  DisplayTextString(280, 5, 3,ORANGE,"Merge");
  DisplayTextString(50, 50, 2,ORANGE,"---> WORKPLACE MONITORING <---");
}

void loop(void) {
  // Get Lux value and display accordingly
  if(GetLuxValue()<500){
    LuxValue = "Light: Low!";
  }
  else if(GetLuxValue()>500 && GetLuxValue()<700){
    LuxValue = "Light: Perfect!";
  }
  else{
    LuxValue = "Light: High!";
  }
  // Get Humidity and Temperature value
  String humidity = "Humidity(%): "+String(dht.readHumidity ( )) ;  // Declaring h a variable and storing the humidity in it.
  String temp = "Temp(*C): "+String(dht.readTemperature ( )) ; // Declaring t a variable and storing the temperature in it.
  
 // Display text inside rectangle boxes
  tft.fillRect(10,80,460,60,MAGENTA);
  DisplayTextString(80, 100, 3,WHITE,LuxValue);
  tft.fillRect(10,160,460,60,DARKGREEN);
  DisplayTextString(80, 180, 3,WHITE,humidity);
  tft.fillRect(10,250,460,60,BLUE);
  DisplayTextString(80, 270, 3,WHITE,temp);
  delay(1000);
}
void WelcomeScreen(void){
  tft.drawFastHLine(5, 100, tft.width(), BLACK); // Print Horizontal black line 
  tft.drawFastHLine(5, 200, tft.width(), BLACK); // Print Horizontal black line 
  delay(100);                                    // Delay of 100ms
  DisplayTextString(20, 110, 3,BLACK,"Welcome To"); // Display text
  delay(100);                                    // Delay of 100ms
  DisplayTextString(50, 150, 4,DARKGREEN,"iNNovation");// Display text
  DisplayTextString(300, 150, 4,ORANGE,"Merge");    // Display text
  delay(1000);                                      // Delay of 100ms
}

void DisplayTextString(int x, int y, int sz, const char *color, String msg)
{
  tft.setTextColor(color);  //Set color
  tft.setTextSize(sz);      //Set Text Size to display
  tft.setCursor(x, y);      //Set Cursor to start printing
  tft.print(msg);           //Print message
}


float GetLuxValue()
{
  sensorValue = analogRead(sensorPin);  //Read from sensor pin declared
  float vout = sensorValue/204.6;       // Calculate voltage 
  float R = (11000-vout*200)/vout; // calculate the resistance
  float lux = 65.9 * (pow( sensorValue, 0.352));//lux calculation;
  return lux;
}
