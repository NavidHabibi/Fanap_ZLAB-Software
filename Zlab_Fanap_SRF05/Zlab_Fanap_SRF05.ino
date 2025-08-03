void setup() {
  // put your setup code here, to run once:

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
  // put your main code here, to run repeatedly:

}
