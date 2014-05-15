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
//#define WLAN_SSID       "fsilva"             
//#define WLAN_PASS       "bru2465betinharml"
#define WLAN_SSID       "ALLBAGS"             
#define WLAN_PASS       "allbags300"
#define WLAN_SECURITY   WLAN_SEC_WPA2

//SERVER
#define LISTEN_PORT           5000          
Adafruit_CC3000_Server chatServer(LISTEN_PORT); 

//API FRAME
const byte frameStartByte = 0x7E;
const byte frameTypeRemoteAT = 0x17;
const byte remoteATOptionApplyChanges = 0x02;

//BUZZER
#define BUZZER 2
//LED VERDE
#define LED 8

void setup(void) {
  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial1.begin(115200);
  
  /* Initialise the module */
  //Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  /* Optional: Get the SSID list (not available in 'tiny' mode) */
#ifndef CC3000_TINY_DRIVER
  listSSIDResults();
#endif

/*  
  // Optional setting a static IP Address 
  unsigned long IPAdd[1] = {0x6A00A8C0};       //192.168.0.106
  unsigned long SubNetMask[1] = {0x00FFFFFF};  //225.255.255.0
  unsigned long dfGW[1] = {0x0100A8C0};        //192.168.0.1
  unsigned long DNSServer[1] = {0x0100A8C0};   //192.168.0.1
  
  //unsigned long IPAdd[4] = {0x00};
  //unsigned long SubNetMask[4] = {0x00};
  //unsigned long dfGW[4] = {0x00};
  //unsigned long DNSServer[4] = {0x00};
  
  if (!cc3000.setStaticIPAddress(IPAdd, SubNetMask, dfGW, DNSServer))
  {
    Serial.println(F("Failed to set static IP"));
    while(true);
  }    
  // End setting static IP Address
*/
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  
  Serial.println(F("Connected!"));
   
  //while (!cc3000.checkDHCP())
  //{
    //delay(100); // ToDo: Insert a DHCP timeout!
  //} 
 
  /* Display the IP address DNS, Gateway, etc. */  
  while (!displayConnectionDetails()) {
    delay(1000);
  } 
  
  // Start listening for connections
  chatServer.begin();
  //Serial.println(F("Waiting..."));
  
  pinMode(BUZZER,OUTPUT);
  pinMode(LED,OUTPUT);
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
      
      //Client connected
      readSerial();
      //xbeePacket();
            
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
            //Set the xbee module (XBEE 2)
            toggleRemotePin(1,0x40,0x8B,0xAE,0x6A);
            toggleRemotePinMove(1,0x40,0x8B,0xAE,0x6A);
            //Buzzer for On
            setBuzzer(1);
          }
          if(commandStr.indexOf("setOffMove")==0){
            //Set the xbee module (XBEE 2)
            toggleRemotePin(0,0x40,0x8B,0xAE,0x6A);
            toggleRemotePinMove(0,0x40,0x8B,0xAE,0x6A);
            //Buzzer for Off
            setBuzzer(0);
          }
          if(commandStr.indexOf("setOnGas")==0){
            //Set the xbee module (XBEE 1)
            toogleRemotePin(1,0x40,0xB0,0x9D,0x68);
            toggleRemotePinGas(1,0x40,0xB0,0x9D,0x68);
            //Buzzer for On
            setBuzzer(1);
          }
          if(commandStr.indexOf("setOffGas")==0){
            //Set the xbee module (XBEE 1)
            toggleRemotePin(0,0x40,0xB0,0x9D,0x68);
            toggleRemotePinGas(0,0x40,0xB0,0x9D,0x68);
            //Buzzer for Off
            setBuzzer(0);
          }
          if(commandStr.indexOf("setOnLight")==0){
            //Set the xbee module (XBEE 2)
            toggleRemotePin(1,0x40,0x8B,0xAE,0x6A);
            toggleRemotePinLight(1,0x40,0x8B,0xAE,0x6A);
            //Buzzer for On
            setBuzzer(1);
          }
          if(commandStr.indexOf("setOffLight")==0){
            //Set the xbee module (XBEE 2)
            toggleRemotePin(0,0x40,0x8B,0xAE,0x6A);
            toggleRemotePinLight(0,0x40,0x8B,0xAE,0x6A);
            //Buzzer for Off
            setBuzzer(0);
          }
          if(commandStr.indexOf("setOnTemp")==0){
            //Set the xbee module (XBEE 1)
            toggleRemotePin(1,0x40,0xB0,0x9D,0x68);
            toggleRemotePinTemp(1,0x40,0xB0,0x9D,0x68);
            //Buzzer for On
            setBuzzer(1);
          }
          if(commandStr.indexOf("setOffTemp")==0){
            //Set the xbee module (XBEE 1)
            toggleRemotePin(0,0x40,0xB0,0x9D,0x68);
            toggleRemotePinTemp(0,0x40,0xB0,0x9D,0x68);
            //Buzzer for Off
            setBuzzer(0);           
          }
          if(commandStr.indexOf("tempValue")==0){
            //Fake command
            //chatServer.println("setTempValue:25");
            queriedSample(0x40,0xB0,0x9D,0x68);
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
  
  //Client not connected
  readSerial();
  //xbeePacket();
  
}

byte sendByte(byte value){
  Serial1.write(value);  
  return value;  
}

void toggleRemotePin(int value, byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){
  
  byte pin_state;
  // 1 = SENSOR ON, 0 = SENSOR OFF
  if(value){
    pin_state = 0x5;
    
    Serial.println("set LED ON");
    //////////////////////////////
    // Packet 1 - LED
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
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('4');

    // The value (0x4 for off, 0x5 for on) 
    sum += sendByte(pin_state);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
         
  } else{
    pin_state = 0x4;
    
    Serial.println("set LED OFF");
    //////////////////////////////
    // Packet 1 - LED
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
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('4');

    // The value (0x4 for off, 0x5 for on) 
    sum += sendByte(pin_state);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
  }   
}

void toggleRemotePinMove(int value, byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){

  if(value){
    
    Serial.println("set MOVE sensor ON");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('1');

    // The value (0x03 for digital input, 0x0 for disable) 
    sum += sendByte(0x03);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the sensor settle down 
    delay(1000); 
   
    ///////////////////////////////
    //Packet 3 - IC
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('I');
    sum += sendByte('C');
    
    // The value (0x0F mask for D3, D2, D1, D0) 
    sum += sendByte(0x0F);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
    
  }
  else{
    
    Serial.println("set MOVE sensor OFF");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('1');

    // The value (0x03 for digital input, 0x0 for disable) 
    sum += sendByte(0x0);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
  }  
}


void toggleRemotePinGas(int value, byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){
  
  if(value){
    
    Serial.println("set GAS sensor ON");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('0');

    // The value (0x02 for analog, 0x0 for disable) 
    sum += sendByte(0x02);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the sensor settle down 
    delay(10);
    
    ///////////////////////////////
    //Packet 3 - IR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('I');
    sum += sendByte('R');
    
    // The value (0xEA60 for 60s, 0x7530 for 30s, 0x2710 for 10s) 
    sum += sendByte(0x2710);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
  }
  else{
    
    Serial.println("set GAS sensor OFF");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('0');

    // The value (0x02 analog, 0x0 for disable) 
    sum += sendByte(0x0);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
    
    ///////////////////////////////
    //Packet 3 - IR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('I');
    sum += sendByte('R');
    
    // The value (0xEA60 for 60s, 0x7530 for 30s, 0x2710 for 10s) 
    sum += sendByte(0x0);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
  }  
}


void toggleRemotePinLight(int value, byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){
  
  if(value){
    
    Serial.println("set LIGHT sensor ON");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('2');

    // The value (0x02 for analog, 0x0 for disable) 
    sum += sendByte(0x02);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the sensor settle down 
    delay(10);
    
    ///////////////////////////////
    //Packet 3 - IR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('I');
    sum += sendByte('R');
    
    // The value (0xEA60 for 60s, 0x7530 for 30s, 0x2710 for 10s) 
    sum += sendByte(0x2710);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
  }
  else{
    
    Serial.println("set LIGHT sensor OFF");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('2');

    // The value (0x03 for digital input, 0x0 for disable) 
    sum += sendByte(0x0);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
    
    ///////////////////////////////
    //Packet 3 - IR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('I');
    sum += sendByte('R');
    
    // The value (0xEA60 for 60s, 0x7530 for 30s, 0x2710 for 10s) 
    sum += sendByte(0x0);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);
  }  
}

void toggleRemotePinTemp(int value, byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){
  
  if(value){
  
    Serial.println("set TEMPERATURE sensor ON");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('3');

    // The value (0x02 for analog, 0x0 for disable) 
    sum += sendByte(0x02);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the sensor settle down 
    delay(10);
  }
  else{
  
    Serial.println("set TEMPERATURE sensor OFF");
    //////////////////////////////////   
    // Packet 2 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);
  
    // Send Remote AT options
    sum += sendByte(remoteATOptionApplyChanges); 

    // The text of the AT command 
    sum += sendByte('D');
    sum += sendByte('3');

    // The value (0x03 for digital input, 0x0 for disable) 
    sum += sendByte(0x0);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);  
  } 
}

void queriedSample(byte sensorAddress0, byte sensorAddress1, 
                     byte sensorAddress2, byte sensorAddress3){
               
    Serial.println("TEMPERATURE sensor DATA");
    //////////////////////////////////   
    // Packet 1 - SENSOR
    sendByte(frameStartByte);  
    sendByte(0x0);
    sendByte(0x10);
    
    long sum = 0; 

    sum += sendByte(frameTypeRemoteAT);
    sum += sendByte(0x0); 
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

    sum += sendByte(0xFF);
    sum += sendByte(0xFE);

    //Remote Command Options
    sum += sendByte(0x0);
    
    // The value (0x4953 for IS command) 
    sum += sendByte(0x4953);
  
    // Send the checksum
    sendByte( 0xFF - ( sum & 0xFF));

    // Pause to let the microcontroller settle down if needed
    delay(10);  
}  

void xbeePacket(void){
  
  //Xbee communication
  if(Serial1.available() > 21){
    //Start byte
    if(Serial1.read() == 0x7E){
      for(int i=0; i<16; i++){ //Reads until first byte of digital channel mask
        byte discardByte = Serial1.read();
      }
      
      //////////////////////////////////
      //SENSOR DATA
      
      //Digital sample data - Move sensor
      if(Serial1.read() == 0x12){ //Second byte of digital channel mask
        byte discardByte = Serial1.read();
        byte discardByte = Serial1.read();
        if(Serial1.read() == 0x12){ //Reads second byte of digital sample data
          //Alarm => 20s
          setAlarm();
          //TODO: button option to turn OFF the alarm  
        }
      } 
      /*
      if(Serial1.read() == 0x01 ){ //Analog channel mask - Gas sensor
        int analogMSB = Serial1.read();
        int analogLSB = Serial1.read();
        int analogGasValue = analogLSB + (analogMSB * 256);
        
        if(analogGasValue > ){ 
          //Gas leakage - Alarm!
          setAlarm();
        }        
        //TODO: different alarms for different values of gas -> what type of gas it is. 
      }  
      else if(Serial1.read() == 0x04){ //Light sensor
        int analogMSB = Serial1.read();
        int analogLSB = Serial1.read();
        int analogLightValue = analogLSB + (analogMSB * 256);
        
        if(analogLightValue > 0 && analogLightValue <= 350 ){
          Serial.println("Too dark!");
          digitalWrite(LED,HIGH);
        }
        if(analogLightValue > 350  && analogLightValue <= 750){
          Serial.println("Perfect for romance..");
          digitalWrite(LED,LOW);
        }  
        if(analogLightValue > 750 && analogLightValue <= 1023){
          Serial.println("No need for more light.");
          digitalWrite(LED,HIGH);
        }  
        
        //TODO: use the information from the light sensor 
        //OPTION 1: turn ON/OFF the lights       
      }
      else if(Serial1.read() == 0x08){ //Temp sensor
        int analogMSB = Serial1.read();
        int analogLSB = Serial1.read();
        int analogLTempValue = analogLSB + (analogMSB * 256);
        
        float celcius = (analogTempValue/1023)*1230;
        chatServer.println("setTempValue:"+String(celcius));
      }  
      */
    }    
  } 
} 

void readSerial(void){
  
  //Xbee communication
  if(Serial1.available()){
    for(int i=0; i<22; i++){
      Serial.print(Serial1.read(),HEX);
      Serial.print(",");
    }
    Serial.println();    
  }   
}

void setBuzzer(int buzzerState){
  
  if(buzzerState){
    //turn ON sensor: 1 beep
    analogWrite(BUZZER,50);
    delay(500);
    analogWrite(BUZZER,0);
  }
  else{
    //turn OFF sensor: 2 beeps
    analogWrite(BUZZER,50);
    delay(250);
    analogWrite(BUZZER,0);
    delay(250);
    analogWrite(BUZZER,50);
    delay(250);
    analogWrite(BUZZER,0);
  }  
}

void setAlarm(void){
  
  float sinVal;
  int toneVal;
  
  for(int j=0; j<50; j++){
    for(int i=0; i<180; i++){
      //Convert degrees to radians
      sinVal = (sin(i*(3.1412/180))); 
    
      //Generate frequency
      toneVal = 2000+(int(sinVal*1000));
      tone(BUZZER,toneVal);
      delay(2);
    }  
  }
  noTone(BUZZER);
}  

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

/**************************************************************************/
/*!
    @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);
    
    Serial.print(F("SSID Name    : ")); Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

