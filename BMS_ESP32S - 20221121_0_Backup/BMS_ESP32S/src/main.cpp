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
// 
// Display and Console outputs setup as 4 parallel and 2 series batteries.
// Upper(+) battery IDs are 1,2,7,8
// Lower(-) battery IDs are 3,4,5,6
// 

#include "BMS_ESP32S.h"

// *****************************************
// Compute the MODBUS RTU CRC
// Adapted from https://ctlsys.com/support/how_to_compute_the_modbus_rtu_message_crc/
// This function MUST come before and other functions in the code if using Visual Studio Code and PlatformIO
// Not sure why but it will have compiling issues if placed anywhere else for some reason ¯\_(ツ)_/¯
// *****************************************
uint16_t ModRTU_CRC(unsigned char buf[], uint8_t len)
{
  uint16_t crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= buf[pos];          // XOR byte into least sig. byte of crc
    for (byte i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
};

// *****************************************
// Initialize startup operation
// *****************************************
void setup()
{
  // Make sure relays are "OFF" until BMS has received data from battery and determined it is safe to enable
  pinMode(loadRelay1, OUTPUT);
  pinMode(chargeRelay2, OUTPUT);
  digitalWrite(loadRelay1, OFF);
  digitalWrite(chargeRelay2, OFF);

  // Set RS485 TX/RX to receive
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, RECEIVE);

  // Set relay status to off on startup
  loadRelay1status = false;
  chargeRelay2status = false;

  // Initialize Relay PWM Channels with Frequency and Resolution
  ledcSetup(chargeRelayPWM_Channel, RelayPWM_Freq, RelayPWM_Resolution);
  ledcSetup(loadRelayPWM_Channel, RelayPWM_Freq, RelayPWM_Resolution);
  // Attach the Relay PWM Channel to the GPIO Pin
  ledcAttachPin(loadRelayPWMPIN, loadRelayPWM_Channel);
  ledcAttachPin(chargeRelayPWMPIN, chargeRelayPWM_Channel);
  // Load OFF duty cycle
  loadRelayDuty = 0;
  chargeRelayDuty = 0;
  
  // Setup debug timing pin for output only
  pinMode(debugTiming, OUTPUT);

  // Reset comm failure counter
  countFailures = 0;
  
  // Start console serial
  Serial.begin(115200);
  //Serial.println("Serial console started.");

  // Start bluetooth serial
  Serial1.begin(9600);
  //Serial.println("Serial 1 started. (Bluetooth?)");

  // start up the LCD and initialize it
  tft.begin();
  drawDisplayNew();

  //Start up the SPIFFS file system
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS_ERROR");
    return;
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("WiFi Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  // put the IP address on the display
  TFTTXTCYAN;
  charSetCursor(4, 39); // Positions are 0 referenced x, y
  tft.print(WiFi.localIP());

  // Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/batt1.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt1.html", "text/html");
  });

  server.on("/batt2.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt2.html", "text/html");
  });

  server.on("/batt3.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt3.html", "text/html");
  });

  server.on("/batt4.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt4.html", "text/html");
  });

  server.on("/batt5.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt5.html", "text/html");
  });

  server.on("/batt6.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt6.html", "text/html");
  });

  server.on("/batt7.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt7.html", "text/html");
  });

  server.on("/batt8.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/batt8.html", "text/html");
  });

  server.on("/graph.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/graph.html", "text/html");
  });

  server.on("/hist_data.xml", HTTP_GET, [](AsyncWebServerRequest *request){
    XML_hist_data(request);
  });
  
  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getBatteryReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });

  server.addHandler(&events);
  server.begin();
  
  Serial.println("");

  Cayenne.begin(mqtt_username, mqtt_password, mqtt_clientID, ssid, password);
  Serial.println("Cayenne MQTT Started..");

  // wake up the batteries
  wakeup();

  // initialize all battery data to default -1 values (no comm)
  for (byte i = 0; i < sizeof(batteries); i++) {
    for (byte j = 0; j < BATTDATASIZE; j++) {
      battData[i][j] = -1;
    }
  }
}

// *****************************************
// Main loop
// *****************************************
void loop() {
  digitalWrite(debugTiming, ON); // Total loop time start pulse

  //WebSerial.println("Hello!"); // Test of the webserial terminal function

  startTime = millis();
  wakeup(); // Send the battery wakeup command

  // If the failure count gets too high, reset to 1 to prevent overflow.
  if (countFailures >=1){
    Serial.print("countFailures: ");
    Serial.println(countFailures);
  }
  if (countFailures >= 254) {
    countFailures = 1;
  }

  // Iterate through all of the batteries connected to the BMS.
  for (byte i = 0; i < sizeof(batteries) / sizeof(byte); i++) {
    requestBattVolts(i);
    delay(10);
    receiveBatteryVolts(i);

    requestBattTemps(i);
    delay(10);
    receiveBatteryTemps(i);

    requestBattCurrent(i);
    delay(10);
    receiveBatteryCurrent(i);

    requestBattBalance(i);
    delay(10);
    receiveBatteryBalance(i);

    requestBattSOC(i);
    delay(10);
    receiveBatterySOC(i);

  }

  // see if there were any flags for voltage or temperature
  checkBattStatus();

  // Calculate cell voltage delta for each battery and store it in VDELT location
  calcBattCellDelta();

  // Read status' and see if the relays should be enabled
  if (loadRelay1status) {
    relayWrite(loadRelay1, true);
  }
  if (chargeRelay2status) {
    relayWrite(chargeRelay2, true);
  }

  //clean out the buffer again, just in case there were more or less bytes
  while (Serial.available()) {
    Serial.read();
  }

  writeToDisplayNew(); // This function writes data to the LCD display
  //writeToDisplay(); // This function writes data to the LCD display

  //writeToConsole();

  getGraphReadings(); // get and store measurements to the graph array

  // Send Events to the web client with the Battery Readings
  events.send("ping",NULL,millis());
  events.send(getBatteryReadings().c_str(),"new_readings" ,millis());

  //Send UDP broadcast
  //udp.broadcast("Anyone here?");

  digitalWrite(debugTiming, OFF); //Total loop time end pulse

  //Cayenne.loop();

  currentTime = millis();
  while (currentTime - startTime < interval) {
    Cayenne.loop();
    // If everything runs at the fastest speed possible, we want to make sure there is
    // enough time between Serial1 messages when restarting the loop.
    currentTime = millis();
  }
}

// *****************************************
// Writes data to Cayenne MQTT
// *****************************************
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	// Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
  // Calculate total battery current - Average two parallel batts then add them as they are in series
  float BattVolt1 = battData[0][TOTAL];
  float BattVolt2 = battData[1][TOTAL];
  float BattVolt3 = battData[2][TOTAL];
  float BattVolt4 = battData[3][TOTAL];
  float BattVolt5 = battData[4][TOTAL];
  float BattVolt6 = battData[5][TOTAL];
  float BattVolt7 = battData[6][TOTAL];
  float BattVolt8 = battData[7][TOTAL];
  float BattCurr1 = battData[0][CURNT];
  float BattCurr2 = battData[1][CURNT];
  float BattCurr3 = battData[2][CURNT];
  float BattCurr4 = battData[3][CURNT];
  float BattCurr5 = battData[4][CURNT];
  float BattCurr6 = battData[5][CURNT];
  float BattCurr7 = battData[6][CURNT];
  float BattCurr8 = battData[7][CURNT];

  if (BattCurr1 > 32767) {
    BattCurr1 = BattCurr1 - 65535;
  }
  else {
    BattCurr1 = BattCurr1;
  }
  if (BattCurr2 > 32767) {
    BattCurr2 = BattCurr2 - 65535;
  }
  else {
    BattCurr2 = BattCurr2;
  }
  if (BattCurr3 > 32767) {
    BattCurr3 = BattCurr3 - 65535;
  }
  else {
    BattCurr3 = BattCurr3;
  }
  if (BattCurr4 > 32767) {
    BattCurr4 = BattCurr4 - 65535;
  }
  else {
    BattCurr4 = BattCurr4;
  }
  if (BattCurr5 > 32767) {
    BattCurr5 = BattCurr5 - 65535;
  }
  else {
    BattCurr5 = BattCurr5;
  }
  if (BattCurr6 > 32767) {
    BattCurr6 = BattCurr6 - 65535;
  }
  else {
    BattCurr6 = BattCurr6;
  }
  if (BattCurr7 > 32767) {
    BattCurr7 = BattCurr7 - 65535;
  }
  else {
    BattCurr7 = BattCurr7;
  }
  if (BattCurr8 > 32767) {
    BattCurr8 = BattCurr8 - 65535;
  }
  else {
    BattCurr8 = BattCurr8;
  }
  float totalBattVolt1 = ((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) / 1000;
  float totalBattVolt2 = ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4) / 1000;
  float totalBattVolt = (((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000;
  float totalBattCurr = (((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) + (BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6)) / 2) / 100;
  float totalBattWatt = ((((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000)
                      * ((((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) + (BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6)) /2) / 100);
  float totalSOC1 = battData[0][BTSOC];
  float totalSOC2 = battData[1][BTSOC];
  float totalSOC3 = battData[2][BTSOC];
  float totalSOC4 = battData[3][BTSOC];
  float totalSOC5 = battData[4][BTSOC];
  float totalSOC6 = battData[5][BTSOC];
  float totalSOC7 = battData[6][BTSOC];
  float totalSOC8 = battData[7][BTSOC];
  float totalSOC = ((((totalSOC1 + totalSOC2 + totalSOC5 + totalSOC6) / 4) + ((totalSOC3 + totalSOC4 + totalSOC7 + totalSOC8) / 4)) / 2) / 10;

  Cayenne.virtualWrite(0, totalSOC, "batt", "p");
  Cayenne.virtualWrite(1, totalBattWatt, "pow", "w");
  Cayenne.virtualWrite(2, totalBattCurr, "current", "a");
  Cayenne.virtualWrite(3, totalBattVolt, "voltage", "v");
  Cayenne.virtualWrite(4, totalBattVolt1, "voltage", "v");
  Cayenne.virtualWrite(5, totalBattVolt2, "voltage", "v");
  Cayenne.virtualWrite(11, battData[0][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(12, battData[1][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(13, battData[2][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(14, battData[3][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(15, battData[4][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(16, battData[5][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(17, battData[6][BALAN], "digital_sensor", "d");
  Cayenne.virtualWrite(18, battData[7][BALAN], "digital_sensor", "d");
  Serial.println("Cayenne MQTT Data Sent.");
}

// *****************************************
// Writes data to SPI LCD
// *****************************************
void drawDisplayNew() {
  TFTSCRBLACK;
  TFTTXTGREEN;
  tft.setTextSize(1);
  tft.setCursor(0, 0);

  TFTTXTWHITE;
  tft.print("      Cel1:Cel2:Cel3:Cel4:Total\r\n"); // Y=0
  tft.print("Batt1:    :    :    :    :     \r\n"); // X pos starts at : which is 5
  tft.print("Batt2:    :    :    :    :     \r\n");
  tft.print("Batt3:    :    :    :    :     \r\n");
  tft.print("Batt4:    :    :    :    :     \r\n");
  tft.print("Batt5:    :    :    :    :     \r\n");
  tft.print("Batt6:    :    :    :    :     \r\n");
  tft.print("Batt7:    :    :    :    :     \r\n");
  tft.print("Batt8:    :    :    :    :     \r\n");
  tft.print(" \r\n");
  tft.print("      Tmp1:Tmp2:Tmp3:Tmp4:Tmp5 \r\n"); // Y=10
  tft.print("Batt1:    :    :    :    :     \r\n"); // X pos starts at : which is 5
  tft.print("Batt2:    :    :    :    :     \r\n");
  tft.print("Batt3:    :    :    :    :     \r\n");
  tft.print("Batt4:    :    :    :    :     \r\n");
  tft.print("Batt5:    :    :    :    :     \r\n");
  tft.print("Batt6:    :    :    :    :     \r\n");
  tft.print("Batt7:    :    :    :    :     \r\n");
  tft.print("Batt8:    :    :    :    :     \r\n");
  tft.print(" \r\n");
  tft.print("      V:T:Curr:Bal:SOC :Delta \r\n");  // Y=20
  tft.print("Batt1: : :    :   :    :     \r\n"); // X pos starts at : which is 5
  tft.print("Batt2: : :    :   :    :     \r\n");
  tft.print("Batt3: : :    :   :    :     \r\n");
  tft.print("Batt4: : :    :   :    :     \r\n");
  tft.print("Batt5: : :    :   :    :     \r\n");
  tft.print("Batt6: : :    :   :    :     \r\n");
  tft.print("Batt7: : :    :   :    :     \r\n");
  tft.print("Batt8: : :    :   :    :     \r\n");
  tft.print(" \r\n");
  tft.print("Upr:            Lwr:       \r\n");  // Y=30, X=5 and X=21
  tft.print("Vlt:            Cur:       \r\n");  // Y=31, X=5 and X=21
  tft.setTextSize(2);
  tft.print("                      \r\n"); // Location of Power
  tft.print("SOC:    \r\n");
  tft.setTextSize(1);
  tft.print("LoadRelay:      ChrgRelay:   \r\n");  // Y=37, X=11 and X=27
  tft.print("IP:               \r\n");  // Y=38, X=4
}

// *****************************************
// Writes data to SPI LCD
// *****************************************
void writeToDisplayNew() {
  TFTTXTGREEN;
  tft.setTextSize(1);
  for (byte i = 0; i < sizeof(batteries); i++) {
    charBlank(5, i+1, 28);  // (character X pos, character  pos, number of characters)
    TFTTXTGREEN;
    charSetCursor(5,i+1);
    for (byte j = 0; j < (TOTAL + 1); j++) {
      TFTTXTWHITE;
      tft.print(":");
      TFTTXTGREEN;
      if ((battData[i][j] >= battHighV) || (battData[i][j] <= battLowV)) {
        TFTTXTRED;
      }
      if (j == (TOTAL)) {
        //TFTTXTWHITE;
        TFTTXTCYAN;
        if (battData[i][j] == -1){
          TFTTXTRED;
        }
      }
      tft.print(String(battData[i][j]));
      TFTTXTWHITE;
    }
  }
  TFTTXTGREEN;
  for (byte i = 0; i < sizeof(batteries); i++) {
    charBlank(5, i+11, 28);  // (character X pos, character  pos, number of characters)
    TFTTXTGREEN;
    charSetCursor(5,i+11);
    for (byte j = (TEMP1); j < (TEMP5 + 1); j++) {
      TFTTXTWHITE;
      tft.print(":");
      TFTTXTGREEN;
      if ((battData[i][j] >= battHighT) || (battData[i][j] <= battLowT)) {
        TFTTXTRED;
      }
      tft.print(String(battData[i][j]));
      TFTTXTWHITE;
    }
  }
  TFTTXTGREEN;
  for (byte i = 0; i < sizeof(batteries); i++) {
    charBlank(5, i+21, 28);  // (character X pos, character  pos, number of characters)
    TFTTXTGREEN;
    charSetCursor(5,i+21);
    for (byte j = (VSTAT); j < (VDELT + 1); j++) {
      TFTTXTWHITE;
      tft.print(":");
      TFTTXTGREEN;

      //some batt data (current) is a signed value stored in an unsigned variable so need to convert
      if (j == (CURNT)) {
        TFTTXTWHITE;
        int battDataLog;
        if (battData[i][j] > 32767) {
          battDataLog = battData[i][j] - 65535;
        }
        else {
          TFTTXTGREEN;
          battDataLog = battData[i][j];
        }
        tft.print(String(battDataLog));
      }
      else {
        TFTTXTGREEN;
        if ((j == BALAN) && (battData[i][j] >= 1)) {
          TFTTXTYELLOW;
          tft.print("BAL");
        }
        else if ((j == BALAN) && (battData[i][j] == 0)) {
          TFTTXTGREEN;
          tft.print("OFF");
        }
        else if ((j == VSTAT || j == TSTAT) && (battData[i][j] >= 1)) {
          TFTTXTRED;
          tft.print(String(battData[i][j]));
        }
        else if (j == BTSOC) {
          //TFTTXTMAGENTA;
          TFTTXTCYAN;
          char str[7];
          uint16_t soc = battData[i][j];
          sprintf(str, "%-6.1f", soc / 10.0);
          tft.print(str);
        }
        else {
          tft.print(String(battData[i][j]));
        }
      }
      TFTTXTWHITE;
    }
  }

  // Calculate total battery current - Average two parallel batts then add them as they are in series
  float BattVolt1 = battData[0][TOTAL];
  float BattVolt2 = battData[1][TOTAL];
  float BattVolt3 = battData[2][TOTAL];
  float BattVolt4 = battData[3][TOTAL];
  float BattVolt5 = battData[4][TOTAL];
  float BattVolt6 = battData[5][TOTAL];
  float BattVolt7 = battData[6][TOTAL];
  float BattVolt8 = battData[7][TOTAL];
  float BattCurr1 = battData[0][CURNT];
  float BattCurr2 = battData[1][CURNT];
  float BattCurr3 = battData[2][CURNT];
  float BattCurr4 = battData[3][CURNT];
  float BattCurr5 = battData[4][CURNT];
  float BattCurr6 = battData[5][CURNT];
  float BattCurr7 = battData[6][CURNT];
  float BattCurr8 = battData[7][CURNT];

  if (BattCurr1 > 32767) {
    BattCurr1 = BattCurr1 - 65535;
  }
  else {
    BattCurr1 = BattCurr1;
  }
  if (BattCurr2 > 32767) {
    BattCurr2 = BattCurr2 - 65535;
  }
  else {
    BattCurr2 = BattCurr2;
  }
  if (BattCurr3 > 32767) {
    BattCurr3 = BattCurr3 - 65535;
  }
  else {
    BattCurr3 = BattCurr3;
  }
  if (BattCurr4 > 32767) {
    BattCurr4 = BattCurr4 - 65535;
  }
  else {
    BattCurr4 = BattCurr4;
  }
  if (BattCurr5 > 32767) {
    BattCurr5 = BattCurr5 - 65535;
  }
  else {
    BattCurr5 = BattCurr5;
  }
  if (BattCurr6 > 32767) {
    BattCurr6 = BattCurr6 - 65535;
  }
  else {
    BattCurr6 = BattCurr6;
  }
  if (BattCurr7 > 32767) {
    BattCurr7 = BattCurr7 - 65535;
  }
  else {
    BattCurr7 = BattCurr7;
  }
  if (BattCurr8 > 32767) {
    BattCurr8 = BattCurr8 - 65535;
  }
  else {
    BattCurr8 = BattCurr8;
  }
  float totalBattVolt1 = ((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) / 1000;
  float totalBattVolt2 = ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4) / 1000;
  float totalBattVolt = (((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000;
  float totalBattCurr = (((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) + (BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6)) / 2) / 100;
  float totalBattWatt = ((((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000)
                      * ((((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) + (BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6)) /2) / 100);
  float totalSOC1 = battData[0][BTSOC];
  float totalSOC2 = battData[1][BTSOC];
  float totalSOC3 = battData[2][BTSOC];
  float totalSOC4 = battData[3][BTSOC];
  float totalSOC5 = battData[4][BTSOC];
  float totalSOC6 = battData[5][BTSOC];
  float totalSOC7 = battData[6][BTSOC];
  float totalSOC8 = battData[7][BTSOC];
  float totalSOC = ((((totalSOC1 + totalSOC2 + totalSOC5 + totalSOC6) / 4) + ((totalSOC3 + totalSOC4 + totalSOC7 + totalSOC8) / 4)) / 2) / 10;
  
  charBlank(5, 30, 6);  // (character X pos, character  pos, number of characters)
  charSetCursor(5,30);
  //TFTTXTWHITE;
  // tft.setTextSize(2);
  //tft.print("Upr: ");
  TFTTXTMAGENTA;
  tft.print(String(totalBattVolt1) + "V");
  
  charBlank(21, 30, 6);  // (character X pos, character  pos, number of characters)
  charSetCursor(21,30);
  //TFTTXTWHITE;
  //tft.print("Lwr: ");
  TFTTXTMAGENTA;
  tft.print(String(totalBattVolt2) + "V");
  
  charBlank(5, 31, 6);  // (character X pos, character  pos, number of characters)
  charSetCursor(5,31);
  //TFTTXTWHITE;
  //tft.print("Vlt: ");
  tft.setTextColor(ILI9341_GREENYELLOW);
  tft.print(String(totalBattVolt) + "V");
  
  charBlank(21, 31, 8);  // (character X pos, character  pos, number of characters)
  charSetCursor(21,31);
  //TFTTXTWHITE;
  //tft.print("Cur: ");
  TFTTXTCYAN;
  tft.print(String(totalBattCurr) + "A");
  
  //charBlank(0, 32, 28);  // (character X pos, character  pos, number of characters)
  //charSetCursor(0,32);
  tft.fillRect(0, 264, 200, 16, ILI9341_BLACK);  // pos x, pos y, pixels x, pixels y, color
  tft.setTextSize(2);
  charSetCursor(0,33);
  // tft.setTextSize(2);
  if (totalBattCurr > 0) {
    TFTTXTYELLOW;
    tft.print("Disch ");
  }
  else if (totalBattCurr < 0) {
    tft.setTextColor(ILI9341_ORANGE);
    tft.print("Charg ");
  }
  else if (totalBattCurr == 0) {
    TFTTXTBLUE;
    tft.print("Idle ");
  }
  tft.print(String(totalBattWatt) + "W");
  
  //charBlank(5, 33, 7);  // (character X pos, character  pos, number of characters)
  tft.fillRect(47, 288, 100, 16, ILI9341_BLACK);  // pos x, pos y, pixels x, pixels y, color
  tft.setTextSize(2);
  charSetCursor(8,36);
  //TFTTXTWHITE;
  //tft.print("SOC: ");
  //TFTTXTMAGENTA;
  TFTTXTCYAN;
  tft.print(String(totalSOC) + "%");
  
  tft.setTextSize(1);
  charBlank(11, 38, 3);  // (character X pos, character  pos, number of characters)
  charSetCursor(11,38);
  //TFTTXTWHITE;
  //tft.print("LoadRelay1: ");
  if (loadRelay1status) {
    TFTTXTGREEN;
    tft.print("ON");
  } else {
    TFTTXTRED;
    tft.print("OFF");
  }
  
  charBlank(27, 38, 3);  // (character X pos, character  pos, number of characters)
  charSetCursor(27,38);
  //TFTTXTWHITE;
  //tft.print("ChrgRelay2: ");
  if (chargeRelay2status) {
    TFTTXTGREEN;
    tft.print("ON");
  } else {
    TFTTXTRED;
    tft.print("OFF");
  }

  charBlank(4, 39, 16);  // (character X pos, character  pos, number of characters)
  charSetCursor(4,39);
  //TFTTXTWHITE;
  //tft.print("IP: ");
  TFTTXTCYAN;
  tft.print(WiFi.localIP());
}


// *****************************************
// Convert character position to pixel position and set it there
// *****************************************
void charSetCursor(uint16_t posx, uint16_t posy) {
  tft.setCursor(charPosX(posx),charPosY(posy));
}

// *****************************************
// Convert character X position to pixel position
// *****************************************
uint16_t charPosX(uint16_t charPos) {
  uint16_t pixels;
  if (charPos > 0) {
    pixels = charPos * 6; // characters are 5 pixels wide plus 1 pad
  }
  else {
    pixels = 0; // position 0 has no padding
  }
  return pixels;
}

// *****************************************
// Convert character Y position to pixel position
// *****************************************
uint16_t charPosY(uint16_t charPos) {
  uint16_t pixels;
  if (charPos > 0) { 
    pixels = charPos * 8; // characters are 7 pixels wide plus 1 pad
  }
  else {
    pixels = 0; // position 0 has no padding
  }
  return pixels;
}

// *****************************************
// Convert character X and Y position to pixel position and blank number of characters
// *****************************************
void charBlank(uint16_t posx, uint16_t posy, uint16_t numChars) {
  uint16_t pixelsX = numChars * 6;  // characters width pixels
  uint16_t pixelsY = 8; // character height in pixels
  tft.fillRect(posx * 6, posy * 8, pixelsX, pixelsY, ILI9341_BLACK);  // pixel position x, pixel position y, pixels x, pixels y
}

// *****************************************
// Process the data into an XML file for the graphing server
//******************************************
void XML_hist_data(AsyncWebServerRequest *request)
{
  int* lambda_copy_ix = new int(index_counter+2);
  int* lambda_counter = new int(0); 
  int* lambda_state = new int(0); 
  AsyncWebServerResponse *response = request->beginChunkedResponse("text/xml",  [lambda_counter, lambda_state, lambda_copy_ix](uint8_t *buffer, size_t maxLen, size_t index) mutable -> size_t  {
    String temp = "";


    if(*lambda_state == XML_Done)
    {
    delete lambda_counter;
    delete lambda_state;
    delete lambda_copy_ix;
    return 0;
    }

    if(*lambda_state == XML_Body)
    {
      temp = "";
      for(uint32_t iteration = 0; iteration < 5 || temp.length() == 0; iteration++)
      {
        uint32_t index = (*lambda_copy_ix+*lambda_counter)%N_POINTS;
        if(p_data.valid[index])
        {
          temp.concat("<d_p t=\""+String(p_data.time[index])+"\">");

          temp.concat("<b1_v1>");
          temp.concat(String(p_data.bt.batt1volt1[index]));
          temp.concat("</b1_v1>");
          temp.concat("<b1_v2>");
          temp.concat(String(p_data.bt.batt1volt2[index]));
          temp.concat("</b1_v2>");
          temp.concat("<b1_v3>");
          temp.concat(String(p_data.bt.batt1volt3[index]));
          temp.concat("</b1_v3>");
          temp.concat("<b1_v4>");
          temp.concat(String(p_data.bt.batt1volt4[index]));
          temp.concat("</b1_v4>");

          temp.concat("<b2_v1>");
          temp.concat(String(p_data.bt.batt2volt1[index]));
          temp.concat("</b2_v1>");
          temp.concat("<b2_v2>");
          temp.concat(String(p_data.bt.batt2volt2[index]));
          temp.concat("</b2_v2>");
          temp.concat("<b2_v3>");
          temp.concat(String(p_data.bt.batt2volt3[index]));
          temp.concat("</b2_v3>");
          temp.concat("<b2_v4>");
          temp.concat(String(p_data.bt.batt2volt4[index]));
          temp.concat("</b2_v4>");

          temp.concat("<b3_v1>");
          temp.concat(String(p_data.bt.batt3volt1[index]));
          temp.concat("</b3_v1>");
          temp.concat("<b3_v2>");
          temp.concat(String(p_data.bt.batt3volt2[index]));
          temp.concat("</b3_v2>");
          temp.concat("<b3_v3>");
          temp.concat(String(p_data.bt.batt3volt3[index]));
          temp.concat("</b3_v3>");
          temp.concat("<b3_v4>");
          temp.concat(String(p_data.bt.batt3volt4[index]));
          temp.concat("</b3_v4>");

          temp.concat("<b4_v1>");
          temp.concat(String(p_data.bt.batt4volt1[index]));
          temp.concat("</b4_v1>");
          temp.concat("<b4_v2>");
          temp.concat(String(p_data.bt.batt4volt2[index]));
          temp.concat("</b4_v2>");
          temp.concat("<b4_v3>");
          temp.concat(String(p_data.bt.batt4volt3[index]));
          temp.concat("</b4_v3>");
          temp.concat("<b4_v4>");
          temp.concat(String(p_data.bt.batt4volt4[index]));
          temp.concat("</b4_v4>");

          temp.concat("<b5_v1>");
          temp.concat(String(p_data.bt.batt5volt1[index]));
          temp.concat("</b5_v1>");
          temp.concat("<b5_v2>");
          temp.concat(String(p_data.bt.batt5volt2[index]));
          temp.concat("</b5_v2>");
          temp.concat("<b5_v3>");
          temp.concat(String(p_data.bt.batt5volt3[index]));
          temp.concat("</b5_v3>");
          temp.concat("<b5_v4>");
          temp.concat(String(p_data.bt.batt5volt4[index]));
          temp.concat("</b5_v4>");

          temp.concat("<b6_v1>");
          temp.concat(String(p_data.bt.batt6volt1[index]));
          temp.concat("</b6_v1>");
          temp.concat("<b6_v2>");
          temp.concat(String(p_data.bt.batt6volt2[index]));
          temp.concat("</b6_v2>");
          temp.concat("<b6_v3>");
          temp.concat(String(p_data.bt.batt6volt3[index]));
          temp.concat("</b6_v3>");
          temp.concat("<b6_v4>");
          temp.concat(String(p_data.bt.batt6volt4[index]));
          temp.concat("</b6_v4>");

          temp.concat("<b7_v1>");
          temp.concat(String(p_data.bt.batt7volt1[index]));
          temp.concat("</b7_v1>");
          temp.concat("<b7_v2>");
          temp.concat(String(p_data.bt.batt7volt2[index]));
          temp.concat("</b7_v2>");
          temp.concat("<b7_v3>");
          temp.concat(String(p_data.bt.batt7volt3[index]));
          temp.concat("</b7_v3>");
          temp.concat("<b7_v4>");
          temp.concat(String(p_data.bt.batt7volt4[index]));
          temp.concat("</b7_v4>");

          temp.concat("<b8_v1>");
          temp.concat(String(p_data.bt.batt8volt1[index]));
          temp.concat("</b8_v1>");
          temp.concat("<b8_v2>");
          temp.concat(String(p_data.bt.batt8volt2[index]));
          temp.concat("</b8_v2>");
          temp.concat("<b8_v3>");
          temp.concat(String(p_data.bt.batt8volt3[index]));
          temp.concat("</b8_v3>");
          temp.concat("<b8_v4>");
          temp.concat(String(p_data.bt.batt8volt4[index]));
          temp.concat("</b8_v4>");

          temp.concat("</d_p>");
        }

        if(*lambda_counter > N_POINTS-5)
        {
          *lambda_state = XML_Done;
          temp.concat("</p_data>");
          iteration = 5;

        }
        else
        {
          *lambda_counter = *lambda_counter + 1;
        }
      }
    }

    if(*lambda_state == XML_Preamble)
    {
      temp = "";
      temp.concat("<?xml version = \"1.0\" ?>");
      temp.concat("<p_data>");
      temp.concat("<curr_time>");
      temp.concat(String(millis()));
      temp.concat("</curr_time>");
      *lambda_state = XML_Body;
    }
    strncpy((char *)buffer, temp.c_str(), maxLen);
    return min(maxLen, temp.length());
  });
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

// *****************************************
// Get battery readings and store in memory
// *****************************************
void getGraphReadings(){
  p_data.time[index_counter] = millis();
  p_data.valid[index_counter] = true;
  p_data.bt.batt1volt1[index_counter] = battData[0][VOLT1] / 1000;  // divided by 1000 to convert mV = V
  p_data.bt.batt1volt2[index_counter] = battData[0][VOLT2] / 1000;
  p_data.bt.batt1volt3[index_counter] = battData[0][VOLT3] / 1000;
  p_data.bt.batt1volt4[index_counter] = battData[0][VOLT4] / 1000;
  p_data.bt.batt2volt1[index_counter] = battData[1][VOLT1] / 1000;
  p_data.bt.batt2volt2[index_counter] = battData[1][VOLT2] / 1000;
  p_data.bt.batt2volt3[index_counter] = battData[1][VOLT3] / 1000;
  p_data.bt.batt2volt4[index_counter] = battData[1][VOLT4] / 1000;
  p_data.bt.batt3volt1[index_counter] = battData[2][VOLT1] / 1000;
  p_data.bt.batt3volt2[index_counter] = battData[2][VOLT2] / 1000;
  p_data.bt.batt3volt3[index_counter] = battData[2][VOLT3] / 1000;
  p_data.bt.batt3volt4[index_counter] = battData[2][VOLT4] / 1000;
  p_data.bt.batt4volt1[index_counter] = battData[3][VOLT1] / 1000;
  p_data.bt.batt4volt2[index_counter] = battData[3][VOLT2] / 1000;
  p_data.bt.batt4volt3[index_counter] = battData[3][VOLT3] / 1000;
  p_data.bt.batt4volt4[index_counter] = battData[3][VOLT4] / 1000;
  p_data.bt.batt5volt1[index_counter] = battData[4][VOLT1] / 1000;
  p_data.bt.batt5volt2[index_counter] = battData[4][VOLT2] / 1000;
  p_data.bt.batt5volt3[index_counter] = battData[4][VOLT3] / 1000;
  p_data.bt.batt5volt4[index_counter] = battData[4][VOLT4] / 1000;
  p_data.bt.batt6volt1[index_counter] = battData[5][VOLT1] / 1000;
  p_data.bt.batt6volt2[index_counter] = battData[5][VOLT2] / 1000;
  p_data.bt.batt6volt3[index_counter] = battData[5][VOLT3] / 1000;
  p_data.bt.batt6volt4[index_counter] = battData[5][VOLT4] / 1000;
  p_data.bt.batt7volt1[index_counter] = battData[6][VOLT1] / 1000;
  p_data.bt.batt7volt2[index_counter] = battData[6][VOLT2] / 1000;
  p_data.bt.batt7volt3[index_counter] = battData[6][VOLT3] / 1000;
  p_data.bt.batt7volt4[index_counter] = battData[6][VOLT4] / 1000;
  p_data.bt.batt8volt1[index_counter] = battData[7][VOLT1] / 1000;
  p_data.bt.batt8volt2[index_counter] = battData[7][VOLT2] / 1000;
  p_data.bt.batt8volt3[index_counter] = battData[7][VOLT3] / 1000;
  p_data.bt.batt8volt4[index_counter] = battData[7][VOLT4] / 1000;
  count++;
  if(count >= DIVIDER_HIST){
    //read success. Increment array index.

    index_counter++;
    count = 0;
  }

  if(index_counter >= N_POINTS)
  {
    index_counter = 0;
  }
}

// *****************************************
// Get the index count for data storage
// *****************************************
uint16_t get_index_counter(){
    return index_counter;
}

// *****************************************
// Get battery readings and return JSON object for web server
// *****************************************
String getBatteryReadings(){
  //Serial.println("getBatteryReadings()");
  for (byte i = 0; i < sizeof(batteries); i++) {
    for (byte j =0; j < (VDELT + 1); j++) {
      readings["batt" + String(i+1) + "data" + String(j+1)] = String(battData[i][j]);
    }
  }
  if(chargeRelay2status){
    readings["battchrelay"] = "ON";
  }else{
    readings["battchrelay"] = "OFF";
  }
  if(loadRelay1status){
    readings["battldrelay"] = "ON";
  }else{
    readings["battldrelay"] = "OFF";
  }
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// *****************************************
// Writes data to SPI LCD
// *****************************************
void writeToDisplay() {
  TFTSCRBLACK;
  TFTTXTGREEN;
  tft.setTextSize(1);
  tft.setCursor(0, 0);

  TFTTXTWHITE;
  tft.print("      Cel1:Cel2:Cel3:Cel4:Total\r\n");
  for (byte i = 0; i < sizeof(batteries); i++) {
    tft.print("Batt" + String(i + 1));
    for (byte j = 0; j < (TOTAL + 1); j++) {
      tft.print(":");
      TFTTXTGREEN;
      if ((battData[i][j] >= battHighV) || (battData[i][j] <= battLowV)) {
        TFTTXTRED;
      }
      if (j == (TOTAL)) {
        //TFTTXTWHITE;
        TFTTXTCYAN;
        if (battData[i][j] == -1){
          TFTTXTRED;
        }
      }
      tft.print(String(battData[i][j]));
      TFTTXTWHITE;
    }
    tft.print("\r\n");
  }
  tft.print("\r\n");
  TFTTXTWHITE;
  tft.print("      Tmp1:Tmp2:Tmp3:Tmp4:TmpB\r\n");
  for (byte i = 0; i < sizeof(batteries); i++) {
    tft.print("Batt" + String(i + 1));
    for (byte j = (TEMP1); j < (TEMP5 + 1); j++) {
      tft.print(":");
      TFTTXTGREEN;
      if ((battData[i][j] >= battHighT) || (battData[i][j] <= battLowT)) {
        TFTTXTRED;
      }
      tft.print(String(battData[i][j]));
      TFTTXTWHITE;
    }
    tft.print("\r\n");
  }
  tft.print("\r\n");
  TFTTXTWHITE;
  tft.print("      V:T:Curr:Bal:SOC:Delta \r\n");
  for (byte i = 0; i < sizeof(batteries); i++) {
    tft.print("Batt" + String(i + 1));
    for (byte j = (VSTAT); j < (VDELT + 1); j++) {
      tft.print(":");
      TFTTXTGREEN;

      //some batt data (current) is a signed value stored in an unsigned variable so need to convert
      if (j == (CURNT)) {
        TFTTXTWHITE;
        int battDataLog;
        if (battData[i][j] > 32767) {
          battDataLog = battData[i][j] - 65535;
        }
        else {
          TFTTXTGREEN;
          battDataLog = battData[i][j];
        }
        tft.print(String(battDataLog));
      }
      else {
        TFTTXTGREEN;
        if ((j == BALAN) && (battData[i][j] >= 1)) {
          TFTTXTYELLOW;
          tft.print("BAL");
        }
        else if ((j == BALAN) && (battData[i][j] == 0)) {
          TFTTXTGREEN;
          tft.print("OFF");
        }
        else if ((j == VSTAT || j == TSTAT) && (battData[i][j] >= 1)) {
          TFTTXTRED;
          tft.print(String(battData[i][j]));
        }
        else if (j == BTSOC) {
          TFTTXTMAGENTA;
          char str[7];
          uint16_t soc = battData[i][j];
          sprintf(str, "%-6.1f", soc / 10.0);
          tft.print(str);
        }
        else {
          tft.print(String(battData[i][j]));
        }
      }
      TFTTXTWHITE;
    }
    tft.print("\r\n");
  }
  tft.print("\r\n");  // Space between sections

  // Calculate total battery current - Average two parallel batts then add them as they are in series
  float BattVolt1 = battData[0][TOTAL];
  float BattVolt2 = battData[1][TOTAL];
  float BattVolt3 = battData[2][TOTAL];
  float BattVolt4 = battData[3][TOTAL];
  float BattVolt5 = battData[4][TOTAL];
  float BattVolt6 = battData[5][TOTAL];
  float BattVolt7 = battData[6][TOTAL];
  float BattVolt8 = battData[7][TOTAL];
  float BattCurr1 = battData[0][CURNT];
  float BattCurr2 = battData[1][CURNT];
  float BattCurr3 = battData[2][CURNT];
  float BattCurr4 = battData[3][CURNT];
  float BattCurr5 = battData[4][CURNT];
  float BattCurr6 = battData[5][CURNT];
  float BattCurr7 = battData[6][CURNT];
  float BattCurr8 = battData[7][CURNT];

  if (BattCurr1 > 32767) {
    BattCurr1 = BattCurr1 - 65535;
  }
  else {
    BattCurr1 = BattCurr1;
  }
  if (BattCurr2 > 32767) {
    BattCurr2 = BattCurr2 - 65535;
  }
  else {
    BattCurr2 = BattCurr2;
  }
  if (BattCurr3 > 32767) {
    BattCurr3 = BattCurr3 - 65535;
  }
  else {
    BattCurr3 = BattCurr3;
  }
  if (BattCurr4 > 32767) {
    BattCurr4 = BattCurr4 - 65535;
  }
  else {
    BattCurr4 = BattCurr4;
  }
  if (BattCurr5 > 32767) {
    BattCurr5 = BattCurr5 - 65535;
  }
  else {
    BattCurr5 = BattCurr5;
  }
  if (BattCurr6 > 32767) {
    BattCurr6 = BattCurr6 - 65535;
  }
  else {
    BattCurr6 = BattCurr6;
  }
  if (BattCurr7 > 32767) {
    BattCurr7 = BattCurr7 - 65535;
  }
  else {
    BattCurr7 = BattCurr7;
  }
  if (BattCurr8 > 32767) {
    BattCurr8 = BattCurr8 - 65535;
  }
  else {
    BattCurr8 = BattCurr8;
  }
  float totalBattVolt1 = ((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) / 1000;
  float totalBattVolt2 = ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4) / 1000;
  float totalBattVolt = (((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000;
  float totalBattCurr = (((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) + (BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6)) / 2) / 100;
  float totalBattWatt = ((((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000) * ((((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) / 4) + ((BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6) / 4)) / 100);
  float totalSOC1 = battData[0][BTSOC];
  float totalSOC2 = battData[1][BTSOC];
  float totalSOC3 = battData[2][BTSOC];
  float totalSOC4 = battData[3][BTSOC];
  float totalSOC5 = battData[4][BTSOC];
  float totalSOC6 = battData[5][BTSOC];
  float totalSOC7 = battData[6][BTSOC];
  float totalSOC8 = battData[7][BTSOC];
  float totalSOC = ((((totalSOC1 + totalSOC2 + totalSOC5 + totalSOC6) / 4) + ((totalSOC3 + totalSOC4 + totalSOC7 + totalSOC8) / 4)) / 2) / 10;
  TFTTXTWHITE;
  // tft.setTextSize(2);
  tft.print("Upr: ");
  TFTTXTMAGENTA;
  tft.print(String(totalBattVolt1) + "V");
  tft.print("\r\n");
  TFTTXTWHITE;
  tft.print("Lwr: ");
  TFTTXTMAGENTA;
  tft.print(String(totalBattVolt2) + "V");
  tft.print("\r\n");
  TFTTXTWHITE;
  tft.print("Vlt: ");
  tft.setTextColor(ILI9341_GREENYELLOW);
  tft.print(String(totalBattVolt) + "V");
  tft.print("\r\n");
  TFTTXTWHITE;
  tft.print("Cur: ");
  TFTTXTCYAN;
  tft.print(String(totalBattCurr) + "A");
  tft.print("\r\n");

  // tft.setTextSize(2);
  if (totalBattCurr > 0) {
    TFTTXTYELLOW;
    tft.print("Disch ");
  }
  else if (totalBattCurr < 0) {
    tft.setTextColor(ILI9341_ORANGE);
    tft.print("Charg ");
  }
  else if (totalBattCurr == 0) {
    TFTTXTBLUE;
    tft.print("Idle ");
  }
  tft.print(String(totalBattWatt) + "W");
  tft.print("\r\n");

  TFTTXTWHITE;
  tft.print("SOC: ");
  TFTTXTMAGENTA;
  tft.print(String(totalSOC) + "%");
  tft.print("\r\n");

  TFTTXTWHITE;
  tft.print("LoadRelay1: ");
  if (loadRelay1status) {
    TFTTXTGREEN;
    tft.print("ON");
  } else {
    TFTTXTRED;
    tft.print("OFF");
  }
  tft.print("\r\n");

  TFTTXTWHITE;
  tft.print("ChrgRelay2: ");
  if (chargeRelay2status) {
    TFTTXTGREEN;
    tft.print("ON");
  } else {
    TFTTXTRED;
    tft.print("OFF");
  }
  tft.print("\r\n");

  tft.print("\r\n");  // Space between sections
  TFTTXTWHITE;
  tft.print("IP: ");
  TFTTXTCYAN;
  tft.print(WiFi.localIP());
}

// *****************************************
// Send data to the serial console
// *****************************************
void writeToConsole() {
  logData("       Cel1 :Cel2 :Cel3 :Cel4 :Total :Tmp1 :Tmp2 :Tmp3 :Tmp4 :TmpB\r\n");
  for (byte i = 0; i < sizeof(batteries); i++) {
    logData("Batt" + String(i + 1));
    for (byte j = 0; j < (TEMP5 + 1); j++) {
      logData(" :");
      logData(String(battData[i][j]));
    }
    logData("\r\n");
  }
  logData("\r\n");

  logData("       V :T :Curr :Bal :SOC\r\n");
  for (byte i = 0; i < sizeof(batteries); i++) {
    logData("Batt" + String(i + 1));
    for (byte j = (VSTAT); j < (BTSOC + 1); j++) {
      logData(" :");

      //some batt data (current) is a signed value stored in an unsigned variable so need to convert
      if (j == (CURNT)) {
        int battDataLog;
        if (battData[i][j] > 32767) {
          battDataLog = battData[i][j] - 65535;
        }
        else {
          battDataLog = battData[i][j];
        }
        logData(String(battDataLog));
      }
      else if (j == (BTSOC)) {
        char str[7];
        uint16_t soc = battData[i][j];
        sprintf(str, "%-6.1f", soc / 10.0);
        //logData(String(str));
        logData(str);
      }
      else {
        logData(String(battData[i][j]));
      }
    }
    logData("\r\n");
  }
  logData("\r\n");

  // Calculate total battery current - Average two parallel batts then add them as they are in series
  float BattVolt1 = battData[0][TOTAL];
  float BattVolt2 = battData[1][TOTAL];
  float BattVolt3 = battData[2][TOTAL];
  float BattVolt4 = battData[3][TOTAL];
  float BattVolt5 = battData[4][TOTAL];
  float BattVolt6 = battData[5][TOTAL];
  float BattVolt7 = battData[6][TOTAL];
  float BattVolt8 = battData[7][TOTAL];
  float BattCurr1 = battData[0][CURNT];
  float BattCurr2 = battData[1][CURNT];
  float BattCurr3 = battData[2][CURNT];
  float BattCurr4 = battData[3][CURNT];
  float BattCurr5 = battData[4][CURNT];
  float BattCurr6 = battData[5][CURNT];
  float BattCurr7 = battData[6][CURNT];
  float BattCurr8 = battData[7][CURNT];
  if (BattCurr1 > 32767) {
    BattCurr1 = BattCurr1 - 65535;
  }
  else {
    BattCurr1 = BattCurr1;
  }
  if (BattCurr2 > 32767) {
    BattCurr2 = BattCurr2 - 65535;
  }
  else {
    BattCurr2 = BattCurr2;
  }
  if (BattCurr3 > 32767) {
    BattCurr3 = BattCurr3 - 65535;
  }
  else {
    BattCurr3 = BattCurr3;
  }
  if (BattCurr4 > 32767) {
    BattCurr4 = BattCurr4 - 65535;
  }
  else {
    BattCurr4 = BattCurr4;
  }
  if (BattCurr5 > 32767) {
    BattCurr5 = BattCurr5 - 65535;
  }
  else {
    BattCurr5 = BattCurr5;
  }
  if (BattCurr6 > 32767) {
    BattCurr6 = BattCurr6 - 65535;
  }
  else {
    BattCurr6 = BattCurr6;
  }
  if (BattCurr7 > 32767) {
    BattCurr7 = BattCurr7 - 65535;
  }
  else {
    BattCurr7 = BattCurr7;
  }
  if (BattCurr8 > 32767) {
    BattCurr8 = BattCurr8 - 65535;
  }
  else {
    BattCurr8 = BattCurr8;
  }
  float totalBattVolt1 = ((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) / 1000;
  float totalBattVolt2 = ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4) / 1000;
  float totalBattVolt = (((BattVolt1 + BattVolt2 + BattVolt7 + BattVolt8) / 4) + ((BattVolt3 + BattVolt4 + BattVolt5 + BattVolt6) / 4)) / 1000;
  float totalBattCurr = (((BattCurr1 + BattCurr2 + BattCurr7 + BattCurr8) + (BattCurr3 + BattCurr4 + BattCurr5 + BattCurr6)) / 2) / 100;
  float totalSOC1 = battData[0][BTSOC];
  float totalSOC2 = battData[1][BTSOC];
  float totalSOC3 = battData[2][BTSOC];
  float totalSOC4 = battData[3][BTSOC];
  float totalSOC5 = battData[4][BTSOC];
  float totalSOC6 = battData[5][BTSOC];
  float totalSOC7 = battData[6][BTSOC];
  float totalSOC8 = battData[7][BTSOC];
  float totalSOC = ((((totalSOC1 + totalSOC2 + totalSOC5 + totalSOC6) / 4) + ((totalSOC3 + totalSOC4 + totalSOC7 + totalSOC8) / 4)) / 2) / 10;
  logData("Upr:" + String(totalBattVolt1) + "V");
  logData("  Lwr:" + String(totalBattVolt2) + "V");
  logData("\r\n");
  logData("Vlt:" + String(totalBattVolt) + "V");
  logData("  Cur:" + String(totalBattCurr) + "A");
  logData("\r\n");
  logData("SOC:" + String(totalSOC) + "%");
  logData("\r\n");
  logData("LoadRelay1: " + String(loadRelay1status));
  logData("\r\n");
  logData("ChrgRelay2: " + String(chargeRelay2status));
  logData("\r\n");
  logData("\r\n"); // Create empty line in log before looping
}

// *****************************************
// Send message to battery to get voltage
// *****************************************
void requestBattVolts(int battNum) {
  if (battNum == 2 || battNum == 4 || battNum == 5 || battNum == 8) { // unsure why battery ID 2, 4, 5, and 8 need odd parity but it does
    Serial2.begin(115200, SERIAL_8O1);  // Use odd parity
  }
  else {
    Serial2.begin(115200, SERIAL_8E1);  // Use even parity
  }
  readVolts[0] = batteries[battNum];  // Modify ID byte
  readVolts[6] = lowByte(ModRTU_CRC(readVolts, 6)); // Modify CRC low byte to reflect ID
  readVolts[7] = highByte(ModRTU_CRC(readVolts, 6)); // Modify CRC high byte to reflect ID
  preTransmission(); // Enable RS485 TX
  Serial2.write(readVolts, sizeof(readVolts));  // Send packet of pre defined data with modified bytes above to battery
  Serial2.flush();  // Wait for serial to finish sending
  postTransmission(); // Disable RS485 TX
}

// *****************************************
// Send message to battery to get temperature
// *****************************************
void requestBattTemps(int battNum) {
  if (battNum == 2 || battNum == 4 || battNum == 5 || battNum == 8) // unsure why battery ID 2, 4, 5, and 8 need odd parity but it does
  {
    Serial2.begin(115200, SERIAL_8O1);
  }
  else
  {
    Serial2.begin(115200, SERIAL_8E1);
  }
  readTemps[0] = batteries[battNum];
  readTemps[6] = lowByte(ModRTU_CRC(readTemps, 6));
  readTemps[7] = highByte(ModRTU_CRC(readTemps, 6));
  preTransmission();
  Serial2.write(readTemps, sizeof(readTemps));
  Serial2.flush();
  postTransmission();
}

// *****************************************
// Send message to battery to get current
// *****************************************
void requestBattCurrent(int battNum) {
  if (battNum == 2 || battNum == 4 || battNum == 5 || battNum == 8) // unsure why battery ID 2, 4, 5, and 8 need odd parity but it does
  {
    Serial2.begin(115200, SERIAL_8O1);
  }
  else
  {
    Serial2.begin(115200, SERIAL_8E1);
  }
  readCurrent[0] = batteries[battNum];
  readCurrent[6] = lowByte(ModRTU_CRC(readCurrent, 6));
  readCurrent[7] = highByte(ModRTU_CRC(readCurrent, 6));
  preTransmission();
  Serial2.write(readCurrent, sizeof(readCurrent));
  Serial2.flush();
  postTransmission();
}

// *****************************************
// Send message to battery to get balance
// *****************************************
void requestBattBalance(int battNum) {
  if (battNum == 2 || battNum == 4 || battNum == 5 || battNum == 8) // unsure why battery ID 2, 4, 5, and 8 need odd parity but it does
  {
    Serial2.begin(115200, SERIAL_8O1);
  }
  else
  {
    Serial2.begin(115200, SERIAL_8E1);
  }
  readBalance[0] = batteries[battNum];
  readBalance[6] = lowByte(ModRTU_CRC(readBalance, 6));
  readBalance[7] = highByte(ModRTU_CRC(readBalance, 6));
  preTransmission();
  Serial2.write(readBalance, sizeof(readBalance));
  Serial2.flush();
  postTransmission();
}

// *****************************************
// Send message to battery to get State of Charge (SOC)
// *****************************************
void requestBattSOC(int battNum) {
  if (battNum == 2 || battNum == 4 || battNum == 5 || battNum == 8) // unsure why battery ID 2, 4, 5, and 8 need odd parity but it does
  {
    Serial2.begin(115200, SERIAL_8O1);
  }
  else
  {
    Serial2.begin(115200, SERIAL_8E1);
  }
  readSOC[0] = batteries[battNum];
  readSOC[6] = lowByte(ModRTU_CRC(readSOC, 6));
  readSOC[7] = highByte(ModRTU_CRC(readSOC, 6));
  preTransmission();
  Serial2.write(readSOC, sizeof(readSOC));
  Serial2.flush();
  postTransmission();
}

// *****************************************
// Receive voltage data from battery
// *****************************************
void receiveBatteryVolts(int battNum) {
  byte resV[25];  // Array to temporarily store the received data
  if (Serial2.available() == 0) { // Check if no response
    battData[battNum][VOLT1] = -1;  // No response so store -1 values
    battData[battNum][VOLT2] = -1;
    battData[battNum][VOLT3] = -1;
    battData[battNum][VOLT4] = -1;
    battData[battNum][TOTAL] = -1;
  }
  while (Serial2.available() > 0) { // Data in the serial response buffer
    if (Serial2.available() >= 25) {  // The response should be 25 or more bytes
      countFailures = 0;  // Received response from batt so reset the count failures
      unsigned int bytesToRead = Serial2.available();
      for (byte a = 0; a < bytesToRead; a++) {
        byte b = Serial2.read();  // Read in one byte of data
        resV[a] = b;  // Store the byte received into our temporary array
      }
      battData[battNum][VOLT1] = (resV[9] * 256) + resV[10];  // Store the received data we want into our global array
      battData[battNum][VOLT2] = (resV[11] * 256) + resV[12];
      battData[battNum][VOLT3] = (resV[13] * 256) + resV[14];
      battData[battNum][VOLT4] = (resV[15] * 256) + resV[16];
      //battData[battNum][TOTAL] = (resV[7] * 256) + resV[8];
      battData[battNum][TOTAL] = battData[battNum][VOLT1] + battData[battNum][VOLT2] + battData[battNum][VOLT3] + battData[battNum][VOLT4];

      /* if (battNum == 7) {
        Serial.print("RX Vol: ");
        for ( byte a = 0; a < sizeof(resV); a++) {
          Serial.print(resV[a]); Serial.print(", ");
        }
        Serial.println(" ");
      } */

      // Clean out the buffer after reading the response
      while (Serial.available()) {
        Serial.read();
      }

    }
    else {  // Clear partial data in serial response buffer
      while (Serial.available()) {
        Serial.println("RX wrong V packet");
        Serial.read();
      }
      battData[battNum][VOLT1] = -1;  // No valid response so store -1 values
      battData[battNum][VOLT2] = -1;
      battData[battNum][VOLT3] = -1;
      battData[battNum][VOLT4] = -1;
      battData[battNum][TOTAL] = -1;
      break;
    }
  }
}

// *****************************************
// Receive temperature data from battery
// *****************************************
void receiveBatteryTemps(int battNum) {
  byte resT[21];
  if (Serial2.available() == 0) { // Check if no response
    battData[battNum][TEMP1] = -1;  // No response so store -1 values
    battData[battNum][TEMP2] = -1;
    battData[battNum][TEMP3] = -1;
    battData[battNum][TEMP4] = -1;
    battData[battNum][TEMP5] = -1;
  }
  while (Serial2.available() > 0)
  {
    if (Serial2.available() >= 21) {
      countFailures = 0;  // Received response from batt so reset the count failures
      unsigned int bytesToRead = Serial2.available();
      for (byte a = 0; a < bytesToRead; a++) {
        byte b = Serial2.read();
        resT[a] = b;
      }
      battData[battNum][TEMP1] = (resT[5] * 256) + resT[6];
      battData[battNum][TEMP2] = (resT[7] * 256) + resT[8];
      battData[battNum][TEMP3] = (resT[9] * 256) + resT[10];
      battData[battNum][TEMP4] = (resT[11] * 256) + resT[12];
      battData[battNum][TEMP5] = (resT[3] * 256) + resT[4];

      /* if (battNum == 7) {
        Serial.print("RX Tem: ");
        for ( byte a = 0; a < sizeof(resT); a++) {
          Serial.print(resT[a]); Serial.print(", ");
        }
        Serial.println(" ");
      } */

      //clean out the buffer after reading the response
      while (Serial.available()) {
        Serial.println("RX wrong T packet");
        Serial.read();
      }
    }
    else {  // Clear partial data in serial response buffer
      while (Serial.available()) {
        Serial.read();
      }
      battData[battNum][TEMP1] = -1;  // No valid response so store -1 values
      battData[battNum][TEMP2] = -1;
      battData[battNum][TEMP3] = -1;
      battData[battNum][TEMP4] = -1;
      battData[battNum][TEMP5] = -1;
      break;
    }
  }
}

// *****************************************
// Receive current data from battery
// *****************************************
void receiveBatteryCurrent(int battNum) {
  int currentcalc;
  byte resC[27];
  if (Serial2.available() == 0) { // Check if no response
    battData[battNum][CURNT] = 0;  // No response so store 0 value
  }
  while (Serial2.available() > 0)
  {
    if (Serial2.available() >= 27) {
      countFailures = 0;  // Received response from batt so reset the count failures
      unsigned int bytesToRead = Serial2.available();
      for (byte a = 0; a < bytesToRead; a++) {
        byte b = Serial2.read();
        resC[a] = b;
      }
      currentcalc = (resC[17] * 256) + resC[18];  // combine the two bytes into a single 16 bit number
      if (currentcalc > 32767){ // convert to a negative value
        currentcalc = currentcalc - 65535;
      }
      battData[battNum][CURNT] = currentcalc; // store into our global array

      /* if (battNum == 7) {
        Serial.print("RX Cur: ");
        for ( byte a = 0; a < sizeof(resC); a++) {
          Serial.print(resC[a]); Serial.print(", ");
        }
        Serial.println(" ");
      } */

      //clean out the buffer after reading the response
      while (Serial.available()) {
        Serial.println("RX wrong C packet");
        Serial.read();
      }
    }
    else {  // Clear partial data in serial response buffer
      while (Serial.available()) {
        Serial.read();
      }
      battData[battNum][CURNT] = 0;  // No valid response so store 0 value
      break;
    }
  }
}

// *****************************************
// Receive balance data from battery
// *****************************************
void receiveBatteryBalance(int battNum) {
  byte resB[9];
  if (Serial2.available() == 0) { // Check if no response
    battData[battNum][BALAN] = 0;  // No response so store 0 value
  }
  while (Serial2.available() > 0)
  {
    if (Serial2.available() >= 9) {
      countFailures = 0;  // Received response from batt so reset the count failures
      unsigned int bytesToRead = Serial2.available();
      for (byte a = 0; a < bytesToRead; a++) {
        byte b = Serial2.read();
        resB[a] = b;
      }
      battData[battNum][BALAN] = bitRead(resB[3], 0); // store into our global array

      //if (battNum <= 8) {
      if (resB[3] > 1) {
        Serial.print("RX Bal: ");
        for ( byte a = 0; a < sizeof(resB); a++) {
          if (a == 3 || a ==4){
            //Serial.print(resB[a]); Serial.print(", ");  // decimal display
            Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(resB[a])); // binary display
          }
          else{
            Serial.printf("%02X", resB[a]); // Hex display
          }
          Serial.print(", ");
        }
        Serial.println(" ");
      }

      //clean out the buffer after reading the response
      while (Serial.available()) {
        Serial.read();
      }
    }
    else {  // Clear partial data in serial response buffer
      while (Serial.available()) {
        Serial.println("RX wrong B packet");
        Serial.read();
      }
      battData[battNum][BALAN] = 0;  // No valid response so store 0 value
      break;
    }
  }
}

// *****************************************
// Receive SOC data from battery
// *****************************************
void receiveBatterySOC(int battNum) {
  byte resS[31];
  if (Serial2.available() == 0) { // Check if no response
    battData[battNum][BTSOC] = 0;  // No response so store 0 value
  }
  while (Serial2.available() > 0)
  {
    if (Serial2.available() >= 31) {
      countFailures = 0;  // Received response from batt so reset the count failures
      unsigned int bytesToRead = Serial2.available();
      for (byte a = 0; a < bytesToRead; a++) {
        byte b = Serial2.read();
        resS[a] = b;
      }
      battData[battNum][BTSOC] = (resS[3] << 8) + resS[4]; // combine and store into global array

      /* if (battNum == 7) {
        Serial.print("RX SOC: ");
        for ( byte a = 0; a < sizeof(resS); a++) {
          Serial.print(resS[a]); Serial.print(", ");
        }
        Serial.println(" ");
      } */

      //clean out the buffer after reading the response
      while (Serial.available()) {
        Serial.read();
      }
    }
    else {  // Clear partial data in serial response buffer
      while (Serial.available()) {
        Serial.println("RX wrong S packet");
        Serial.read();
      }
      battData[battNum][BTSOC] = 0;  // No valid response so store 0 value
      break;
    }
  }
}

// *****************************************
// Checks the battery data to get the current status
// *****************************************
void checkBattStatus() {
  for (byte battNum = 0; battNum < sizeof(batteries); battNum++) {
    // Check Volts
    // Check and make sure batt is communicating, if it's not it'll return -1 values
    if (battData[battNum][VOLT1] == -1 ||
        battData[battNum][VOLT2] == -1 ||
        battData[battNum][VOLT3] == -1 ||
        battData[battNum][VOLT4] == -1) {
      battData[battNum][VSTAT] = -1;
      Serial.print(battNum + 1);
      Serial.println("_battData VOLT = -1");
    }
    else if (battData[battNum][VOLT1] >= battHighV ||
             battData[battNum][VOLT2] >= battHighV ||
             battData[battNum][VOLT3] >= battHighV ||
             battData[battNum][VOLT4] >= battHighV) {
      // Change this to suit your high voltage threshold strategy.
      // You can disconnect both, or just the pv array. Since there is a possibility of power being
      // backfed through a circuit that is normally drawing power, I am defaulting to shutting everything down.
      battData[battNum][VSTAT] = 2; // VSTAT = 2 means high voltage
      Serial.print(battNum + 1);
      Serial.print("_battData ");
      Serial.print(battNum + 1);
      Serial.print(" battHighV ");
      Serial.print(battData[battNum][VOLT1]);
      Serial.print(" ");
      Serial.print(battData[battNum][VOLT2]);
      Serial.print(" ");
      Serial.print(battData[battNum][VOLT3]);
      Serial.print(" ");
      Serial.println(battData[battNum][VOLT4]);
      shutdownPv(); // Turn off charge relay but leave on Load relay
    }
    else if (battData[battNum][VOLT1] <= battLowV ||
             battData[battNum][VOLT2] <= battLowV ||
             battData[battNum][VOLT3] <= battLowV ||
             battData[battNum][VOLT4] <= battLowV) {
      // Ideally, we could use a 3rd relay to shut off load and allow the battery to charge in this condition,
      // but for now we have to disconnect everything.
      battData[battNum][VSTAT] = 1; // VSTAT = 1 means low voltage
      Serial.print(battNum + 1);
      Serial.print("_battData ");
      Serial.print(battNum + 1);
      Serial.print(" battLowV ");
      Serial.print(battData[battNum][VOLT1]);
      Serial.print(" ");
      Serial.print(battData[battNum][VOLT2]);
      Serial.print(" ");
      Serial.print(battData[battNum][VOLT3]);
      Serial.print(" ");
      Serial.println(battData[battNum][VOLT4]);
      shutdownAll();  // Turn off both Load and Charge relays
    }
    else {
      // All is good. We'll do stuff here eventually when implementing auto reconnect.
      battData[battNum][VSTAT] = 0; // VSTAT = 0 means within normal range
      turnonAll();  // Turn on both Load and Charge relays
    }

    // Check Temps
    // Check and make sure batt is communicating, if it's not it'll return -1 values
    if (battData[battNum][TEMP1] == -1 ||
        battData[battNum][TEMP2] == -1 ||
        battData[battNum][TEMP3] == -1 ||
        battData[battNum][TEMP4] == -1 ||
        battData[battNum][TEMP5] == -1) {
      battData[battNum][TSTAT] = -1;
      Serial.print(battNum + 1);
      Serial.println("_battData TEMP = -1");
    }
    else if (battData[battNum][TEMP1] >= battHighT ||
             battData[battNum][TEMP2] >= battHighT ||
             battData[battNum][TEMP3] >= battHighT ||
             battData[battNum][TEMP4] >= battHighT ||
             battData[battNum][TEMP5] >= battHighT) {
      battData[battNum][TSTAT] = 2; // TSTAT = 2 means high temperature
      Serial.print(battNum + 1);
      Serial.print("_battData ");
      Serial.print(battNum + 1);
      Serial.print(" battHighT ");
      Serial.print(battData[battNum][TEMP1]);
      Serial.print(" ");
      Serial.print(battData[battNum][TEMP2]);
      Serial.print(" ");
      Serial.print(battData[battNum][TEMP3]);
      Serial.print(" ");
      Serial.print(battData[battNum][TEMP4]);
      Serial.print(" ");
      Serial.println(battData[battNum][TEMP5]);
      shutdownAll();  // Turn off the Load and Charge relays
    }
    else if (battData[battNum][TEMP1] <= battLowT ||
             battData[battNum][TEMP2] <= battLowT ||
             battData[battNum][TEMP3] <= battLowT ||
             battData[battNum][TEMP4] <= battLowT ||
             battData[battNum][TEMP5] <= battLowT) {
      // We can allow the batteries to dischage, but we don't want to charge them
      battData[battNum][TSTAT] = 1; // TSTAT = 1 means low temperature
      Serial.print(battNum + 1);
      Serial.print("_battData ");
      Serial.print(battNum + 1);
      Serial.print(" battLowT ");
      Serial.print(battData[battNum][TEMP1]);
      Serial.print(" ");
      Serial.print(battData[battNum][TEMP2]);
      Serial.print(" ");
      Serial.print(battData[battNum][TEMP3]);
      Serial.print(" ");
      Serial.print(battData[battNum][TEMP4]);
      Serial.print(" ");
      Serial.println(battData[battNum][TEMP5]);
      shutdownPv(); // Turn off the Charge relay
    }
    else
    {
      // All is good. We'll do stuff here eventually when implementing auto reconnect.
      battData[battNum][TSTAT] = 0;
      turnonAll();  // Turn on both Load and Charge relays
    }
  }
}

// *****************************************
// Calculate cell voltage delta for each battery and store it in VDELT location
// *****************************************
void calcBattCellDelta() {
  for (byte i = 0; i < sizeof(batteries); i++) {
    if ((battData[i][VOLT1] == -1) || (battData[i][VOLT1] == -1) || (battData[i][VOLT1] == -1) || (battData[i][VOLT1] == -1)) {
      battData[i][VDELT] = -1;
      return;
    }
    int battcellvolt1 = battData[i][VOLT1];
    int battcellvolt2 = battData[i][VOLT2];
    int battcellvolt3 = battData[i][VOLT3];
    int battcellvolt4 = battData[i][VOLT4];
    int battcellhighest;
    int battcelllowest;
    int battcelldelta;

    // Find highest value
    battcellhighest = battcellvolt1;
    if (battcellvolt2 >= battcellhighest) {
      battcellhighest = battcellvolt2;
    }
    if (battcellvolt3 >= battcellhighest) {
      battcellhighest = battcellvolt3;
    }
    if (battcellvolt4 >= battcellhighest) {
      battcellhighest = battcellvolt4;
    }
//    Serial.print("High=");
//    Serial.print(battcellhighest);
//    Serial.print(" ");

    // Find lowest value
    battcelllowest = battcellvolt1;
    if (battcellvolt2 <= battcelllowest) {
      battcelllowest = battcellvolt2;
    }
    if (battcellvolt3 <= battcelllowest) {
      battcelllowest = battcellvolt3;
    }
    if (battcellvolt4 <= battcelllowest) {
      battcelllowest = battcellvolt4;
    }
//    Serial.print("Low=");
//    Serial.print(battcelllowest);
//    Serial.print(" ");

    // Calculate difference and store it
    battcelldelta = battcellhighest - battcelllowest;
    battData[i][VDELT] = battcelldelta;

    /* if (battData[i][BALAN] == 1) {
      Serial.print("Batt ");
      Serial.print(i + 1);
      Serial.print(" Balance ");
      Serial.print("Delta = ");
      Serial.print(battcelldelta);
      Serial.print(" mV, ");
      Serial.print("Low=");
      Serial.print(battcelllowest);
      Serial.print(" mV, ");
      Serial.print("High=");
      Serial.print(battcellhighest);
      Serial.println(" mV");
    } */
  }
}

// *****************************************
// Sets actual relay state
// *****************************************
void relayWrite(unsigned int relayNum, bool state) {  // relayNum is the pin #
  pinMode(relayNum, OUTPUT);  // Makes sure the IO is properly set to output
  if (state)  // if the state passed is 1 or ON, it turns on the relay
  {
    digitalWrite(relayNum, ON);
    return;
  }
  digitalWrite(relayNum, OFF);
  return;
}

// *****************************************
// Data logging and console output all in one!
// *****************************************
void logData(String data) {
  Serial.print(data);
  //Serial1.print(data); // Send out serial 1 (Bluetooth?)
}

// *****************************************
// Change RS485 to Transmit
// *****************************************
void preTransmission() {
  digitalWrite(enablePin, TRANSMIT);
}

// *****************************************
// Change RS485 to Receive
// *****************************************
void postTransmission() {
  digitalWrite(enablePin, RECEIVE);
}

// *****************************************
// Send a message to bus to wake up the batteries
// *****************************************
void wakeup() {
  Serial2.begin(9600, SERIAL_8N2);  // Note that the wakeup uses only 9600 baud with 2 stop bits
  preTransmission();  // Enable the RS485 TX
  Serial2.write(messageW, sizeof(messageW));  // Send our pre defined message array
  Serial2.flush();
  postTransmission(); // Disable the RS485 TX
}

// *****************************************
// Turn on all relays
// *****************************************
void turnonAll() {
  // Set the Load relay to ON
  if (loadRelay1status == false) { // If it was previously off, we neet to step the PWM to turn it on
    ledcWrite(loadRelayPWM_Channel, 255); // Initially we turn on the relay hard with full voltage
    delay(10);
    ledcWrite(loadRelayPWM_Channel, 128); // Now the relay uses less power to hold it on with half voltage, 50% duty
  }else{
    ledcWrite(loadRelayPWM_Channel, 128); // Keep the relay on with half voltage, 50% duty
  }
  loadRelay1status = true;  // Relay status now set to true so next time it stays half voltage without step
  relayWrite(loadRelay1, loadRelay1status); // Set the non-PWM relay output
  
  // Set the Charge relay to ON
  if (chargeRelay2status == false) { // If it was previously off, we neet to step the PWM to turn it on
    ledcWrite(chargeRelayPWM_Channel, 255); // Initially we turn on the relay hard with full voltage
    delay(10);
    ledcWrite(chargeRelayPWM_Channel, 128); // Now the relay uses less power to hold it on with half voltage, 50% duty
  }else{
    ledcWrite(chargeRelayPWM_Channel, 128); // Keep the relay on with half voltage, 50% duty
  }
  chargeRelay2status = true;  // Relay status now set to true so next time it stays half voltage without step
  relayWrite(chargeRelay2, chargeRelay2status); // Set the non-PWM relay output
}

// *****************************************
// Turn off all relays
// *****************************************
void shutdownAll() {
  // Shutdown pv array first
  chargeRelay2status = false;
  relayWrite(chargeRelay2, chargeRelay2status); // Shut off non-PWM relay output
  ledcWrite(chargeRelayPWM_Channel, 0); // Shut off PWM relay output, 0% duty
  
  loadRelay1status = false;
  relayWrite(loadRelay1, loadRelay1status); // Shut off non-PWM relay output
  ledcWrite(loadRelayPWM_Channel, 0); // Shut off PWM relay output, 0% duty
}

// *****************************************
// Turn off only the PV (relay 2) relay
// *****************************************
void shutdownPv() {
  // Shutdown pv array, leave battery alone.
  chargeRelay2status = false;
  relayWrite(chargeRelay2, chargeRelay2status); // Shut off non-PWM relay output
  ledcWrite(chargeRelayPWM_Channel, 0); // Shut off PWM relay output, 0% duty
}

