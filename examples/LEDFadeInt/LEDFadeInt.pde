/*
 *
 * Description:
 * Fade an LED by combining the output of two
 * fuses with similar intervals. The combined
 * output exhibits a beat pattern that varies
 * the output pulse width.
 *
 * Use the interrupt based MsTimer2 library as
 * a source for a 1mS event for the burn function.
 *
 */
 
#include <EventFuse.h>
#include <MsTimer2.h>

int ledPin = 13;       // LED output pin
boolean output = LOW;  // Output state

void ToggleOutput(FuseID fuse, int userData){
  output = !output;
  digitalWrite( ledPin, output );
}

void tick(){
  eventFuse.burn(1);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  eventFuse.newFuse( 20, INF_REPEAT, ToggleOutput );
  eventFuse.newFuse( 21, INF_REPEAT, ToggleOutput );
  
  MsTimer2::set( 1, tick );
  MsTimer2::start();
}

void loop(){
}