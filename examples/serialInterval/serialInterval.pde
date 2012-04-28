// This example sends a Serial message every 250 milliseconds

#include <EventFuse.h> // Include the EventFuse library

void sendMessage(FuseID fuse, int userData){
  // Output all the analog readings separated by a space character
  for (int i = 0; i < 6; i++ ) {
    Serial.print (analogRead(i));
    Serial.print(' ',BYTE);
  }
  
  // Terminate message with a linefeed and a carriage return
  Serial.print('\r\n', BYTE);
}

void setup() {
  Serial.begin(115200); // Start the Serial communication
  eventFuse.newFuse( 250, INF_REPEAT, sendMessage );
}

void loop() {
  delay(1);
  eventFuse.burn(1);
}