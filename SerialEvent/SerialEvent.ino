/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 Created 9 May 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */
#include <Servo.h>

#define MAX_DEGREE 175 // max without weird jitter
#define MIN_DEGREE 2   // min without weird jitter

int degree = 0;
boolean stringComplete = false;  // whether the string is complete
int servoPin = 9;
Servo myServo;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  myServo.attach(9);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(degree);
    myServo.write(degree);
    // clear the string:
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    degree = (int)Serial.read();
    
    if (degree > MAX_DEGREE)
      degree = MAX_DEGREE;

    if (degree < MIN_DEGREE)
      degree = MIN_DEGREE;
      
    // add it to the inputString:
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    stringComplete = true;
  }
}


