#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Hariharan";
const char* password = "ha22232750";
const char* mqtt_server = "broker.hivemq.com";
const char* TOPIC_NAME = "/techo/smartoffice/smartlock";
const char* AUTHENTICATE_SUCCESS = "{\"Authenticate\":\"True\"}";

char msg[50];

WiFiClient espClient;
PubSubClient client(espClient);

int LOCKPIN = 13;    //Testing with LED instead of solenoid

void setup() {
  // put your setup code here, to run once:
  pinMode(LOCKPIN, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  
  setup_wifi();
  Serial.begin(115200);
 // setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  digitalWrite(LOCKPIN, HIGH);  
  // put your main code here, to run repeatedly:
 // Serial.println("In Loop");
  //digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(10000);              // wait for a second
  //digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  //delay(10000);    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //snprintf (msg, 75, "hello world #%ld", 1);
 // client.publish(TOPIC_NAME, msg);
}

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}


void reconnect() {
  // Loop until we're reconnected
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //if (client.connect(clientId,userName,passWord)) //put your clientId/userName/pass
    if (client.connect("Techolockclient")) {
      Serial.println("connected");
     
      client.subscribe(TOPIC_NAME);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
//  String messageReceived = bytestostring(payload);
  //char messageReceived[50] = String(payload);
  String messageReceived = "";
  for (int i = 0; i < length; i++) {
 //   Serial.print((char)payload[i]);
  //  messageReceived=messageReceived+(char)payload[i];
 //   messageReceived[i]=(char)payload[i];
    messageReceived.concat((char)payload[i]);
  }
 // char * messageReceived=payload;
  
  //byte byteArray[50];
  //strcpy((char *)byteArray,payload);
 

   Serial.println(messageReceived);
  if(messageReceived ==AUTHENTICATE_SUCCESS){
    Serial.println("TURNING THE LED ON");
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(LOCKPIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    delay(30000);
    Serial.println("TURNING THE LED OFF");
    //digitalWrite(BUILTIN_LED, HIGH);  
    digitalWrite(LOCKPIN, HIGH);  
    
  }
  // Switch on the LED if an 1 was received as first character
  
  /*if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }*/

}


