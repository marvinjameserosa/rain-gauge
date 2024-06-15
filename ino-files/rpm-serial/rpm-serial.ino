#define ENC_COUNT_REV 2400
#define ENC_IN 26

volatile long encoder = 0;

int interval = 1000;

long previousMillis = 0;
long currentMillis = 0;

int motorPwm = 0;

void setup() {
  Serial.begin(9600); 

  pinMode(ENC_IN, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);

  previousMillis = millis();
}

void loop() {
 
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    float rpm = (float)(encoder * 60 / ENC_COUNT_REV);
    String rpmStr = String(rpm) + " rpm";

    Serial.print("rpm:");
    Serial.println(rpmStr);

    float flowrate = rpm * 2;
    String flowrateStr = String(flowrate) + " m3/s";
    Serial.print("flowrate:");
    Serial.println(flowrateStr);

    if (rpm == 0.00){
      String intensity = "intensity: No Rain";
      Serial.println(intensity);
    }else if (rpm < 50){
      String intensity = "intensity: Light Rain";
      Serial.println(intensity);
    }else if (rpm < 100){
      String intensity = "intensity: Heavy Rain";
      Serial.println(intensity);
    }

    encoder = 0;
  }
}

void updateEncoder() {
  encoder++;
}


