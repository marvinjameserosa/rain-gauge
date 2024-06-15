#define SIM800L_RX 16
#define SIM800L_TX 17

HardwareSerial mySerial(2); 

String response; 

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, SIM800L_RX, SIM800L_TX);
  
  Serial.println("Initializing..."); 
  delay(1000);
  
  if (!sendCommand("ATE0")) return; // Turn off echo
  if (!sendCommand("AT")) return; // Test AT command
  if (!sendCommand("AT+CPIN?")) return; // Check SIM card status
  
  if (!sendCommand("AT+CFUN?")) return; // Check functionality status
  if (!sendCommand("AT+CMGF=1")) return; // Configuring TEXT mode
  if (!sendCommand("AT+CSQ")) return; // Check signal quality
  
  // Register with the network
  if (!sendCommand("AT+CREG=1")) return;

  // Check network registration status again
  if (!sendCommand("AT+CREG?")) return;

  Serial.println("SIM card registered successfully!");
  
  // Send SMS if all checks are passed
  const char* phoneNumber = "+###########"; // Replace with your phone number
  const char* message = "Hello it's me"; // Replace with your message
  if (sendSMS(phoneNumber, message)) {
    Serial.println("SMS sent successfully!");
  } else {
    Serial.println("Failed to send SMS.");
  }
}

void loop() {
  
}

bool sendCommand(const char* cmd) {
  Serial.print("Sending command: ");
  Serial.println(cmd);
  mySerial.println(cmd);
  delay(2000); // Adjust delay as needed
  return readSerialResponse();
}

bool sendSMS(const char* phoneNumber, const char* message) {
  Serial.print("Sending SMS to: ");
  Serial.println(phoneNumber);
  
  // Send SMS command
  mySerial.print("AT+CMGS=\"");
  mySerial.print(phoneNumber);
  mySerial.println("\"");
  
  // Wait for '>' prompt from SIM800L
  if (!waitForPrompt('>')) {
    Serial.println("Failed to get '>' prompt.");
    return false;
  }
  
  // Send SMS content
  mySerial.print(message);
  
  // Send Ctrl+Z to indicate end of message
  mySerial.write(26);
  
  // Wait for response from SIM800L
  if (!waitForResponse(10000)) { // Timeout in 10 seconds
    Serial.println("Timeout occurred while waiting for SMS response.");
    return false;
  }
  
  // Check response for success or error
  if (response.endsWith("OK\r\n")) {
    Serial.println("SMS sent successfully!");
    return true;
  } else if (response.endsWith("ERROR\r\n")) {
    Serial.println("Failed to send SMS. Error response received.");
    return false;
  } else {
    Serial.println("Unexpected response after sending SMS:");
    Serial.println(response);
    return false;
  }
}

bool waitForPrompt(char promptChar) {
  unsigned long timeout = millis() + 2000; // Timeout in 2 seconds
  while (millis() < timeout) {
    if (mySerial.available()) {
      char c = mySerial.read();
      if (c == promptChar) {
        return true;
      }
    }
  }
  return false;
}

bool waitForResponse(unsigned long timeoutMillis) {
  unsigned long timeout = millis() + timeoutMillis;
  response = "";
  while (millis() < timeout) {
    if (mySerial.available()) {
      char c = mySerial.read();
      response += c;
      if (response.endsWith("OK\r\n") || response.endsWith("ERROR\r\n")) {
        return true;
      }
    }
  }
  return false;
}

bool readSerialResponse() {
  unsigned long timeout = millis() + 10000; // Timeout in 10 seconds
  response = "";

  while (millis() < timeout) {
    if (mySerial.available()) {
      char c = mySerial.read();
      response += c;
      
      // Check if the response ends with "OK" or "ERROR"
      if (response.endsWith("OK\r\n")) {
        Serial.println("Response: " + response);
        return true; // Return true for success
      } else if (response.endsWith("ERROR\r\n")) {
        Serial.println("Response: " + response);
        return false; // Return false for error
      }
    }
  }

  // Timeout occurred
  Serial.println("Timeout occurred while waiting for response.");
  return false;
}
