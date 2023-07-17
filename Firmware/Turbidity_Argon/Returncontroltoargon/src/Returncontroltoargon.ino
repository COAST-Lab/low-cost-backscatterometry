/** This program returns control of the RGB back to the Argon device.
  */

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{

RGB.control(false);
}
  
