#include <XBee.h>
#include <UIPEthernet.h> // Used for Ethernet
/**************CONFIGURACION DEL ETHERNET**************/
// **** ETHERNET SETTING ****
// Arduino Uno pins: 10 = CS, 11 = MOSI, 12 = MISO, 13 = SCK
// Ethernet MAC address - must be unique on your network - MAC Reads T4A001 in hex (unique in your network)
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };                                       
// For the rest we use DHCP (IP address and such)
EthernetClient client;
char server[] = "192.168.10.125"; // IP Adres (or name) of server to dump data to

IPAddress ip(192,168,10,102);
IPAddress dns_server(8,8,8,8);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,10,2);

/*********************************************************/
/*************CONFIGURACION DEL XBEE COORDI****************/
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
/*********************************************************/

void setup() {
  // start serial
  Serial.begin(9600);
  xbee.begin(Serial);
  Ethernet.begin(mac, ip,dns_server, gateway,subnet);
  
  Serial.println("Tweaking4All.com - Temperature Drone - v2.0");
  Serial.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP());
  
  pinMode(6,OUTPUT);
  
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
  delay(1000);
  xbee.readPacket();
     if (xbee.getResponse().isAvailable()) {
      // got something
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
        // I check both bytes (or)
        if (rx.getData(0) == 0x34 || rx.getData(0) == 0x76 ) {
            if (client.connect(server, 80)) {
              //Serial.println("-> Connected");
              // Make a HTTP request:
              client.print( "GET /internetOfThings/iot.php?");
              client.print("valor=");
          //    client.print( temp  );
              //client.print("&&");
              //client.print("temperature=");
              client.print( "12.3" );
              client.println( " HTTP/1.1");
              client.print( "Host: " );
              client.println(server);
              client.println( "Connection: close" );
              client.println();
              client.println();
              client.stop();
            }
            else {
              // you didn't get a connection to the server:
              Serial.println("--> connection failed/n");
              Ethernet.begin(mac, ip,dns_server, gateway,subnet);
            }
          digitalWrite(6, HIGH);
          delay(1000);
          digitalWrite(6, LOW);
          delay(1000);
       }
       else{
         digitalWrite(6,LOW);
       }
      } 
   }
}
