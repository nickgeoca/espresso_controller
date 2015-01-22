#include "PID.h"

void initPID(pid_data_t &data, float step_time)
{
    Serial.println("Setup PID");
    
    // Load temp sensor 1 PID data
    //memcpy(&temp1, USER_FLASH_LOC, sizeof(pid_data_t));

    // Init PID settings
    data.dt = step_time;
    data.intgrl = 0;
    data.prev_err = 0;
}

float pidCalc(float set, float actl, pid_data_t &data)
{
    float err, deriv, output;

    // Error
    err = set - actl;

    // PID Calc
    data.intgrl += err * data.dt;
    deriv = (err - data.prev_err) / data.dt;
    output = data.kp * err + data.ki * data.intgrl + data.kd * deriv;

    // Saturation filter
    output = output > 1 ? 1 : output; output = output < 0 ? 0 : output;

    // Update error
    data.prev_err = err;

    return output;
}
