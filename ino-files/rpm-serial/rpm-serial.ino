#define ENC_COUNT_REV 2400
#define ENC_IN 26

volatile long encoder = 0;

const int interval = 1000;

long previousMillis = 0;
long currentMillis = 0;

float flowrateSum = 0.0;
int flowrateCount = 0;
float mm = 0.0;

void setup() {
  Serial.begin(9600);

  pinMode(ENC_IN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);

  previousMillis = millis();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float rpm = (float)(encoder * 60) / ENC_COUNT_REV;
    Serial.print("rpm: ");
    Serial.println(rpm, 2);  // Print rpm with 2 decimal places

    float flowrate = rpm * 2;
    Serial.print("flowrate: ");
    Serial.print(flowrate, 2);  // Print flowrate with 2 decimal places
    Serial.println(" m3/s");

    if (rpm == 0.0) {
      Serial.println("intensity: No Rain");
    } else if (rpm < 50) {
      Serial.println("intensity: Light Rain");
    } else if (rpm < 100) {
      Serial.println("intensity: Heavy Rain");
    } else {
      Serial.println("intensity: Very Heavy Rain");
    }

    // Average flowrate computation
    flowrateSum += flowrate;
    flowrateCount++;

    if (flowrateCount == 5) {
      mm = flowrateSum / 5.0;

      flowrateSum = 0.0;
      flowrateCount = 0;

      Serial.print("mm: ");
      Serial.println(mm, 2);  // Print mm with 2 decimal places
    }

    encoder = 0;
  }
}

void updateEncoder() {
  encoder++;
}
