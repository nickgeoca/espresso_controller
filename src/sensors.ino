#include "globals.h"

// Defines
#define TEMP_K_TO_F(t)    (((float)t - 273.15) * 9 / 5 + 32)
#define TEMP_F_TO_K(t)    (((float)t - 32) * 5 / 9 + 273.15)
//**************************************************************************************************
//                                                getTemp1
// Description:
//         Finds temp sensor one based on voltage divider and thermistor constants.
// Returns:
//         Fahrenheit 

float getTemp1(void) {
  // Temperature sensor data
  const float temp1Beta = 4500;
  const float temp1Rinf = 0.027871559;

  float temp, resistance = 0;
  uint32_t a_read;
  const uint32_t milli_amps = 1000;

  SI32_IDAC_1->DATA.U32 = milli_amps;

  for (int j = 0; j < 256; j++) {
    a_read = 0;
    // Get analog value. Increase resolution to 14 bits.
    for (int i = 0; i < 4; i++) a_read += analogRead(temp1Pin);
    a_read = a_read << 1;
    a_read /= 4;
    resistance += (float)(a_read) * 3300 * 1000 / (4096 * 2 * milli_amps);
  }
  SI32_IDAC_1->DATA.U32 = 0;
  resistance /= 256;
#if 0
  // calc resistance. Voltage divider with 6.49k resistor in series
  resistance = (float)(a_read) * 6490 / ((4096 * 8) - (float)(a_read));
  
  // Thermistor equation = Beta / log(R / Rinf)
  temp = temp1Beta / log(resistance / temp1Rinf);
#endif
  temp = (resistance / 100 - 1) / .00375;
  return (temp * 1.8) + 32 - 14.7 - 19 - 6;
}

float getTemp2(void) {
  float temp, resistance = 0;
  uint32_t a_read;
  const uint32_t milli_amps = 250;
  
  SI32_IDAC_0->DATA.U32 = milli_amps;
  for (int j = 0; j < 256; j++) {
    a_read = 0;
    // Get analog value. Increase resolution to 14 bits.
    for (int i = 0; i < 4; i++) a_read += analogRead(temp2Pin);
    a_read = a_read << 1;
    a_read /= 4;
    resistance += (float)(a_read) * 3300 * 1000 / (4096 * 2 * milli_amps);
  }
  SI32_IDAC_0->DATA.U32 = 0;
  resistance /= 256;
  // thermistor equation
  // temperature = Beta / log(R / R_inf)
  temp = 3984 / log(resistance / 0.015731958);

  return TEMP_K_TO_F(temp);
}
