#include <XBee.h>

// create the XBee object
XBee xbee = XBee();

uint8_t payload[] = { 0, 0 ,0 };
//enviar a router 
// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40B7D717);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int pin1 = 0;
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDbrightness;        //
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  pinMode(pin1,INPUT);
}

void loop() {   
   photocellReading = analogRead(photocellPin);  
 
  // LED gets brighter the darker it is at the sensor
  // that means we have to -invert- the reading from 0-1023 back to 1023-0
  photocellReading = 1023 - photocellReading;
  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
  
  payload[0] = 0x90;
  payload[1] = 0x60;
  
  xbee.send(zbTx);
  delay(1000);
  
}
