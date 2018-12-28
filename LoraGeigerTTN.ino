#include <TheThingsNetwork.h>
#include <CayenneLPP.h>
#define MAXCNT 100 
#define CalFactor 1 


// Set your AppEUI and AppKey
const char *appEui = "";
const char *appKey = ""; 

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);

volatile int counter = 0; 
unsigned long oldTime = 0; 

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  attachInterrupt(0, count, FALLING); 
}

void loop()
{
  unsigned long time; 
  unsigned long dt; 
  float rate; 

  if (counter == MAXCNT) { 
    detachInterrupt(0); 
    time = millis(); 
    dt = time-oldTime; 
    rate = (float)MAXCNT*60.0*1000.0/(float)dt/CalFactor; 
    debugSerial.println(round(rate));
        
    lpp.reset();
    lpp.addLuminosity(1, rate); 
    ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
    delay(60000);
    
    oldTime = millis(); 
    counter = 0;     
    attachInterrupt(0, count, FALLING);   
  }   
}

void count() 
{ 
  counter++; 
}
