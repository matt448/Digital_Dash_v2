////////////////////////////////////////////
//
// Trinket Pro CAN Bus to UART Converter
//
// https://matthewcmcmillan.blogspot.com
// Twitter: matthewmcmillan
//

#include "mcp_can.h"
#include <SPI.h>
#include <stdio.h>
#define INT8U unsigned char

// Connections for Trinket Pro
// ----------------------------
// Digital3 to MCP2515 INT
// Digital10 to MCP2515 CS
// Digital11 to MCP2515 SI
// Digital12 to MCP2515 SO
// Digital13 to MCP2515 SCK
// TX on Trinket is the UART TX Pin

/////////////////////////////////
// Notes / To do
// int.1 is DigitalPin3
//
// It would probably be better to use 
// an External Interupt.
// Using an External Interupt would allow for 
// additional fucntionality because we wouldn't
// just be polling Pin3 the whole time

MCP_CAN CAN0(10); //CAN controller CS pin

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
INT8U buf[8];

void setup()
{
  
  Serial.begin(115200);
  CAN0.begin(CAN_500KBPS);         // init can bus : baudrate = 500
     if(CAN0.begin(CAN_500KBPS) ==CAN_OK)
     {
       Serial.print("CAN INIT OK\r\n");

     } else {
       Serial.print("ERROR: Can init fail\r\n");
       Serial.print("ERROR: Check wiring and reset");
     }
}



void loop()
{
  //The INT pin on the MCP2515 goes low when a message has been recieved.
  if(!digitalRead(3))            // If pin 3 is low, read receive buffer.
    {
      len = 0;
      rxId = 0;
      CAN0.readMsgBuf(&len, rxBuf);       // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();  
      //Print CAN ID and DLC
      Serial.print("CANDATA,");
      Serial.print(rxId);
      Serial.print(",");
      Serial.print(len);
      Serial.print(",");
      //Print data for the CAN packet
      for(int i = 0; i<len; i++)    // print the data
      {
        Serial.print(rxBuf[i]);
        if(i < len-1){
          Serial.print(",");
        }
      }
      Serial.println();
    }
    
}
