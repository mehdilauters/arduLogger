#define DEFAULT_LOG_LEVEL 5
#include <Logger.h>


#define MODULE_ID 0
#define SIN_WAVE_ID 0

/*
corresponding xml file for client:

<ids>
     <module id="0" name="main" >
       <signal id="0" name="sin(t)" />
     </module>
     <module id="1" name="XXX" >
             <signal id="0" name="XX" />
             <signal id="1" name="YY" />
     </module>
</ids>

*/



Logger *m_logger;

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  m_logger = new Logger(&Serial, LOG_LEVEL_DEBUG);
}


void loop() {
  if(millis() % 1000)
  {
    VERBOSE("I'm verbose");
    DEBUG("I'm debug");
    INFO("I'm info");
    WARNING("I'm warning");
    ERROR("I'm ERROR");
  }

  LOG_ANALOG(sin(millis()), SIN_WAVE_ID, LOG_LEVEL_INFO);
  delay(1);
}