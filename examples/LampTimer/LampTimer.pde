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

#define OutputCount 4
// These would be better handled as enums, 
// but that requires a seperate .h file.
#define OffTime 0
#define OnTime 1
#define OutputPin 2

// The outputs array defines how long each output will
// be turned off, on, and what pin to use for that output.
// The off and on values are in units of 'ticks'. The length
// of a tick is controlled by the setup of MsTimer2. 
                             // off   on  pin
byte outputs[OutputCount][3] ={{  5,  10,  13},   // Output A
                               { 15,  20,  12},   // Output B
                               {  2,  12,  11},   // Output C
                               { 10,   2,  10},}; // Output D
                    
void OutputHandler(FuseID fuseID, int outputID){
  // look up the pin associated with this output
  byte pin = outputs[outputID][OutputPin];

  // get and invert the current pin state and write
  // it back to the port to invert the current pin state.
  int state = 1&~digitalRead(pin);
  digitalWrite( pin, state );
  
  // Reset the fuse length with a new interval. The current state
  // of the pin is used to determine which interval should be used.
  eventFuse[fuseID].fuseLen = outputs[outputID][state];
}

void timerTick(){
  eventFuse.burn(1);
}

void setup() {
  // Set up and init all outputs to off
  for(byte i = 0; i<OutputCount; i++){
    pinMode( outputs[i][OutputPin], OUTPUT);
    digitalWrite( outputs[i][OutputPin], LOW );

    // Set up an event fuse for this output.
    eventFuse.newFuse( i, outputs[i][OffTime], INF_REPEAT, OutputHandler );
  }
  
  // Set MsTimer2 for one second per tick.
  MsTimer2::set(1000, timerTick );
  MsTimer2::start();
}

void loop(){
}