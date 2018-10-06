#include <Servo.h>
#include <ESP8266WiFi.h>

Servo servo;

const char* ssid = "SSID";
const char* password = "PASS";

int value = 0;
String address;

WiFiServer server(80);
int SERVOPIN = 2;

void setup() {

  servo.attach(SERVOPIN);
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }
  
  Serial.println();
  Serial.println("Wifi Connected!");

  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  address = WiFi.localIP().toString();
  
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(address);
  Serial.println();
  
  servo.write(value);
}

void loop() {
    // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()) {
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  if (request.indexOf("/ON") != -1)  {
    
    value = 180;
    servo.write(value);
    
  } else if (request.indexOf("/OFF") != -1)  {
    
    value = 0;
    servo.write(value);
    
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println(""); //  do not forget this one

  int statusInteger = value == 0 ? 0 : 1;  
  client.println(statusInteger);
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
