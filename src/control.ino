#include "globals.h"
#include "parameters.h"

const uint32_t previous_temps_size = 3;
volatile float previous_temperatures[previous_temps_size];

pid_data_t sensorPIDData[3];

void queue_temp(float val);
float get_avg_temp(void);

//**************************************************************************************************
// Controller
// Description:
//         Controls espresso temperature.

void ctrlHeater(void) {
  static int32_t heaterCntr = 0, heaterThreshold = 0, pump_state = mode_pump_off, pumpCntr = 0;

  heaterCntr = (heaterCntr + 1) % heaterInputRange;
  pumpCntr = (pumpCntr + 1) % pumpInputRange;

  // ---- Pump
  bool pumpChangedState = false;
  if (digitalRead(pumpPinIn)) {
    pumpChangedState = pump_state != mode_pump_on;
    pump_state = mode_pump_on;

    uint32_t powerPump = (pumpCntr > 3)?1:0; 
    digitalWrite(pumpPinOut, powerPump);
  }
  else {
    pumpChangedState = pump_state != mode_pump_off;
    pump_state = mode_pump_off;
  }

  // ---- Queue temperatures to average
  if (heaterCntr >= (heaterInputRange - previous_temps_size))
    queue_temp(getTemp1());

  // ---- Update PID
  if (heaterCntr == 0) {
    digitalWrite(heaterPin, HEATER_OFF);

    // If changed state, clear pid vals
    if (pumpChangedState) {
      sensorPIDData[pump_state].intgrl = 0;
      sensorPIDData[pump_state].prev_err = 0;
    }
    float temperature = get_avg_temp();

    float pid_rslt = pidCalc( SET_TEMP
                            , temperature
                            , sensorPIDData[pump_state]);
    
    heaterThreshold = (uint32_t)(pid_rslt * (float)heaterInputRange);

    // Set leds
    if (abs(temperature - SET_TEMP) < g_LEDOnBand)
      digitalWrite(notifyPin2, 1);
    else 
      digitalWrite(notifyPin2, 0);
  }
  
  // ---- Heater
  if (heaterCntr == heaterThreshold)
    togglePin(heaterPin);
}

//**************************************************************************************************
// Miscallaneous

void queue_temp(float val) {
  static uint32_t head = 0;

  previous_temperatures[head] = val;
  head = (head + 1) % previous_temps_size;
}

float get_avg_temp(void) {
  float sum = 0;

  for (uint32_t i = 0; i < previous_temps_size; i++) {
    sum += previous_temperatures[i];
  }

  return sum / (float)previous_temps_size;
}
