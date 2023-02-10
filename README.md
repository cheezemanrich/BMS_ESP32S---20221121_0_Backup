# BMS_ESP32S---20221121_0_Backup

NOTE: This is an As-Is rough prototype of software and hardware.
      It is in no way ready for release and has no guarantees.
      This is open source andyou are free to do with it as you like.

Project to communicate with Valence U27-12XP battery BMS module via RS485 using an ESP32 module.

Developed with VSCode and PlatformIO

Features:
    -Uses inexpensive ESP32-DevKitC module
    -Drives color LCD module over SPI
    -Built in web server to view near real time data
    -Configured for 4 parrallel and 2 series batteries (24V)
    -Cayenne MQTT
  
Note that this project can only compile under older Espressif 3.5.0 version.

Web page files need to uploaded to SPIFFS (Terminal menu > Run task.. > PlatformIO: Upload Filesystem Image)

There is a known bug that causes reboot due to a Watchdog triggered by the Com port driver.
This is supposedly fixed in later Espressif versions but I couldn't get it to compile and don't have the time to debug it.

Some of the info used to develop this came from the DIY Solar Power Forum:
  https://diysolarforum.com/threads/interfacing-with-valence-built-in-monitoring.2183/
  https://diysolarforum.com/threads/valence-xp-super-thread.5683/
