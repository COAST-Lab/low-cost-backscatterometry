# Documentation

•	The goal is to create a device that measures turbidity in a water sample.  Components include: Particle Argon microprocessor attached via stacking headers to an Adalogger FeatherWing (which includes a PCF 8523 RTC and SD card slot-note there must always be a battery in the coin battery holder, even if its a dead one, or the device will lose track of time), Adafruit AW9523 LED driver, Adafruit 7341 sensor, IR LED, cuvette holder, light blocking cap, 12.5mm x 12.5mm x 45mm (outer dimension) cuvette, and housing.  The LED and sensor must be perpindicular to each other, which the cuvette holder is designed to accomodate. Light from the LED enters the sample, and after striking any turbidity-causing particles in the water sample, bounces off and is read by the sensor.  The more light that reaches the sensor, the higher the turbidity.

•	The CAD file for the cuvette holder is in the CAD directory, and is called "cuvette holder v3", which is the most up-to-date version.  The light blocking cap that goes with it is in the same directory.  Directions for how to assemble the device are also in the same directory, and is called "Cuvette Holder Notes and Specifications".  If printing a new cuvette holder, be advised that the inside walls of the cavities may need to be sanded/shaved, or their intended pieces may not fit.

• The housing for the device will need to be ordered.

•	 To flash the LED, there is a program in the following directory: Firmware/Turbidity_Argon.  As of 4 Aug, 2023, there are two versions: Turbidity_v3 and Turbidity_v4.  Both flash an IR LED for a set time interval while bright, and flashes for a set time interval while dim.  We do not want the LED to turn off completely, which is why the LED needs to switch back and forth from bright to dim.  This is a workaround for the sensor reading negative values, which will display as “0”.  During each interval, the AS7341 will take light measurements and record them to an SD card, and it will say whether the reading was during a bright or a dim interval.

•	Turbidity_v3 is a workable code, but just needs to be fine-tuned and condensed (see next steps).  Turbidity_v4 is an attempt to correct the issues in turbidity_v3, but is less usable than Turbidity_v3, and is a work in progress.

•	If sample collection and readings are needed before Turbidity_v4 is complete, then use Turbidity_v3.  Despite the issues with that version, it has fewer issues and does everything that is needed.

•	If testing Turbidity_v3 or Turbidity_v4, you can cover the sensor when the light goes dim, and uncover when the light goes bright; this will reflect in the readings.

•	The ATIME is set to 59, and ASTEP is set to 599, and Gain is set to AS7341_GAIN_4X, per Bob Brewin’s recommendation.  Using the equation: integration time = (ATIME + 1) x (ASTEP+1) x 2.78 µs, this leads to an integration time of 100.08 ms.  Integration time is the amount of time the sensor measures light, and the gain is the sensitivity of the sensor.  

•	Be advised: When flashing the program at each of the gain settings, it will not save to the SD card as that gain, it will only save as a number that increases by 1 as you increase to the next gain setting.  For example, the .5 setting outputs/saves to SD card as 0, and just increases by 1 for the next setting up.  Here it is, formatted as (Gain setting -- what it saves to the card as):

    o	AS7341_GAIN_0_5x -- 0
  
    o	AS7341_GAIN_1x -- 1
    
    o	AS7341_GAIN_2x -- 2
    
    o	AS7341_GAIN_4x -- 3
    
    o	AS7341_GAIN_8x -- 4
    
    o	AS7341_GAIN_16x -- 5
    
    o	AS7341_GAIN_32x -- 6
    
    o	AS7341_GAIN_64x -- 7
    
    o	AS7341_GAIN_128x -- 8
    
    o	AS7341_GAIN_256x -- 9
    
    o	AS7341_GAIN_512x – 10

•	In the code, the LED intensities (bright and dim) are listed as variables, which are: LEDbright and LEDdim.  The bright setting is 250 and the dim setting is 1 (this is to clearly distinguish between the bright and dim settings).  These are arbitrary; feel free to adjust those numbers as needed when using the IR LED.  

• For both versions, the time stamp includes: year, month, day, hour, minute, second, and millisecond.  Be advised that this is not synced up with the RTC (the RTC cannot process units smaller than a second, while the Argon records the time since it was last reset in milliseconds), however it is useful to determine the timing of the intervals.  Also, in the code, be advised that the serial print and fileprint record the timestamps differently (HH:MM:SS.mmm and HH:MM:SS:mmm, respectively).  If you try to format the fileprint timestamp as HH:MM:SS.mmm, it will not record the hours for whatever reason.  

•	Optical measurements are complete.  This process involved placing the sensor in a box with a 1-inch diameter hole, with the sensor 5 inches away from the hole.  The inside of the box was black.  Outside of the box, there was a posterboard mounted at a 45 degree angle to the hole, and reflects light into the hole.  The following measurements were needed: while the posterboard was facing the sun (or flat on the ground) during a sunny day; shortly after the first measurement, but with a square or disk completely shadowing the posterboard; and indoors with the hole covered.  For light and shadow runs, two 1-minute runs were conducted and recorded, and for the dark, one 1-minute run was conducted.  Note that for the dark measurements, in order to get any reading at all, the gain had to be increased to x512, (which is the highest gain), and the integration time had to be increased to 1000 ms (this required the ATIME to be set to 103-the highest it’ll let you go, and an ASTEP of 3457).  Even then, we had to gradually let more and more light into the box by slowly removing the cover.  The measurements were averaged, and given to Alan Holmes for processing.  The measurements collected can be found in the following directory: Data_Analysis/Data/27Jul_Turbidity_Optical_Calibration_Data.xlsx.  Alan’s results are in: Data_Analysis/Data/WilmingtonResult-072723.xlsx (use the numbers in yellow). 

•	In addition to the runs above, there was an additional dark counts run, this time with enough light so that the average ADU for the NIR was ~100, per Alan’s recommendation.  This can be found in the same Data folder that the other optical files are in, and is named “100ADU_Dark_2023_Jul_28_1104.csv.

•	In the Firmware directory, there is a program called Hawkeyewavelengths-this just cycles the Argon’s onboard LED through all the wavelengths in the Hawkeye satellite.  Be advised that running this will move control of the Argon to the user, and it cannot be used for other applications unless control is returned to the Argon.  To return control to the Argon, simply flash the Returncontroltoargon program (be sure to flash while the on-board LED is on!).

# Next Steps

•	Condense the coding into functions.  Right now Turbidity_v4 is condensed into functions, but the timing is off-it’ll sometimes record bright intervals (250) as dim (1), and vice versa.  Also it stays on an interval for a lot longer.  Turbidity_v3 does not have functions and is bulkier, but does not have these issues either.

•	The number of consecutive samples at a given LED intensity appears to be variable. Sometimes it's two, sometimes four. We'd need that to be a fixed value for future analyses. Ideally a variable that can be set just like the intensity itself.  

•	Assemble the turbidity device.  

•	Order the housing for the turbidity device.

•	Once the turbidity meter is assembled, the next step is calibration.  Google ISO 7027 and EPA 180.1 for reference, as these are the standards to abide by.  

•	After calibration, the device will be ready to take measurements.  
