#include "globals.h"

// Control system data
const uint32_t heaterInputRange = 60;
const uint32_t pumpInputRange = 7;
const float g_LEDOnBand = 2;
const float g_setTemperature = 204;

volatile uint32_t g_heatSet = 0;
volatile uint32_t g_pumpSet = 0;
pid_data_t sensorPIDData[3];
volatile uint32_t g_LEDOn_True = 0;
volatile float moving_average[1];
thread_mail_t mail_heaterControl;

void queue_avg(float val) {
  static uint32_t head = 0;
  moving_average[head] = val;
  head += 1;
  head %= 1;
}
float get_avg(void) {
  float sum = 0;
  for (uint32_t i = 0; i < 1; i++) {
    sum += moving_average[i];
  }
  return sum / 1;
}

//**************************************************************************************************
//                                                ctrlHeater
// Description:
//         Controls espresso temperature.

void ctrlHeater(void)
{
    static int32_t ctr = 0, state = 1;
    ctr -= 1;
    
      printTerm();  
    // Run PID function before the start of the new step
    if (ctr <= 0) {
        digitalWrite(heaterPin, HEATER_OFF);
        ctr = heaterInputRange;

        // Calculate new output value
        queue_avg(getTemp1());
        mail_heaterControl.data.actl_temp = get_avg();
        
        mail_heaterControl.pending = 1;
        float pid_rslt = pidCalc(mail_heaterControl.data.set_temp, mail_heaterControl.data.actl_temp, sensorPIDData[mail_heaterControl.data.state]) * (mail_heaterControl.data.state == mode_pump_on ? g_pumpSet / pumpInputRange:1);
        mail_heaterControl.data.heat_set = (uint32_t)(pid_rslt * (float)heaterInputRange);
        float tmp = (mail_heaterControl.data.actl_temp - mail_heaterControl.data.set_temp) / mail_heaterControl.data.set_temp;
        tmp = (1 - tmp) * (1 - tmp);
        Timer1.setCompare(5, Timer1.getOverflow() - (uint32_t)((float)Timer1.getOverflow() * tmp * 3));
        //analogWrite(notifyPin2, 8);//abs((int32_t)((float)256 * ((float)mail_heaterControl.data.actl_temp - (float)mail_heaterControl.data.set_temp) / (float)mail_heaterControl.data.set_temp)));
        //Timer1.setCompare(5, (uint16_t)(Timer1.getOverflow()/ 2));
    }

    // Start of PID step
    if (ctr == mail_heaterControl.data.heat_set)
      togglePin(heaterPin);

    if (digitalRead(pumpPinIn)) 
      g_pumpSet = pumpInputRange;
    else
      g_pumpSet = 0;
    
    if (g_pumpSet > 0) {
        if (state == 1) {
            sensorPIDData[mode_pump_on].intgrl = 0;
            sensorPIDData[mode_pump_on].prev_err = 0;
            mail_heaterControl.data.state = mode_pump_on;
            state = 0;
        }

    }
    else {
        if (state == 0) {
            sensorPIDData[mode_pump_off].intgrl = 0;
            sensorPIDData[mode_pump_off].prev_err = 0;
            mail_heaterControl.data.state = mode_pump_off;
            state = 1;
        }
    }
    static uint32_t pumpCntr = 0;
    static uint32_t abc123 = 0;
    abc123 += 1;
    abc123 = abc123 % 2;
    if (abc123) return;
    uint32_t pumpOn = scale_onoff(pumpInputRange, g_pumpSet, pumpCntr);
    pumpCntr = (pumpCntr + 1) % pumpInputRange;
    digitalWrite(pumpPinOut, pumpOn);
   // Serial.print(pumpOn);
//uif (pumpCntr == 0)    Serial.println();
}

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

void ctrlTask1(void) {
    if (abs(mail_heaterControl.data.set_temp - mail_heaterControl.data.actl_temp) < g_LEDOnBand) {
        g_LEDOn_True = 1;
    }  
    else {
        g_LEDOn_True = 0;
    }
    // Output data from last step
    mail_heaterControl.pending = 0;
}

void ctrlLED(void) {
    static uint32_t ctr = 0;
      printTerm();  
    if (g_LEDOn_True) {
      if (ctr == 30) togglePin(notifyPin2);
      else ctr += 1;
    }
    else {
      digitalWrite(notifyPin2, 0);
      ctr = 0;
    }
}

void threadStart(void) {
        Serial.println("Thread start");
        
	// Begin control thread
	mail_heaterControl.pending = 0;
}

void threadBlock(void) {
    while (mail_heaterControl.pending == 0);
}

uint32_t threadReady(void) {
    return (uint32_t)mail_heaterControl.pending;
}

void initControl(void) {
    mail_heaterControl.data.set_temp = g_setTemperature;
    mail_heaterControl.data.state = mode_pump_off;
}
