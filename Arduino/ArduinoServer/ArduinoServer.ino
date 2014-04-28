/*
  Modified bidirectional TCP Server (by Laurid Meyer) 
  and ChatServer (by Limor Fried and Kevin Townsend)
  
  created 17.04.2014
  by Bruna Seewald
  
*/
#include<SPI.h>
#include<Adafruit_CC3000.h>
#include "utility/socket.h"

//CC3000 PINS
#define ADAFRUIT_CC3000_IRQ   3 
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); 

//WI-FI CONFIGURATION
#define WLAN_SSID       "fsilva"             
#define WLAN_PASS       "bru2465betinharml"
#define WLAN_SECURITY   WLAN_SEC_WPA2

//SERVER
#define LISTEN_PORT           5000          
Adafruit_CC3000_Server chatServer(LISTEN_PORT); 

//API FRAME
const byte frameStartByte = 0x7E;
const byte frameTypeRemoteAT = 0x17;
const byte remoteATOptionApplyChanges = 0x02;

void setup(void) {
  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  
  /* Initialise the module */
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  /* Optional setting a static IP Address  */
  unsigned long IPAdd[1] = {0x6A00A8C0};       //192.168.0.106
  unsigned long SubNetMask[1] = {0x00FFFFFF};  //225.255.255.0
  unsigned long dfGW[1] = {0x0100A8C0};        //192.168.0.1
  unsigned long DNSServer[1] = {0x0100A8C0};   //192.168.0.1

  /*
  unsigned long IPAdd[4] = {0x00};
  unsigned long SubNetMask[4] = {0x00};
  unsigned long dfGW[4] = {0x00};
  unsigned long DNSServer[4] = {0x00};
  */

  if (!cc3000.setStaticIPAddress(IPAdd, SubNetMask, dfGW, DNSServer))
  {
    Serial.println(F("Failed to set static IP"));
    while(true);
  }    
  /* End setting static IP Address*/
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  //while (!cc3000.checkDHCP())
  //{
    //delay(100); // ToDo: Insert a DHCP timeout!
  //}  
  
  // Start listening for connections
  chatServer.begin();
  
  //TODO: pins
}  

void loop(void)
{
  //Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = chatServer.available();
  if (client) {
    //Command string where incoming commands are stored
    String commandStr ="";
    
    //if a client is connected
    while (client.connected()) {
            
      //if client is sending
      if (client.available()) {
        //reading the inputs from the client
        //and adds them to the command String
        char c = client.read();
        commandStr+=c;
        
        //if a newline character is sent (command line is fully recieved)
        if (c == '\n') {
          //output the command
          Serial.println("Command:"+commandStr);
          
          //if the command begins with "set..."
          if(commandStr.indexOf("setOnMove")==0){
            //TODO: set the xbee module
            toogleRemotePin(1,0x00,0x00,0x00,0x00);
          }
          if(commandStr.indexOf("setOffMove")==0){
            //TODO: set the xbee module
            toogleRemotePin(0);
          }
          if(commandStr.indexOf("setOnGas")==0){
            //TODO: set the xbee module
            toogleRemotePin(1);
          }
          if(commandStr.indexOf("setOffGas")==0){
            //TODO: set the xbee module
            toogleRemotePin(0);
          }
          if(commandStr.indexOf("setOnLight")==0){
            //TODO: set the xbee module
            toogleRemotePin(1);
          }
          if(commandStr.indexOf("setOffLight")==0){
            //TODO: set the xbee module
            toogleRemotePin(0);
          }
          if(commandStr.indexOf("setOnTemp")==0){
            //TODO: set the xbee module
            toogleRemotePin(1);
          }
          if(commandStr.indexOf("setOffTemp")==0){
            //TODO: set the xbee module
            toogleRemotePin(0);
          }
          if(commandStr.indexOf("tempValue")==0){
            //Fake command
            chatServer.println("setTempValue:25");
            //TODO: set the xbee module
          }          
                    
          //reset the commandline String
          commandStr="";
        } 
      }
    }
    // give the client time to receive the data
    delay(1);
    // close the connection:
    client.close();
  }
}

byte sendByte(byte value){
  Serial.write(value); //VERIFICAR SERIAL!!! 
  return value;  
}

void toogleRemotePin(int value, byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){
  
  byte pin_state;
  // 1 = ON, 0 = OFF
  if(value){
    pin_state = 0x5;
  } else{
    pin_state = 0x4;
  }  
  
  // Begin the API frame
  sendByte(frameStartByte); 
  
  // High and low parts of the frame length (not counting checksum) 
  sendByte(0x0);
  sendByte(0x10);

  // Accumulate the checksum
  long sum = 0; 

  // Indicate this frame contains a Remote AT command
  sum += sendByte(frameTypeRemoteAT);
  // frame ID set to zero for no reply 
  sum += sendByte(0x0); 
  
  // The following 8 bytes indicate the ID of the recipient. 
  // DH: 0x0013A200 
  sum += sendByte(0x0);
  sum += sendByte(0x13);
  sum += sendByte(0xA2); 
  sum += sendByte(0x0); 
  // DL: Depends on the sensor
  sum += sendByte(sensorAddress0); 
  sum += sendByte(sensorAddress1); 
  sum += sendByte(sensorAddress2); 
  sum += sendByte(sensorAddress3);

  // The following 2 bytes indicate the 16-bit address of the recipient. 
  // Use 0xFFFE to broadcast to all nodes.
  sum += sendByte(0xFF);
  sum += sendByte(0xFF);
  
  // Send Remote AT options
  sum += sendByte(remoteATOptionApplyChanges); 

  // The text of the AT command 
  sum += sendByte('D');
  sum += sendByte('1');

  // The value (0x4 for off, 0x5 for on) 
  sum += sendByte(pin_state);
  
  // Send the checksum
  sendByte( 0xFF - ( sum & 0xFF));

  // Pause to let the microcontroller settle down if needed
  delay(10); 
}

