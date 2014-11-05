#include <XBee.h>

// create the XBee object
XBee xbee = XBee();

uint8_t payload[] = { 0, 0 ,0 };
//enviar a router 
// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4090C47D);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int pin1 = 0;
float temp;
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  pinMode(pin1,INPUT);
}

void loop() {   
  
  temp = analogRead(pin1);
  
  temp = temp * 0.48828125;

  payload[0] = temp;
  payload[1] = 0x65;
  
  xbee.send(zbTx);
  delay(1000);
}
