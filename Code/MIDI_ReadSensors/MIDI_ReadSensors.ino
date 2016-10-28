#define LED 13 // LED pin on Arduino Uno
// Sensor details
int sensorPin1 = A1;
const int n_sensors = 4;
int sensorPins[n_sensors] = {A0, A1, A2, A3};
double sensorVals[n_sensors] = {0.0};

// MIDI Details
double pitches[n_sensors] = {0.0};
int vel = 120;
int note_delay = 500;
int noteON  = 144; //144 = 10010000 in binary, note on command
int noteOFF = 128; //128 = 10000000 in binary, note off command

void setup() {
  Serial.begin(9600);
//  Serial.begin(31250);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Read values from the sensors
  //  for (int i = 0; i < n_sensors; i++){
  //    sensorVals[i] = analogRead(sensorPins[i]);
  //    pitches[i] = floor((sensorVals[i]/1023)*17+36);
  //  }
  sensorVals[0] = analogRead(sensorPins[0]); 
  pitches[0] = floor((sensorVals[0] / 1023) * 17 + 36);
  double val = analogRead(sensorPin1);
  int pitch = pitches[0];
//  Serial.print("Sensor: ");
  Serial.println(val);
//  Serial.print(", Pitch: ");
//  Serial.println(pitch);
  delay(10);

  // Send a MIDI note, based on the sensor value
//  sendMIDI(noteON, 36, vel);
//  delay(300);
//  sendMIDI(noteON, 37, vel);
//  delay(300);
//  sendMIDI(noteOFF, 36, vel);
//  delay(300);
//  sendMIDI(noteOFF, 37, vel);
//  delay(300);
}

void sendMIDI(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
  int i = 0;
  while (i < 10) {
    digitalWrite(LED, HIGH);
    delay(10);
    digitalWrite(LED, LOW);
    delay(10);
    i++;
  }
}

