#include <SoftwareSerial.h>
#define LED 13
// From: http://itp.nyu.edu/physcomp/labs/labs-serial-communication/lab-midi-output-using-an-arduino/
 
 // Variables:
  byte note = 0;            // The MIDI note value to be played
 
 //software serial
 SoftwareSerial midiSerial(2, 3); // digital pins that we'll use for soft serial RX & TX
 
  void setup() {
    //  Set MIDI baud rate:
//    Serial.begin(9600);
      midiSerial.begin(31250); 
      pinMode(LED, OUTPUT);
  }
 
  void loop() {
    // play notes from F#-0 (30) to F#-5 (90):
    for (note = 30; note < 90; note ++) {
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0x90, note, 0x45);
      digitalWrite(LED, HIGH);
      delay(50);
      //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
      noteOn(0x90, note, 0x00);
      digitalWrite(LED, LOW);
      delay(50);
    }
  }
 
  //  plays a MIDI note.  Doesn't check to see that
  //  cmd is greater than 127, or that data values are  less than 127:
  void noteOn(byte cmd, byte data1, byte data2) {
    midiSerial.write(cmd);
    midiSerial.write(data1);
    midiSerial.write(data2);
 
     //prints the values in the serial monitor so we can see what note we're playing
//    Serial.print("cmd: ");
//     Serial.print(cmd);
//       Serial.print(", data1: ");
//   Serial.print(data1);
//      Serial.print(", data2: ");
//    Serial.println(data2);
// 
  }
