#include "PID.h"
#include "IDAC.h"
#include "globals.h"
#include <SPI.h>
#include <stdint.h>

// Shutoff time
#define THIRTY_MINUTES (30 * 60 * 1000)
const uint32_t SHUT_OFF_TIME_MILIS = THIRTY_MINUTES;

void setup(void) {
    Serial.begin(115200);
    Serial.println("Setup-Start"); 
    
    initPins();
    initControl();
    setupPID();
    setupHardware();
    threadStart();
    
    Serial.println("Setup-End");
}

void loop(void) {  
  printTerm();  
    // Wait til there is data from the prior step
    if (threadReady()) {
        ctrlTask1();
        queue();

        resetCheck();
    }
}

#define QUEUE_SIZE 90
static log_data_t g_logData[QUEUE_SIZE];
static volatile uint32_t g_LD_head = 0;
static volatile uint32_t g_head_temp = 0;
void set_queue_head(void) {
    g_head_temp = (g_LD_head + 1) % QUEUE_SIZE;
}
log_data_t *get_next(void) {
    if (g_LD_head == g_head_temp) return 0;
    uint32_t tmp = g_head_temp;
    g_head_temp = (g_head_temp + 1) % QUEUE_SIZE;
    return &g_logData[tmp];
}
void queue(void) {
    //((uint8_t)((float)g_pumpSet / (float)pumpInputRange) * 100);
    g_logData[g_LD_head].power = (uint8_t)(mail_heaterControl.data.heat_set * 100 / 60);
    g_logData[g_LD_head].state = mail_heaterControl.data.state;
    g_logData[g_LD_head].set = mail_heaterControl.data.set_temp;
    g_logData[g_LD_head].water = mail_heaterControl.data.actl_temp;
    g_logData[g_LD_head].boiler = getTemp2();
    g_LD_head = (g_LD_head + 1) % QUEUE_SIZE;
}

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
    Timer2.attachInterrupt(1, ctrlLED);
}

void resetCheck(void) {
    // Reset if it's been running to long        
    if (millis() > SHUT_OFF_TIME_MILIS) {
        *(volatile uint32_t*)0x4002D060 = 0x00000040;
        while (1);
    }
}

void printTerm(void) {
    static uint32_t last_millis = 0;
    uint32_t delta_millis = millis() - last_millis;
    last_millis += delta_millis;    

    Serial.print(((float)g_pumpSet / (float)pumpInputRange) * 100);
    Serial.print('%');
    Serial.print("\ts: ");
    Serial.print(mail_heaterControl.data.heat_set);
    Serial.print("\ts: ");
    Serial.print(mail_heaterControl.data.state);
    Serial.print("\ti: ");
    Serial.print(sensorPIDData[mail_heaterControl.data.state].intgrl);
    Serial.print("\tdt: ");
    Serial.print(delta_millis);
    Serial.print("\tset: ");
    Serial.print(mail_heaterControl.data.set_temp);
    Serial.print("\tactl: ");
    Serial.print(mail_heaterControl.data.actl_temp);
    Serial.print("\tt2: ");
    Serial.println(getTemp2());
}

