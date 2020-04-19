#include <Wire.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

// OLED constants and instance

#define PIN_RESET 9  
#define DC_JUMPER 1 
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration

// Bio Sensor constants and intstance

// No other Address options.
#define DEF_ADDR 0x55

// Reset pin, MFIO pin
const int resPin = 4;
const int mfioPin = 5;

// Takes address, reset pin, and MFIO pin.
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin, DEF_ADDR); 

bioData body;  

void fatal(void)
{
  while(1);
}

void setup()
{
  bool fatal = false;
  
  delay(100);

  // I2C initialization
  Wire.begin();

  // OLED initialization
  oled.begin();    // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory

  // Set up for initialization messages
  oled.setFontType(0);  // Set font to type 0
  oled.setCursor(0, 0); // Set cursor to top-left

  // Bio Sensor initialization
  int result = bioHub.begin();
  if (!result) {
    oled.println("Started");
  } else {
    oled.println("Could not communicate with the sensor!!!");
    fatal = true;
  }

  
  oled.println("Config..."); 
  int error = bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings. 
  if(!error){
    oled.println("Configured");
  }
  else {
    oled.println("Error configuring sensor.");
    oled.print("Error: "); 
    oled.println(error);
    fatal = true; 
  }

  oled.display();

  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up.
  delay(4000);

  if (fatal) {
    while(1);
  }
}

void loop()
{

    // Information from the readBpm function will be saved to our "body"
    // variable.  
    body = bioHub.readBpm();
    
    oled.clear(PAGE);
    oled.setCursor(0,0);
    oled.print("Pulse: ");
    oled.println(body.heartRate); 
    oled.print("Conf:  ");
    oled.println(body.confidence); 
    oled.print("O2 %:  ");
    oled.println(body.oxygen); 
    oled.print("Stat:  ");
    oled.println(body.status);
    oled.display();
     
    delay(250); // Slowing it down, we don't need to break our necks here.
}
