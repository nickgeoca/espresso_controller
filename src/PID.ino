#include "PID.h"

float min(float x, float y) {
  if (x > y) return y;
  return x;
}

float max(float x, float y) {
  if (x < y) return y;
  return x;
}


// returns 0 to 1
float pidCalc(float set, float actl, pid_data_t &data) {
  float err, deriv, output;
  
  // Error
  err = set - actl;
  
  // PID Calc
  data.intgrl += err * data.dt;
  deriv = (err - data.prev_err) / data.dt;
  output = data.kp * err + data.ki * data.intgrl + data.kd * deriv;
  
  // Saturation filter
  output = min(output, 1);
  output = max(output, 0);
  
  // Update error
  data.prev_err = err;
  
  return output;
}
