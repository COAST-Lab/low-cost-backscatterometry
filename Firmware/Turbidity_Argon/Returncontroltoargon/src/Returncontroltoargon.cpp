/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Russe/OneDrive/Desktop/Coastlab/Returncontroltoargon/src/Returncontroltoargon.ino"
/** This program returns control of the RGB back to the Argon device.
  */

void setup();
#line 4 "c:/Users/Russe/OneDrive/Desktop/Coastlab/Returncontroltoargon/src/Returncontroltoargon.ino"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{

RGB.control(false);
}
  
