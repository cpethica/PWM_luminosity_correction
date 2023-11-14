// Please include ArtnetEther.h to use Artnet on the platform
// which can use both WiFi and Ethernet
#include <ArtnetEther.h>
// this is also valid for other platforms which can use only Ethernet
 #include <Artnet.h>
#include "cie1931.h"      // header file can be adapted for varying pwm bit rates

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// called this way, it uses the default address 0x40
// you can also call it with a different address you want
//Adafruit_PWMServoDriver servo1 = Adafruit_PWMServoDriver(0x40);
//Adafruit_PWMServoDriver servo2 = Adafruit_PWMServoDriver(0x42);
Adafruit_PWMServoDriver light1 = Adafruit_PWMServoDriver(0x43);
Adafruit_PWMServoDriver light2 = Adafruit_PWMServoDriver(0x44);



// Ethernet stuff
const IPAddress ip(192, 168, 0, 201);
uint8_t mac[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};

ArtnetReceiver artnet;
uint32_t universe1 = 1;  // 0 - 15
uint32_t universe2 = 2;  // 0 - 15

uint8_t dmx_states[32];

void callback(const uint8_t* data, const uint16_t size) {
       for (int i = 0; i < 32; ++i) {
            dmx_states[i] = int(data[i]);
        }
}

void setup() {
    Serial.begin(115200);

    Ethernet.begin(mac, ip);
    artnet.begin();


  //ligths
  light1.begin();
  light1.setPWMFreq(1600);  // Light
  light2.begin();
  light2.setPWMFreq(1600);  // Light


    artnet.subscribe(universe2, callback);
}

void loop() {
    artnet.parse();  // check if artnet packet has come and execute callback
    
}

void lightSet() {
  for (int i = 0; i < 32; i++) {
      
      int pulselen = CIE[dmx_states[i]];   // lookup value from table based on dmx value
     
      if (i < 16) {
        light1.setPin(i % 16, pulselen, 1); // setPin special do deal with 0
      } else {
        light2.setPin(i % 16, pulselen, 1);
      }
    }
}
