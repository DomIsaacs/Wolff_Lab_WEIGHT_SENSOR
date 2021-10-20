
#include <Ethernet.h> //Load Ethernet Library
#include <EthernetUdp.h> //Load the Udp Library
#include <SPI.h> //Load SPI Library
#include <HX711.h> // import the Pressure/Temperature sensor library

HX711 weight_sensor; //Create weight sensor object


float weight; //Declare variable for weight
float tempC; //Declare variable for temperature sensor

const int CLK_pin = A0;
const int DAT_pin = A1;


byte mac[] ={0xA8, 0x61, 0x0A, 0xAE, 0x84, 0x16}; //Assign mac address, a specific code the network knows this machine as. Can actually be random, so long as its unique
//IPAddress ip(x, x, x, x); //Assign the IP Adress only use if you need to hard cose the ip address in
unsigned int localPort = 1900; // Assign a port to talk over (either this or port 5000)
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //dimmension a char array to hold our data packet
String datReq; //String for our data
int packetSize; //Size of the packet
EthernetUDP Udp; // Create a UDP Object
 
void setup()
{
 
  //DHCP setup (ask the network for an ip address, and also a bunch of discretionary shit)--------------------------------------------------------------------
   // start serial port:
  Serial.begin(115200);
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
//    Ethernet.begin(mac, ip); //Inialize the Ethernet WITHOUT DHCP (NOT PREFFERED, in fact if you cant get this auto assigned it may screw you over..)
    Serial.print("My IP address: "); 
    Serial.println(Ethernet.localIP());
  } 
  else
  {
    Serial.print("  DHCP assigned IP "); // yay, it was assigned a router to talk to on the network
    Serial.println(Ethernet.localIP()); // local ip address our device was given. 
  }
 
  //Initialize Udp------------------------------------------------------------------------------------------------------------------------------------------
  Udp.begin(localPort); 
  delay(1500); //delay
  
 
 // Init weight sensor---------------------------------------------------------------------------

  weight_sensor.begin(DAT_pin,CLK_pin);
  weight_sensor.set_scale(2020.f);    // this value is obtained by calibrating the scale with known weights; see the README for details
  weight_sensor.tare();               // reset the scale to 0

}// END void SETUP


void loop()
{ 
  
  packetSize =Udp.parsePacket(); //Reads the packet size
  if(packetSize>0) { //if packetSize is >0, that means someone has sent a request
    
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Read the data request
    String datReq(packetBuffer); //Convert char array packetBuffer into a string called datReq
    
    if (datReq =="req"){ //Do the following if weight is requested
    
      weight = weight_sensor.get_units(); //Read the digital number reported by the sensor and divide by scale factor. can average readings over CLK cycles by specifying get_units(n)
      weight = weight/1.5; // linear equation to retrieve real weight (empirically determined with a weight set, unique for each strain gauge)
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //Initialize packet send
      Udp.print(weight); //Send the weight data
      Udp.endPacket(); //End the packet
      
    }

    /*
    if (datReq== "Temperature") { //Do the following if Pressure is requested
    
       tempC=temp_sensor.read(); //read the pressure
      
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //Initialize packet send
        Udp.print(Pressure); //Send the Pressure data
        Udp.endPacket(); //End the packet
        
    }*/
      
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE); //clear out the packetBuffer array
  
}//end void LOOP
