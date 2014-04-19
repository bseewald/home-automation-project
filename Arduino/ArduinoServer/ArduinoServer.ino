/*
  Modified bidirectional TCP Server (by Laurid Meyer) and WiFi Web Server (by dlf and Tom Igoe)
  
  created 17.04.2014
  by Bruna Seewald
  
 */
 
 #include<SPI.h>
 #include<WiFi.h>
 #include<XBee.h>
 
// Enter a MAC address, IP address and Portnumber for your Server below.
// The IP address will be dependent on your local network:
IPAddress serverIP(192,168,0,214); 
int serverPort=8888;

char ssid[] = "fsilva";      // your network SSID (name) 
char pass[] = "bru2465betinharml";   // your network password

int status = WL_IDLE_STATUS;

// Initialize the WiFi server library
// with the IP address and port you entered
WiFiServer server(serverPort);

void setup() {
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  }
  
  //Static IP Address
  WiFi.config(serverIP);
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus(); 
  
  //TODO: pins
}  

void loop()
{
  // listen for incoming clients
  WiFiClient client = server.available();
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
            //set the servo to the recieved value
            //myservo.write(convertToInt(value));
          }
          
          //TODO: comandos para todos os botoes
          
          
          
          
          //reset the commandline String
          commandStr="";
        } 
      }
    }
    // give the client time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
  

