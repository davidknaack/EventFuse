/*
 *
 * Description:
 * EventFuse example demonstrating control of 
 * multiple independent switched outputs. Each
 * output can be configured with independent
 * on and off durations with a minimum of 1 second
 * and a maximum of about 1100 hours (2^32 mS).
 *
 */
 
#include <EventFuse.h>
#include <MsTimer2.h>

#define OUTPUT_COUNT 4
#define OFF_TIME 0
#define ON_TIME 1
#define OUTPUT_PIN 2

// The outputs array defines how long each output will
// be turned off, on, and what pin to use for that output.
// The off and on values are in units of 'ticks'. The length
// of a tick is controlled by the setup of MsTimer2. 
                             // off   on  pin
byte outputs[OUTPUT_COUNT][3] ={{  5,  10,  2},   // Output A
                               { 15,  20,  3},   // Output B
                               {  2,  12,  4},   // Output C
                               { 10,   2,  5},}; // Output D
                    
void OutputHandler(FuseID fuseID, int& outputID){
  // look up the pin associated with this output
  byte pin = outputs[outputID][OUTPUT_PIN];

  // get and invert the current pin state and write
  // it back to the port to invert the current pin state.
  int state = 1&~digitalRead(pin);
  digitalWrite( pin, state );
  
  // Reset the fuse length with a new interval. The current state
  // of the pin is used to determine which interval should be used.
  EventFuse::fuses[fuseID].fuseLen = outputs[outputID][state];
}

void setup() {
  Serial.begin(115200);
  
  // Set up and init all outputs to off
  for(byte i = 0; i<OUTPUT_COUNT; i++){
    Serial.println(i);
    pinMode( outputs[i][OUTPUT_PIN], OUTPUT);
    digitalWrite( outputs[i][OUTPUT_PIN], LOW );

    // Set up an event fuse for this output.
    EventFuse::newFuse( i, outputs[i][OFF_TIME], INF_REPEAT, OutputHandler );
  }
  
  // Set MsTimer2 for one second per tick.
  MsTimer2::set(100, EventFuse::burn );
  MsTimer2::start();
}

void loop(){
}
