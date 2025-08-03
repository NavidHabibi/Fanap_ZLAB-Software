#define TRIGGER_PIN 9
#define ECHO_PIN 8

String defaultUnit = "CM";

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(115200);
}

long measureRaw() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH, 30000);
}

float measureOnce(String unit = "CM") {
  long duration = measureRaw();
  return convertToDistance(duration, unit);
}

void checkSerialForUnit() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toUpperCase();
    if (input == "CM" || input == "MM" || input == "INCH") {
      defaultUnit = input;
      Serial.print("Unit changed to: ");
      Serial.println(defaultUnit);
    } else {
      Serial.println("Invalid unit! Use CM, MM, or INCH.");
    }
  }
}

void loop() {
  checkSerialForUnit();

  Serial.print("Distance: ");
  Serial.print(dist, 1);
  Serial.print(" ");
  Serial.println(defaultUnit);

}
