#define LED 13              // LED pin on Arduino Uno
// Sensor details
const int n_sensors = 1;
int    sensorPins[n_sensors] = {A1};
double sensorVals[n_sensors] = {0};
bool   sensorOn[n_sensors] = {false};
int threshold = 200;

// MIDI Details
//int pitches[n_sensors] = {34, 35, 36, 37};
int pitches[n_sensors] = {39};
int vels[n_sensors] = {120};
int note_delay = 10;
bool note_is_on = false;
bool turn_off_notes = false;

// Potentiometer
int potPin = A0;
double potVal = 0.0;

// Buttons in place of sensors
int button1 = 6;
double pitch1 = 36;
bool up1 = 1;
double t1 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(button1, INPUT); 
  Serial.begin(31250);
}

void loop() {
  // Read value from pot, determine pitch value for the 1st sensor
  potVal = analogRead(potPin);  
  pitches[0] = floor((potVal/1023)*17+36);

  // Check for button presses
//  if (digitalRead(button1) == true and up1 == true){
//    sendNoteOn(pitch1,vel);    
//    note_is_on = 1;
//    t1 = millis();
//    up1 = false;
//  }
  if (digitalRead(button2) == true and up2 == true){
    sendNoteOn(pitch2,vel);   
    note_is_on = 1;
    t2 = millis();
    up2 = false;
  }

  // Check for sensor hits (over threshold)
  for (int i = 0; i < n_sensors; i++){
    double temp1 = analogRead(sensorPins[i]); 
    sensorVals[i] = temp1;
    Serial.println(temp1);
    if (sensorVals[i] > threshold and sensorOn[i] == false) {
      vel = floor((potVal/1023)*17+36);
      sendNoteOn(pitches[i],vel);
      note_is_on = 1;
      t1 = millis();
      sensorOn[i] = true;
    }
  }
    
  // Check for button release
//  if (digitalRead(button1) == false){
//    up1 = true;
//  }
  if (digitalRead(button2) == false){
    up2 = true;
  }

  // Check that the sensors are below threshold
  for (int i = 0; i < n_sensors; i++){
    double temp2 = analogRead(sensorPins[i]);
    if (temp2 <= threshold){
      sensorOn[i] = false;
    }
  }
  
  // Turn notes off
  if (note_is_on == true){
    if (millis()-t1 > note_delay or millis()-t2 > note_delay){
      for (int i = 0; i < n_sensors; i++){
        sendNoteOff(pitches[i]);
      }           
      note_is_on = 0;   
    }
  }
  delay(5);
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
