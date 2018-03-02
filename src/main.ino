#include "PID.h"
#include "IDAC.h"
#include "globals.h"
#include <SPI.h>
#include <stdint.h>
#include "parameters.h"

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

void setupPID(void) {
  sensorPIDData[mode_pump_off].dt = (float)heaterInputRange / 120; 
  sensorPIDData[mode_pump_off].intgrl = 0;
  sensorPIDData[mode_pump_off].prev_err = 0;
  sensorPIDData[mode_pump_off].kp = pumpOffKp;
  sensorPIDData[mode_pump_off].ki = pumpOffKi;
  sensorPIDData[mode_pump_off].kd = pumpOffKd;
  
  sensorPIDData[mode_pump_on].dt = (float)heaterInputRange / 120; 
  sensorPIDData[mode_pump_on].intgrl = 0;
  sensorPIDData[mode_pump_on].prev_err = 0;
  sensorPIDData[mode_pump_on].kp = pumpOnKp;
  sensorPIDData[mode_pump_on].ki = pumpOnKi;
  sensorPIDData[mode_pump_on].kd = pumpOnKd;
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
