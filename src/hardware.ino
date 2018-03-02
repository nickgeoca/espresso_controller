#include "globals.h"

// Pin setup
const uint32_t notifyPin2 =      D5;   // D31
const uint32_t notifyPin1 =      D16;  // Good
const uint32_t heaterPin =       D17;  // Maybe not owrking
const uint32_t pumpPinOut =      D19;  // Good
const uint32_t pumpPinIn =       D20;  // Good
const uint32_t temppumpGnd_pin = D21;  // Good
const uint32_t idac0Pin =        D22;  // Good
const uint32_t idacPin =         D23;  // Good
const uint32_t temp1Pin =        D37;  // Good
const uint32_t temp2Pin =        D38;  // Good

//**************************************************************************************************
//                                            initTimer
// Description:
//         Set EPCA (timer1) to generate irq at given frequency.

void initTimer(uint32_t targ_freq)
{
    uint32_t timer_freq, ovfl;
    Serial.println("Setup Timer");
    
    // Set prescaler
    Timer1.setPrescaleFactor(1000);
    Timer1.setCompare(1, 1);
    timer_freq = DIVIDE_INT(clk_get_bus_freq((clk_dev_id)10), Timer1.getPrescaleFactor());
    ovfl = DIVIDE_INT(timer_freq, targ_freq);
    Timer1.setOverflow(ovfl);
    Timer1.setMode(1, TIMER_OUTPUT_COMPARE);
    Timer1.attachInterrupt(1, NULL);
    
    Timer2.setPrescaleFactor(1000);
    Timer2.setCompare(1, 1);
    timer_freq = DIVIDE_INT(clk_get_bus_freq((clk_dev_id)10), Timer2.getPrescaleFactor());
    ovfl = DIVIDE_INT(timer_freq, 15);
    Timer2.setOverflow(ovfl);
    Timer2.setMode(1, TIMER_OUTPUT_COMPARE);
    Timer2.attachInterrupt(1, NULL);
}

void initIDAC(void) {
    SI32_IDAC_A_Type* reg = SI32_IDAC_1;
    Serial.println("Setup IDAC");
        
    *((volatile uint32_t *)0x4002D024) = (3 << 24);  // turn on clock to both idacs
    
    // TODO: Fix this line
    *((volatile uint32_t *)0x4002A0C8) = (1 << 13);  // analog. pbmdsel mode select
    
    reg->CONTROL.IDACEN = 1;
    reg->CONTROL.LOADEN = 0;    // load resistor enable
    reg->CONTROL.DMARUN = 0;
    reg->CONTROL.INFMT = 0b00;    // 10-bit sample
    reg->CONTROL.OUPDT = 0b111;  // on demand trigger
    reg->CONTROL.JSEL = 0;      // right justified
    reg->CONTROL.OUTMD = 0b01;  // 1mA

    reg->DATA.U32 = 0;  // 500mA
    
    reg = SI32_IDAC_0;
           *((volatile uint32_t *)0x4002A0C8) = (1 << 12);  // analog. pbmdsel mode select 
               reg->CONTROL.IDACEN = 1;
    reg->CONTROL.LOADEN = 0;    // load resistor enable
    reg->CONTROL.DMARUN = 0;
    reg->CONTROL.INFMT = 0b00;    // 10-bit sample
    reg->CONTROL.OUPDT = 0b111;  // on demand trigger
    reg->CONTROL.JSEL = 0;      // right justified
    reg->CONTROL.OUTMD = 0b01;  // 1mA

    reg->DATA.U32 = 0;  // 500mA
}

void initPins(void) {
    Serial.println("Setup Pins");
    
    pinMode(notifyPin1, OUTPUT);
    digitalWrite(notifyPin1, 1);
    pinMode(notifyPin2, OUTPUT);
    pinMode(pumpPinIn, INPUT_PULLUP);
    
    pinMode(temppumpGnd_pin, OUTPUT);
    digitalWrite(temppumpGnd_pin, 0);
    pinMode(pumpPinOut, OUTPUT);
    digitalWrite(pumpPinOut, 0);
    	
    // Pin setup
    pinMode(heaterPin, OUTPUT);
    digitalWrite(heaterPin, HEATER_OFF);
}
