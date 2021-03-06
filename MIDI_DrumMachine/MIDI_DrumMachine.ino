#define LED 10              // LED pin on Arduino Uno
// Sensors
const int n_sensors = 4;
int    sensorPins[n_sensors] = {A0, A1, A2, A3};
double sensorVals[n_sensors] = {0, 0, 0, 0};
bool   sensorIsReady[n_sensors] = {false, false, false, false};

// Button
int butPin = 4;
int sensor_to_change = 0;
bool butVal = false;
int pitchCounter = 0;

// Potentiometer (selects sounds)
int potPin = A4;
double potVal = 0.0;

// MIDI
bool noteIsOn[n_sensors] = {false, false, false, false};
int  pitches[n_sensors] = {36, 37, 38, 39};
// int  pitches[n_sensors] = {36, 43, 38, 39};
int  noteVel = 0;
int  newPitch = 0;
double t_last_note[n_sensors];

// Timing stuff
int t_between_notes = 100; // Min. time until next trigger from sensor (ms)
int note_duration = 500;  // Time until note is cut off (ms)

int    threshold = 15; // Sensors must cross this level to be registered as a note
int count = 0;
int debug1 = 5;
int debug2 = 6;
bool selectPitches = true;


// ----------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);  
  pinMode(butPin, INPUT);
  Serial.begin(31250);
  // Serial.begin(9600);
}

// ----------------------------------------------------------------
void loop() {
  if (count == 0 && digitalRead(debug1) == HIGH && digitalRead(debug2) == HIGH){
    selectPitches = false;
    count = 1;
  }
  if (digitalRead(butPin) == HIGH && selectPitches == true){
    while(digitalRead(butPin)==HIGH){
      delay(1); // Wait for button release
    }
    digitalWrite(LED, HIGH);
    pitchCounter = pitchCounter + 1;
    if (pitchCounter > (n_sensors-1) ){
      pitchCounter = 0;
    }
  }
  else {
    digitalWrite(LED, LOW);
  }

  if (selectPitches == false){
    pitchCounter = 4;
  }

  // Read value from pot, determine pitch value for the 1st sensor
  potVal = analogRead(potPin);  
  newPitch = floor((potVal/1023)*17+36);  

  // Check for sensor hits (threshold crossing)
  // Also, turn any notes off if it's time to do so
  for (int i = 0; i < n_sensors; i++){                
    // Read from sensor and (maybe) trigger a note
    sensorVals[i] = analogRead(sensorPins[i]);     
    if (sensorVals[i] > threshold && sensorIsReady[i] == true) {            
      if (noteIsOn[i] == true){
        sendNoteOff(pitches[i]);
      }
      pitches[pitchCounter] = newPitch;
      noteVel = convertToVel(sensorVals[i], threshold);      
      // noteVel = 127;
      sendNoteOn(pitches[i], noteVel); // Turn the MIDI note on
      t_last_note[i] = millis();
      noteIsOn[i] = true;
      sensorIsReady[i] = false;
    }    

    // Check if the current sensor is ready to be reactivated    
    if (millis() - t_last_note[i] > t_between_notes){
      sensorIsReady[i] = true;      
      
      // Turn off previous note if it's rang out long enough
      if (noteIsOn[i] == true && (millis() - t_last_note[i] > note_duration)){
        sendNoteOff(pitches[i]); // Turn off note
        noteIsOn[i] = false;
      }      
    }
  } // End looping through checking each sensor  
  
  delay(1);
} // End main loop()

// -------------------------------------------------------------------
// Define the internal functions for MIDI communication
void sendNoteOn(int note, int velocity){  
  int command  = 144; //144 = 10010000 in binary, note on command    
  Serial.write(command); // Send note on command
  Serial.write(note);
  Serial.write(velocity); // Velocity value of 0 (turn the note off)  
}

void sendNoteOff(int note){  
  int command = 128; // Note off, 128 = 10000000 in binary, note off command  
  Serial.write(command); // Send note on command
  Serial.write(note);
  Serial.write(0); // Velocity value of 0 (turn the note off)  
}

int convertToVel(double raw, int threshold){
  double outputVel;
  double min = 60;
  double max = 127;  
  // outputVel = floor(raw/8.0+10);
  outputVel = floor(max*(raw - threshold)/(max-min));
  if (outputVel >= 115){
    outputVel = max;
  }

  return outputVel;
}

void flashLED(){
  for (int n=0;n<10;n++){
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
  }
  delay(500);
}


























