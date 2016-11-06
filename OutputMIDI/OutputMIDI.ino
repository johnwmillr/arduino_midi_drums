#define LED 13              // LED pin on Arduino Uno
// Sensors
const int n_sensors = 4;
int    sensorPins[n_sensors] = {A0, A1, A2, A3};
double sensorVals[n_sensors] = {0, 0, 0, 0};
bool   sensorIsReady[n_sensors] = {false, false, false, false};

// Potentiometer (selects sounds)
int potPin = A4;
double potVal = 0.0;

// MIDI
bool noteIsOn[n_sensors] = {false, false, false, false};
int  pitches[n_sensors] = {36, 37, 38, 39};
int  noteVel = 0;
int  newPitch = 0;
double t_last_note[n_sensors];

// Timing stuff
int t_between_notes = 100; // Min. time until next trigger from sensor (ms)
int note_duration = 500;   // Time until note is cut off (ms)
int threshold = 25; // Sensors must cross this level to be registered as a note
int count = 0;

// ----------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);  
  Serial.begin(31250);  // MIDI communicates at 31250 baud, serial communication w/ computer is 9600  
}

// ----------------------------------------------------------------
void loop() {
  // Read value from pot, determine pitch value for the 1st sensor
  potVal = analogRead(potPin);  
  newPitch = floor((potVal/1023)*17+36); // Play w/ these values to match the resistance of your pot

  // Check for sensor hits (threshold crossing)
  // Also, turn any notes off if it's time to do so
  for (int i = 0; i < n_sensors; i++){                
    // Read from sensor and (maybe) trigger a note
    sensorVals[i] = analogRead(sensorPins[i]);     
    if (sensorVals[i] > threshold && sensorIsReady[i] == true) {            
      if (noteIsOn[i] == true){
        sendNoteOff(pitches[i]);
      }
      pitches[0] = newPitch;
      noteVel = convertToVel(sensorVals[i], threshold);      

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
  outputVel = floor(raw/8.0+10);
  return outputVel;
}



