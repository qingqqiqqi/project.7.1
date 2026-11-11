#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float target;
    float integral;
    float last_err;
    float out_max;
    float out_min;
} pid_t;

void  PID_Init(pid_t *p, float kp, float ki, float kd, float out_max, float out_min);
float PID_Calc(pid_t *p, float actual);
void  PID_Reset(pid_t *p);

#endif
