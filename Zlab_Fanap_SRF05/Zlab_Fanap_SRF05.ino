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

float convertToDistance(long duration, String unitStr) {
  unitStr.toUpperCase();
  if (unitStr == "CM") return duration / 58.0;
  if (unitStr == "MM") return (duration / 58.0) * 10.0;
  if (unitStr == "INCH") return duration / 148.0;
  return -1.0;
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

float measureFiltered(unsigned long duration_ms, String unit = "CM") {
  unsigned long startTime = millis();
  float readings[50];
  int count = 0;
  while (millis() - startTime < duration_ms && count < 50) {
    float dist = measureOnce(unit);
    if (dist >= 0) readings[count++] = dist;
    delay(10);
  }
  if (count == 0) return -1.0;
  if (count < 5) {
    float sum = 0;
    for (int i = 0; i < count; i++) sum += readings[i];
    return sum / count;
  }
  float med = median(readings, count);
  float maxV = readings[0], minV = readings[0];
  for (int i = 1; i < count; i++) {
    if (readings[i] > maxV) maxV = readings[i];
    if (readings[i] < minV) minV = readings[i];
  }
  float weightedSum = 0, totalWeight = 0;
  for (int i = 0; i < count; i++) {
    if (readings[i] == maxV || readings[i] == minV) continue;
    float diff = abs(readings[i] - med);
    float weight = 1.0 / (1.0 + diff);
    weightedSum += readings[i] * weight;
    totalWeight += weight;
  }
  if (totalWeight == 0) return med;
  return weightedSum / totalWeight;
}


float median(float arr[], int size) {
  float temp[size];
  memcpy(temp, arr, sizeof(float) * size);
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (temp[i] > temp[j]) {
        float t = temp[i];
        temp[i] = temp[j];
        temp[j] = t;
      }
    }
  }
  if (size % 2 == 0) return (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
  return temp[size / 2];
}

bool isObjectAtDistance(float threshold, String unit = "CM") {
  unsigned long startTime = millis();
  const unsigned long timeout = 5000;
  while (millis() - startTime < timeout) {
    float distance = measureFiltered(200, unit);
    if (distance >= 0 && distance <= threshold) return true;
    delay(100);
  }
  return false;
}

void loop() {
  checkSerialForUnit();

  Serial.print("Distance: ");
  float dist = measureFiltered(200, defaultUnit);
  Serial.print(dist, 1);
  Serial.print(" ");
  Serial.println(defaultUnit);

    if (isObjectAtDistance(100, defaultUnit)) {
    Serial.println("Object Detected!");
  } else {
    Serial.println("No Object.");
  }

  delay(2000);

}
