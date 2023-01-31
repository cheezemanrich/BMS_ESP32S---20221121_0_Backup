//
// "ESP32 Dev Kit C V4" Version
// VS Code and PlatformIO version
//
// Current pinout:
// ******************************
// RS485 Board RO: GIOP16 (RX2) (Through 3.3V-5V logic translator)
// RS485 Board RE: GIOP4        (Through 3.3V-5V logic translator)
// RS485 Board DE: GIOP4        (Through 3.3V-5V logic translator)
// RS485 Board DI: GIOP17 (TX2) (Through 3.3V-5V logic translator)
// RS485 Board VCC: 5V and Battery Cable Pin 5
// RS485 Board GND: Gnd and Battery Cable Pin 2
// RS485 Board A: Battery Cable Pin 3
// RS485 Board B: Battery Cable Pin 4
// 
// Charge Enable Output: 14 (ON/OFF)
// Load Enable Output: 27 (ON/OFF)
// Charge Enable Output: 26 (PWM, 50% duty relay hold)
// Load Enable Output: 25 (PWM, 50% duty relay hold)
// 
// ILI9341 Pin 1: 5V
// ILI9341 Pin 2: Gnd
// ILI9341 Pin 3: GPIO13 (CS)
// ILI9341 Pin 4: 3.3V
// ILI9341 Pin 5: GPIO12 (DC)
// ILI9341 Pin 6: GPIO23 (MOSI/SDI)
// ILI9341 Pin 7: GPIO18 (SCK)
// ILI9341 Pin 8: 10k resistor to 3.3V (Backlight)
// ILI9341 Pin 9: GPIO19 (MISO/SDO)
// ILI9341 Pins 10-14 Not Used
//
// Debug timing pin: GPIO15
// 

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <SPIFFS.h>
#include <CayenneMQTTESP32.h>

// Replace with your network credentials
const char* ssid = "Your WiFi SSID";
const char* password = "Your WiFi Password";

//Cayenne MQTT Info
char mqtt_username[] = "Your MQTT Info";
char mqtt_password[] = "Your MQTT Info";
char mqtt_clientID[] = "Your MQTT Info";

// Set web server port number to 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Number of points in the graphing XML file
#define N_POINTS 240

// fancy math to find out how often to sample chart
#define DIVIDER_HIST ((48*60*1000)/N_POINTS/(1000))*60

uint16_t index_counter = 0;
uint16_t count = DIVIDER_HIST;

enum State { XML_Preamble, XML_Body, XML_Done };

// Data items for graphing
struct btData{
  float batt1volt1[N_POINTS];
  float batt1volt2[N_POINTS];
  float batt1volt3[N_POINTS];
  float batt1volt4[N_POINTS];
  float batt2volt1[N_POINTS];
  float batt2volt2[N_POINTS];
  float batt2volt3[N_POINTS];
  float batt2volt4[N_POINTS];
  float batt3volt1[N_POINTS];
  float batt3volt2[N_POINTS];
  float batt3volt3[N_POINTS];
  float batt3volt4[N_POINTS];
  float batt4volt1[N_POINTS];
  float batt4volt2[N_POINTS];
  float batt4volt3[N_POINTS];
  float batt4volt4[N_POINTS];
  float batt5volt1[N_POINTS];
  float batt5volt2[N_POINTS];
  float batt5volt3[N_POINTS];
  float batt5volt4[N_POINTS];
  float batt6volt1[N_POINTS];
  float batt6volt2[N_POINTS];
  float batt6volt3[N_POINTS];
  float batt6volt4[N_POINTS];
  float batt7volt1[N_POINTS];
  float batt7volt2[N_POINTS];
  float batt7volt3[N_POINTS];
  float batt7volt4[N_POINTS];
  float batt8volt1[N_POINTS];
  float batt8volt2[N_POINTS];
  float batt8volt3[N_POINTS];
  float batt8volt4[N_POINTS];
};

struct powerData{
  btData bt;
  unsigned long time[N_POINTS];
  bool valid[N_POINTS] = {};
};

struct powerData p_data;

// For the ILI9341, these are the default pins.
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_DC  32
#define TFT_CS 33
// Use hardware SPI (#13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Set digital out pin numbers for relays. loadRelay1 is the load disconnect and chargeRelay2 is a charge disconnect.
#define loadRelay1 27
#define chargeRelay2 14

// PWM variables for relay hold function
const int loadRelayPWMPIN = 25;
const int chargeRelayPWMPIN = 26;
const int RelayPWM_START = 255;
const int RelayPWM_ON = 128;
const int chargeRelayPWM_Channel = 0;
const int loadRelayPWM_Channel = 2;
const int RelayPWM_Freq = 20000;
const int RelayPWM_Resolution = 8;
const int RelayPWM_MAXDUTY = (int)(pow(2, RelayPWM_Resolution) - 1);
uint16_t loadRelayDuty;
uint16_t chargeRelayDuty;

#define enablePin 4 // Pin used to switch RS485 TX/RX - High=TX, Low=RX

#define debugTiming 15 // Pin used for analyzing during scope debugging timing or path issues

#define RECEIVE LOW // RS485 Pin Mode
#define TRANSMIT HIGH //RS485 Pin Mode
#define OFF LOW // Relay Pin Mode
#define ON HIGH // Relay Pin Mode

// Used for formatting console to binary display of integers
// Use: printf("Leading text "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

// Set BMS thresholds - mV and C
int battHighV = 3600;  // 3600 Default
int battLowV = 2750;  // 2750 Default
int battHighT = 6000;  // 6000 Default
int battLowT = 0200;  // 0200 Default

// keeps track of display update task state
uint8_t displayUpdateState = 0;

// The loop will change one or both of these depending on the data from the batteries.
bool loadRelay1status = true;
bool chargeRelay2status = true;

// Wake UP (9600 baud) must be used to keep the batteries awake to communicate
byte messageW[] =    {0x00, 0x00, 0x01, 0x01, 0xc0, 0x74, 0x0d, 0x0a, 0x00, 0x00};

// Commands to send to the batteries. (115200 baud) MODBUS
//                    ID  , Func, HReg, LReg, HLen, LLen, HCRC, LCRC, End , End
byte readVolts[] =   {0x00, 0x03, 0x00, 0x45, 0x00, 0x09, 0x00, 0x00, 0x0d, 0x0a};
byte readTemps[] =   {0x00, 0x03, 0x00, 0x50, 0x00, 0x07, 0x00, 0x00, 0x0d, 0x0a};
byte readCurrent[] = {0x00, 0x03, 0x00, 0x39, 0x00, 0x0a, 0x00, 0x00, 0x0d, 0x0a};
byte readBalance[] = {0x00, 0x03, 0x00, 0x1e, 0x00, 0x01, 0x00, 0x00, 0x0d, 0x0a};
byte readSOC[] =     {0x00, 0x03, 0x00, 0x6a, 0x00, 0x0c, 0x00, 0x00, 0x0d, 0x0a};

unsigned long startTime;  // Time tracking for the poll loop
unsigned long currentTime;  // Time tracking for the poll loop
unsigned long interval = 1000;  // How often to poll the batteries in milliseconds
unsigned int countFailures = 0; // Keeps track of how many times the battery didn't have a valid response

// Set address numbers for batteries you want to communicate
byte batteries[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
// Display and Console outputs setup as 4 parallel and 2 series batteries.
// Upper (positive side) battery IDs are 1,2,7,8
// Lower (negative side) battery IDs are 3,4,5,6

// Array for each battery
#define BATTDATASIZE 16
int battData[sizeof(batteries)][BATTDATASIZE];

// Array values for battery data
#define VOLT1 0 // -1=No Comm, else=value
#define VOLT2 1 // -1=No Comm, else=value
#define VOLT3 2 // -1=No Comm, else=value
#define VOLT4 3 // -1=No Comm, else=value
#define TOTAL 4 // -1=No Comm, else=value
#define TEMP1 5 // -1=No Comm, else=value
#define TEMP2 6 // -1=No Comm, else=value
#define TEMP3 7 // -1=No Comm, else=value
#define TEMP4 8 // -1=No Comm, else=value
#define TEMP5 9 // -1=No Comm, else=value
#define VSTAT 10  // -1=No Comm, 1=Normal, 2=Low, 3=High
#define TSTAT 11  // -1=No Comm, 1=Normal, 2=Low, 3=High
#define CURNT 12  // -1=No Comm, else=value
#define BALAN 13  // -1=No Comm, else=value
#define BTSOC 14  // -1=No Comm, else=value
#define VDELT 15  // -1=No Comm, else=Delta of highest to lowest cell in battery

// Hard defined macros to make changing display text color easier
#define TFTSCRBLACK   tft.fillScreen(ILI9341_BLACK)
#define TFTTXTGREEN   tft.setTextColor(ILI9341_GREEN)
#define TFTTXTWHITE   tft.setTextColor(ILI9341_WHITE)
#define TFTTXTRED     tft.setTextColor(ILI9341_RED)
#define TFTTXTYELLOW  tft.setTextColor(ILI9341_YELLOW)
#define TFTTXTMAGENTA tft.setTextColor(ILI9341_MAGENTA)
#define TFTTXTCYAN    tft.setTextColor(ILI9341_CYAN)
#define TFTTXTORANGE  tft.setTextColor(ILI9341_ORANGE)
#define TFTTXTBLUE    tft.setTextColor(ILI9341_BLUE)

// New Display Info
#define SETTEXTCURSOR 

// Function prototypes
uint16_t ModRTU_CRC(unsigned char, uint8_t);
void setup(void);
void drawDisplayNew(void);
void writeToDisplayNew(void);
void charSetCursor(uint16_t, uint16_t);
uint16_t charPosX(uint16_t);
uint16_t charPosY(uint16_t);
void charBlank(uint16_t, uint16_t, uint16_t);
void XML_hist_data(AsyncWebServerRequest *request);
void getGraphReadings(void);
String getBatteryReadings(void);
void writeToDisplay(void);
void writeToConsole(void);
void requestBattVolts(int);
void requestBattTemps(int);
void requestBattCurrent(int);
void requestBattBalance(int);
void requestBattSOC(int);
void receiveBatteryVolts(int);
void receiveBatteryTemps(int);
void receiveBatteryCurrent(int);
void receiveBatteryBalance(int);
void receiveBatterySOC(int);
void checkBattStatus(void);
void calcBattCellDelta(void);
bool relayRead(unsigned int);
void relayWrite(unsigned int, bool);
void logData(String data);
void preTransmission(void);
void postTransmission(void);
void wakeup(void);
void turnonAll(void);
void shutdownAll(void);
void shutdownPv(void);
