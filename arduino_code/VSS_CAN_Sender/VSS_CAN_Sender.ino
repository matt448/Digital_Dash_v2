// Matthew McMillan 
// @matthewmcmillan
// http://matthewcmcmillan.blogspot.com
//
// VSS to CAN Sender module
//
// Counts VSS pulses and converts them into MPH and KPH speeds


#include "mcp_can.h"
#include <SPI.h>
#include <stdio.h>

////////////////////
//CAN controller pin
MCP_CAN CAN(10);

/////////////////
// CAN ID
int canId = 0x12C;  //Decimal value: 300

////////////////////////
// VSS pulses per mile
const float pulsesPerMile = 4000; // this is pulses per mile for Toyota. Other cars are different.

const int samplePeriod = 250; //Pulse counter sample period in milliseconds
const float convertMph = pulsesPerMile/3600;
unsigned int count;
float mph;
float kph;
unsigned int imph;
unsigned int ikph;
int roundedMph;
int roundedKph;

void setup()
{
  //Initialize hardware pulse counter on pin 5
  TCCR1A = 0; //Configure hardware counter 
  TCNT1 = 0;  // Set hardware counter to zero
  
  //Initialize serial communication
  Serial.begin(115200);
  
  //Initialize CAN bus, baudrate: 500k
  if(CAN.begin(CAN_500KBPS) ==CAN_OK) Serial.println("CAN Init OK");
  else Serial.println("CAN init fail. Check SPI wiring.");
}

//Some sample CAN messages
//unsigned char msg1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
//unsigned char msg2[8] = {0xFF, 0x01, 0x10, 0x0A, 0x00, 0x00, 0x00, 0x00};
//unsigned char msg3[4] = {0xFF, 0x01, 0x10, 0x0A};


void loop()
{

  /////////////////////////////////////////////////////////////
  // This uses the hardware pulse counter on the Arduino.
  //
  bitSet(TCCR1B, CS12); // start counting pulses
  bitSet(TCCR1B, CS11); // Clock on rising edge
  delay(samplePeriod); // Allow pulse counter to collect for samplePeriod
  TCCR1B = 0; // stop counting
  count = TCNT1; // Store the hardware counter in a variable
  TCNT1 = 0;     // Reset hardware counter to zero
  
  // For a 250ms sample period we need to multiply by 4 to get
  // pulses for a 1 second period. Speeds are based on 1 sec measurement
  // periods but 250ms allows for increased message frequency.
  count = count * 4; 
  Serial.print("Pulse count: ");
  Serial.println(count);
  
  mph = (count/convertMph)*100; // Convert pulse count into 100x mph.
  
  kph = mph*1.60934;  //Convert 100x mph to 100x kph
  
  imph = (unsigned int) mph; // Cast to integer. 100x allows retaining 10th of mph resolution.
  ikph = (unsigned int) kph; // Cast to integer. 100x allows retaining 10th of mph resolution.

  int mx = imph / 100;
  int my = imph % 100;
  int kx = ikph / 100;
  int ky = ikph % 100;
  
  // Round to speed to whole number
  if(my >= 50){
    roundedMph = mx + 1;
  }else{
    roundedMph = mx;
  }
  
  if(ky >= 50){
    roundedKph = kx + 1;
  }else{
    roundedKph = kx;
  }
  
  Serial.print("roundedMph: ");
  Serial.println(roundedMph);
  Serial.print("roundedKph: ");
  Serial.println(roundedKph);
  Serial.println();
  //Create data packet for CAN message. Sends both Mph and Kph.
  unsigned char canMsg[8] = {roundedMph, roundedKph, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // send data:  (CAN_ID, ID_TYPE, DLC, DATA_BUF)
  CAN.sendMsgBuf(canId, 0, 8, canMsg);  
  
}

