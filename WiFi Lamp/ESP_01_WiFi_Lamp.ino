#include <ESP8266WiFi.h>    
//#include <WiFiClient.h>              
//const char* ssid = "Ria's Router";                
//const char* password = "Ria@PloversNest";             
// const char* ssid = "ASUS";                 
// const char* password = "wilson@1965"; 
const char* ssid = "WifiLamp";                 
 
WiFiServer server(80);                     // Create an instance of the server and specify the port to listen on as an argument
unsigned initial_Time = 0;
unsigned long recheck_Connect = 100000;

IPAddress local_IP(192,168,4,21);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup() 
{
  Serial.begin(9600);
  Serial.print("Access Point");
 // set buad rate 
  delay(10);
  pinMode(0, OUTPUT);                           // set GPIO 0 as OUTPUT
  pinMode(2, OUTPUT);                           // set GPIO 2 as OUTPUT
  //pinMode(1,INPUT);
  digitalWrite(0, 0);                           // initially set GPIO 0 as Low
  digitalWrite(2, 0);                           // initially set GPIO 2 as Low
  //digitalWrite(1, 0);
  Serial.println();
  Serial.println();
  
  //Serial.print("Connecting to ");               // Connect to WiFi network
  
  /////////////////////////////////////////////////////////////////////////////////////

  WiFi.mode(WIFI_AP);           //Only Access point
  //Serial.println("Setting soft-AP configuration ... ");
  WiFi.softAPConfig(local_IP, gateway, subnet);////////////;
  WiFi.softAP(ssid);
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  //Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");
  ////////////////////////////////////////////////////////////////////////////////////
  Serial.println(ssid);
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  // while (WiFi.status() != WL_CONNECTED) 
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  server.begin();                              // Start the server
  Serial.println("Server started");
  Serial.println(WiFi.localIP());              // Print the IP address
  // WiFi.setAutoReconnect(true);
  // WiFi.persistent(true);
}

void loop() 
{ 
  // unsigned long current_Time = millis();
  //Serial.printf("Access Point");
  WiFiClient client = server.available();       // Check if a client has connected
  // if((WiFi.status() != WL_CONNECTED) && (current_Time - initial_Time >= recheck_Connect))
  // {
  //   WiFi.disconnect();
  //   WiFi.reconnect();
  //   initial_Time = current_Time;
  // }
  if (!client) 
  {
    return;                                     // Wait until the client sends some data
  }
  Serial.println("new client");
  while(!client.available())
  {
    Serial.println("-");
    delay(1);
  }
  String req = client.readStringUntil('\r');   // Read the first line of the request
  Serial.println(req);
  client.flush();
  int val1;
  int val2;
  if (req.indexOf("/gpio1/0") != -1)           // Match the request
    val1 = 1;
  else if (req.indexOf("/gpio1/1") != -1)
    val1 = 0;
  //else if (req.indexOf("/gpio2/0") != -1)
  //  val2 = 1;
  //else if (req.indexOf("/gpio2/1") != -1)
  //  val2 = 0;  
  else 
  {
    //Serial.println("invalid request");
    //val1=1;
    //client.flush();
    //client.stop();
    return;
  }
  digitalWrite(2, val1);               // Set GPIO 2 according to the request
  digitalWrite(0, val2);               // Set GPIO 0 according to the request
  client.flush();
  
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";        // Prepare the response
  s += "<body><h1>Client has connected</h1>\r\nLamp is ";
  s += (val1)?"on!":"off!";
  s += "</html>\n";
  client.print(s);                           // Send the response to the client
  delay(1);
  Serial.println("Client disconnected");      // The client will actually be disconnected and when the function returns and 'client' object is detroyed
}
