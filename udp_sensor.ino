
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load the Udp Library
#include <SPI.h> //Load SPI Library
#include <HX711.h> // import the Pressure/Temperature sensor library
HX711 mySensor; //Create a sensor object
 
float tempC; //Declare variable for Temp in C
float tempF; //Declare variable for Temp in F
float Pressure; //Declare a variable for Pressure

byte mac[] ={ 0xA8, 0x61, 0x0A, 0xAE, 0x84, 0x16}; //Assign mac address
//IPAddress ip(10, 91, 120, 61); //Assign the IP Adress
unsigned int localPort = 1900; // Assign a port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //dimensian a char array to hold our data packet
String datReq; //String for our data
int packetSize; //Size of the packet
EthernetUDP Udp; // Create a UDP Object
 
void setup() {
  //DHCP setup
   // start serial port:
  Serial.begin(9600);
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }

    if (Ethernet.linkStatus() == LinkOFF) 
    {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
//    Ethernet.begin(mac, ip); //Inialize the Ethernet WITHOUT DHCP (NOT PREFFERED)
    Serial.print("My IP address: "); 
    Serial.println(Ethernet.localIP());
  } 
  else
  {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }


  Udp.begin(localPort); //Initialize Udp
  delay(1500); //delay
  mySensor.begin(A1, A0); //initialize pressure-temp sensor
 
}
 
void loop()
{ //BEGIN VOID LOOP
  
  packetSize =Udp.parsePacket(); //Reads the packet size
  
  if(packetSize>0) { //if packetSize is >0, that means someone has sent a request
    
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Read the data request
    String datReq(packetBuffer); //Convert char array packetBuffer into a string called datReq
    
    if (datReq =="Temperature"){ //Do the following if Temperature is requested
    
      tempC = mySensor.read(); //Read the temperature
      tempF = tempC*1.8 + 32; //Convert temp to F
      
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //Initialize packet send
      Udp.print(tempF); //Send the temperature data
      Udp.endPacket(); //End the packet
      
    }
    /*
    if (datReq== "Pressure") { //Do the following if Pressure is requested
    
      Pressure=mySensor.readPressure(); //read the pressure
      
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //Initialize packet send
        Udp.print(Pressure); //Send the Pressure data
        Udp.endPacket(); //End the packet
        
    }*/
      
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE); //clear out the packetBuffer array
  //tempC = mySensor.read();
  //Serial.println(tempC);
  
}//end void LOOP
