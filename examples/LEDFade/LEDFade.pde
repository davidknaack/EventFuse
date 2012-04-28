/*
 *
 * Description:
 * Fade an LED by combining the output of two
 * fuses with similar intervals. The combined
 * output exhibits a beat pattern that varies
 * the output pulse width.
 *
 */
#include <EventFuse.h>

int ledPin = 13;       // LED output pin
boolean output = LOW;  // Output state

void FuseEvent(FuseID fuse, int userData){
  output = !output;
  digitalWrite( ledPin, output );
}

void setup() {
  pinMode(ledPin, OUTPUT);
  
  // Set up the two fade fuses
  eventFuse.newFuse( 150, INF_REPEAT, FuseEvent );
  eventFuse.newFuse( 152, INF_REPEAT, FuseEvent );
}

void loop(){
  delayMicroseconds(100);
  eventFuse.burn(1);
}