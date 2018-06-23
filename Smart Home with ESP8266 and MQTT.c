#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
// Wifi for connectiom
const char* ssid = "LANIUS489";
const char* password = "sebatdulu1";
// Server IP
const char* mqtt_server = "192.168.100.58";
// MQTT Port
const int mqttPort = 1883;
// MQTT user and Password ( This from NodeRed js)
const char* mqttUser = "smart home";
const char* mqttPassword = "ardi7777";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int tombol1 = D7;
int tombol2 = D8;
int led1 = D1, led2 = D2;

void setup_wifi() {
  
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  String t=topic;
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(t == "led1"){
    //Serial.println("LEDD");
      if ((char)payload[0] == '1') {
        digitalWrite(led1, HIGH);
        }else{
          digitalWrite(led1, LOW);
          }
    }else if(t=="led2"){
      if ((char)payload[0] == '1') {
        digitalWrite(led2, HIGH);
        }else{
          digitalWrite(led2, LOW);
          }
      }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
      client.subscribe("led1");
      client.subscribe("led2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(tombol1,INPUT);
  pinMode(tombol2,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //Serial.println("Ready,,,");
}
int buttonState1=0, buttonState2=0;
bool f1=false,f2=true,f3=false,f4=true;
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  buttonState1 = digitalRead(tombol1);
  if ((buttonState1 == HIGH) && (f1==false)) {
    client.publish("tombol1", "1");
    Serial.println("tombol 1 : 1");
    f1=true;
    f2=false;
    delay(300);
  } else if((buttonState1 == LOW) && (f2==false)) {
    //client.publish("tombol1", "0");
    Serial.println("tombol 1 : 0");
    f2=true;
    f1=false;
    delay(50);
  }

  buttonState2 = digitalRead(tombol2);
  if ((buttonState2 == HIGH) && (f3==false)) {
    client.publish("tombol2", "1");
    Serial.println("tombol 2 : 1");
    f3=true;
    f4=false;
    delay(300);
  } else if((buttonState2 == LOW) && (f4==false)) {
    //client.publish("tombol2", "0");
    Serial.println("tombol 2 : 0");
    f3=false;
    f4=true;
    delay(50);
  }
}