#define LED 13
const int n_sensors = 4;
int    sensorPins[n_sensors] = {A1, A2, A3, A4};
double sensorVals[n_sensors] = {0, 0, 0, 0};
//int pitches[n_sensors] = {3, 3};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Read a value from each sensor
  for (int n = 0; n < n_sensors; n++){
    double temp = analogRead(sensorPins[n]);    
//    Serial.print(n); Serial.print(": ");
    Serial.println(temp);
    delay(5);
  }
    
  // Flash the LED
//  for (int i = 0; i < n_sensors; i++) {
//    int count = 0;
//    while (count < pitches[i]) {
//      digitalWrite(LED, HIGH);
//      delay(200);
//      digitalWrite(LED, LOW);
//      delay(200);
//      count++;
//    }
//    delay(3000);
//  }
}
