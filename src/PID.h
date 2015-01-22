#if !defined(_pid_lib_h_)
#define _pid_lib_h_

#include "Arduino.h"

typedef struct pid_data_t {
    float dt;  // time interval
    float kp;  // proportion
    float ki;  // integral
    float kd;  // derivative
    float prev_err;
    float intgrl;
} pid_data_t;

typedef struct thread_data_t {
    volatile float set_temp;
    volatile float actl_temp;
	volatile uint32_t heat_set;
	volatile uint32_t state;
} thread_data_t;

// Thread communication data
typedef struct thread_mail_t {
    volatile uint32_t pending;
    thread_data_t data;
} thread_mail_t;

float pidCalc(float set, float actl, pid_data_t &data);
void initPID(pid_data_t &data, float step_time);

#endif // !defined(_pid_lib_h_)
