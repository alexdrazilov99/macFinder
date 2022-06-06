/*
 UDPSendReceiveString:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender

 A Processing sketch is included at the end of file that can be used to send
 and received messages for testing with a computer.

 created 21 Aug 2010
 by Michael Margolis

 This code is in the public domain.
 */

#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 32, &Wire);

byte mac[] = {0xc1, 0x00, 0x00, 0xad, 0x4d, 0xc3};
IPAddress ip(192, 168, 0, 1);

enum state {
  SEARCHING, UNPLUGGED, FOUND
  };

state currentState = FOUND;

unsigned int localPort = 67;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[300];  // buffer to hold incoming packet,
String clientMAC = "";
String prevMAC = "";

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
//  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }  

  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  display.display();
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("Setting up");
  display.display();
  delay(250); // Let message play

  // start the Ethernet
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    //Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }

  Udp.begin(localPort);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Searching for device...");
  display.display();
}

void loop() {
  // if there's data available, read a packet
  
  if(currentState != FOUND){
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      currentState = FOUND;
      prevMAC = clientMAC;
          
      // read the packet into packetBufffer
      Udp.read(packetBuffer, 300);
  
      String packet1 = String(packetBuffer[28], HEX);
      String packet2 = String(packetBuffer[29], HEX);
      String packet3 = String(packetBuffer[30], HEX);
      String packet4 = String(packetBuffer[31], HEX);
      String packet5 = String(packetBuffer[32], HEX);
      String packet6 = String(packetBuffer[33], HEX);
      
      clientMAC = packet1[packet1.length() - 2];
      clientMAC += packet1[packet1.length() - 1];
      clientMAC += ":";
      clientMAC += packet2[packet2.length() - 2];
      clientMAC += packet2[packet2.length() - 1];
      clientMAC += ":";
      clientMAC += packet3[packet3.length() - 2];
      clientMAC += packet3[packet3.length() - 1];
      clientMAC += ":";
      clientMAC += packet4[packet4.length() - 2];
      clientMAC += packet4[packet4.length() - 1];
      clientMAC += ":";
      clientMAC += packet5[packet5.length() - 2];
      clientMAC += packet5[packet5.length() - 1];
      clientMAC += ":";
      clientMAC += packet6[packet6.length() - 2];
      clientMAC += packet6[packet6.length() - 1];

      if(prevMAC != clientMAC){    
        //Serial.println(clientMAC);

        display.clearDisplay();
        display.setCursor(0,0);
        display.print(clientMAC);
        display.display();
      }
    }
  }
  else if(currentState != SEARCHING){
    currentState = SEARCHING;
  }
  delay(10);
}
