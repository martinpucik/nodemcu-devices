#include <Servo.h>
#include <ESP8266WiFi.h>

Servo servo;

const char* ssid = "SSID";
const char* password = "PASS";



String address;

const int offValue = 0;
const int onValue = 180;
int value = onValue;

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
  Serial.println(WiFi.macAddress());

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

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println(""); //  do not forget this one
  
  if (request.indexOf("/STATE") != -1) {

     /// Idle position for blinds
     client.println("2");
     
  } else if (request.indexOf("/POSITION") != -1) {
    
    /// Current poisition
    int statusInteger = value == offValue ? 0 : 100;  
    client.println(statusInteger);
    
  } else {

    /// Set servo to passed position
    /// http://192.168.0.115/1 || http://192.168.0.115/100

    String noMethodString = request.substring(5);
    int endIndex = noMethodString.indexOf(" ");
    if (endIndex != -1) {
      String valueString = noMethodString.substring(0, endIndex);
      double requestedValue = (double(valueString.toInt()) / 100);
      int newServoValue = int(requestedValue * onValue);

      value = newServoValue;
      servo.write(value);
    }
    
    client.println();
  }
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
