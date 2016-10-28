#define LED 13              // LED pin on Arduino Uno
// Sensor details
int n_sensors = 2;
int sensorPin1 = A1;
double sensorVal1 = 0.0;
bool sensor1on = false;
int threshold = 200;

// MIDI Details
//int pitches[n_sensors] = {34, 35, 36, 37};
//int pitches[n_sensors] = {38, 36};
int vel = 120;
int kickVel = 40;
int note_delay = 10;
int noteON  = 144; //144 = 10010000 in binary, note on command
int noteOFF = 128; //128 = 10000000 in binary, note off command
bool note_is_on = 0;
bool turn_off_notes = 0;

// Potentiometer
int potPin = A0;
double potVal = 0.0;

// Buttons in place of sensors
int button1 = 5;
int button2 = 6;
double pitch1 = 0;
double pitch2 = 36;
bool up1 = 1;
bool up2 = 1;
double t1 = 0;
double t2 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);  
  Serial.begin(31250);
}

void loop() {
  // Read value from pot, determine pitch value for the 1st sensor
  potVal = analogRead(potPin);  
//  pitches[1] = floor((potVal/1023)*17+36);
  pitch1 = floor((potVal/1023)*17+36);

  // Check for button presses
//  if (digitalRead(button1) == true and up1 == true){
//    sendNoteOn(pitch1,vel);    
//    note_is_on = 1;
//    t1 = millis();
//    up1 = false;
//  }
  if (digitalRead(button2) == true and up2 == true){
    sendNoteOn(pitch2,kickVel);   
    note_is_on = 1;
    t2 = millis();
    up2 = false;
  }

  // Check for sensor hits (over threshold)
  sensorVal1 = analogRead(sensorPin1);
//  Serial.println(sensorVal1);
  if (sensorVal1 > threshold and sensor1on == false){
    vel = floor((sensorVal1/1023)*127);
    sendNoteOn(pitch1,vel);
    note_is_on = 1;
    t1 = millis();
    sensor1on = true;
  }
  
//  for (int i = 0; i < n_sensors; i++){
//    double temp1 = analogRead(sensorPins[i]); 
//    sensorVals[i] = temp1;
//    Serial.println(temp1);
//    if (sensorVals[i] > threshold and sensorsOn[i] == false) {
//      vel = floor((potVal/1023)*17+36);
//      sendNoteOn(pitches[i],vel);
//      note_is_on = 1;
//      t1 = millis();
//      sensorsOn[i] = true;
//    }
//  }
    
  // Check for button release
//  if (digitalRead(button1) == false){
//    up1 = true;
//  }
  if (digitalRead(button2) == false){
    up2 = true;
  }

  
  // Check that the sensors are below threshold
  double temp1 = analogRead(sensorPin1);
  if (temp1 <= threshold){
    sensor1on = false;
  }
  
//  // Check that the sensors are below threshold
//  for (int i = 0; i < n_sensors; i++){
//    double temp2 = analogRead(sensorPins[i]);
//    if (temp2 <= threshold){
//      sensorsOn[i] = false;
//    }
//  }
//  
  // Turn notes off
  if (note_is_on == true){
    if (millis()-t1 > note_delay or millis()-t2 > note_delay){
      for (int i = 0; i < n_sensors; i++){
//        sendNoteOff(pitches[i]);
        sendNoteOff(pitch1);
        sendNoteOff(pitch2);
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
