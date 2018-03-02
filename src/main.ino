#include "PID.h"
#include "IDAC.h"
#include "globals.h"
#include <SPI.h>
#include <stdint.h>

// Shutoff time
#define THIRTY_MINUTES (30 * 60 * 1000)
const uint32_t SHUT_OFF_TIME_MILIS = THIRTY_MINUTES;

void setup(void) {
  initPins();
  setupPID();
  setupHardware();
}

void loop(void) {  
  resetCheck();
}

static volatile uint32_t g_LD_head = 0;
static volatile uint32_t g_head_temp = 0;

void setupPID(void) {
    for (int i = 0; i < 3; i++) { 
        initPID(sensorPIDData[i], (float)heaterInputRange / 120); 
    }
	
    sensorPIDData[mode_pump_off].kp = 0.12;
    sensorPIDData[mode_pump_off].ki = 0.000;
    sensorPIDData[mode_pump_off].kd = 0.25004;

    sensorPIDData[mode_pump_on].kp = .09;//bitt/lessflav-.07; //.09
    sensorPIDData[mode_pump_on].ki = 0;
    sensorPIDData[mode_pump_on].kd = .05;//see above-.03; //.05
}

void setupHardware(void) {
    initIDAC();
	
    // Set timer to fire at 120Hz
    initTimer(120);			
    Timer1.attachInterrupt(1, ctrlHeater);
}

void resetCheck(void) {
  if (millis() < SHUT_OFF_TIME_MILIS) return;

  // Reset if it's been running to long          
  *(volatile uint32_t*)0x4002D060 = 0x00000040;
  while (1);
}
