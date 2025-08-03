

String defaultUnit = "CM";

void setup() {

  Serial.begin(115200);
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
