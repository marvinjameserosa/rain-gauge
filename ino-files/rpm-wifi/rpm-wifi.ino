#include <WiFi.h>

#define ENC_COUNT_REV 2400
#define ENC_IN 26

volatile long encoder = 0;

int interval = 1000;

long previousMillis = 0;
long currentMillis = 0;

int motorPwm = 0;

// WiFi Variables
const char* ssid = "####"; // WiFi SSID        
const char* password = "####"; // WiFi Password
const char* host = "192.168.X.X";  // Host IP Adress     
const uint16_t port = ####; // Port Number

WiFiClient client;

void setup() {
  Serial.begin(9600); 

  // Setup Encoder
  pinMode(ENC_IN, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);

  previousMillis = millis();

  // Setup WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Attempt to connect to the server
  connectToServer();
}

void loop() {
  if (!client.connected()) {
    Serial.println("Disconnected from server. Attempting to reconnect...");
    connectToServer();
  }

  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    // Calculate RPM
    float rpm = (float)(encoder * 60 / ENC_COUNT_REV);
    String rpmStr = String(rpm) + " rpm";

    Serial.println(rpmString);
    client.print(rpmString);
    
    encoder = 0;
  }
}

void updateEncoder() {
  encoder++;
}

void connectToServer() {
  while (!client.connect(host, port)) {
    Serial.println("Connection to host failed. Retrying...");
    delay(1500);
  }
  Serial.println("Connected to server.");
}
