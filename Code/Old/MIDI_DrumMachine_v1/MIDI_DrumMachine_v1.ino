#include <MIDI.h>

// Use Hairless MIDI to get this  working!!

// Simple tutorial on how to receive and send MIDI messages.
// Here, when receiving any message on channel 4, the Arduino
// will blink a led and play back a note for 1 second.
MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 13              // LED pin on Arduino Uno
int midi_ch_out = 1;
int midi_ch_in  = 4;
int piezoPin = 8;
int sensorPin = A0;
double sensorVal = 0.0;

int button1 = 5;
int button2 = 6;
double pitch1 = 0;
double pitch2 = 36;
bool up1 = 1;
bool up2 = 1;
double t1 = 0;
double t2 = 0;
double note_delay = 500;
int vel = 90;

bool note_is_on = 0;
bool turn_off_notes = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  MIDI.begin(midi_ch_in);
  Serial.begin(115200);
}

void loop() {
  // Read value from pot, determine pitch value
  sensorVal = analogRead(sensorPin);  
  pitch1 = floor((sensorVal/1023)*17+36);

  // Check for button presses
  if (digitalRead(button1) == true and up1 == true){
    MIDI.sendNoteOn(pitch1,vel,midi_ch_out);
    tone(piezoPin, sensorVal);
    note_is_on = 1;
    t1 = millis();
    up1 = false;
  }
  if (digitalRead(button2) == true and up2 == true){
    MIDI.sendNoteOn(pitch2,vel,midi_ch_out);
    tone(piezoPin, sensorVal);
    note_is_on = 1;
    t2 = millis();
    up2 = false;
  }

  // Check for button release
  if (digitalRead(button1) == false){
    up1 = true;
  }
  if (digitalRead(button2) == false){
    up2 = true;
  }
  
  // Turn notes off
  if (note_is_on == true){
    if (millis()-t1 > note_delay or millis()-t2 > note_delay){
      MIDI.sendNoteOff(pitch1,0,midi_ch_out);
      MIDI.sendNoteOff(pitch2,0,midi_ch_out);
      noTone(piezoPin);
      note_is_on = 0;   
    }
  }

  delay(5);

}




























