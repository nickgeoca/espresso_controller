#include "globals.h"

// Parameters
const uint32_t previous_temps_size = 1;
const uint32_t heaterInputRange = 60;
const uint32_t pumpInputRange = 7;
const float g_LEDOnBand = 2;
const float g_setTemperature = 200;

volatile uint32_t g_heatSet = 0;
pid_data_t sensorPIDData[3];
volatile uint32_t g_LEDOn_True = 0;
volatile float previous_temperatures[previous_temps_size];
thread_mail_t mail_heaterControl;

void queue_temp(float val);
float get_avg_temp(void);
uint32_t scale_onoff(uint32_t scl, uint32_t val, uint32_t cnt);

//**************************************************************************************************
// Controller
// Description:
//         Controls espresso temperature.

void ctrlHeater(void)
{
  static int32_t ctr = 0, state = 1, heat_cntr, pump_state;
  static bool pumpChangedState = false;

  ctr += 1;
  
  // Run PID function before the start of the new step
  if (ctr > heaterInputRange) {
    ctr = 0;
    queue_temp(getTemp1());
    digitalWrite(heaterPin, HEATER_OFF);

    float temperature = get_avg_temp();

    // Calculate pid and scale based on pump speed
    float pid_rslt = pidCalc( g_setTemperature
                            , temperature
                            , sensorPIDData[mail_heaterControl.data.state]);
    if (pump_state == mode_pump_on) pid_rslt = pid_rslt * pump_cntr / pumpInputRange;
    
    heat_cntr = (uint32_t)(pid_rslt * (float)heaterInputRange);
  }
  
  // ---- Heater
  if (ctr == heat_cntr)
    togglePin(heaterPin);
  
  // ---- Pump
  if (digitalRead(pumpPinIn)) {
    pumpChangedState = pump_state != mode_pump_on;
    pump_state = mode_pump_on;
  }
  else {
    pumpChangedState = pump_state != mode_pump_off;
    pump_state = mode_pump_off;
  }
  
  // Clear PID if pump changed state
  if (pumpChangedState) {
    sensorPIDData[pump_state].intgrl = 0;
    sensorPIDData[pump_state].prev_err = 0;
  }


  static uint32_t pumpCntr = 0;

  uint32_t pumpOn = scale_onoff(pumpInputRange, pump_cntr, pumpCntr);
  pumpCntr = (pumpCntr + 1) % pumpInputRange;
  digitalWrite(pumpPinOut, pumpOn);
}

//**************************************************************************************************
// Miscallaneous

// (cnt + 1) % (scl / (val + 1))
uint32_t scale_onoff(uint32_t scl, uint32_t val, uint32_t cnt) {
  uint32_t xr = 0;
  if (val == 0) return 0;
  if (val == scl) return 1;
  if ((scl / val) == 1) xr = 1;
  if (xr) val = scl - val;
  if (cnt % (scl / val) != 0) return 0 ^ xr;
  return 1 ^ xr;
}


void ctrlLED(void) {
  static uint32_t ctr = 0;

  bool ledon = abs(mail_heaterControl.data.set_temp - mail_heaterControl.data.actl_temp) < g_LEDOnBand);

  if (ledon) {
    if (ctr == 30) togglePin(notifyPin2);
    else ctr += 1;
  }
  else {
    digitalWrite(notifyPin2, 0);
    ctr = 0;
  }
}


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
