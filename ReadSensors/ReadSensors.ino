#define LED 13              // LED pin on Arduino Uno
const int n_sensors = 4;
int    sensorPins[n_sensors] = {A0, A1, A2, A3};
double sensorVals[n_sensors] = {0, 0, 0, 0};
int threshold = 15; // Sensors must cross this level to register a hit (user-defined)

void setup() {
  pinMode(LED, OUTPUT);  
  Serial.begin(9600);
}

void loop() {
  // Check for sensor hits (threshold crossings)
  for (int i = 0; i < n_sensors; i++){                
    // Read from sensor and (maybe) trigger a note
    sensorVals[i] = analogRead(sensorPins[i]);     
    if (sensorVals[i] > threshold) {            
      digitalWrite(LED, HIGH);
      delay(200);
    }    
  } // End looping through checking each sensor  
} // End main loop()