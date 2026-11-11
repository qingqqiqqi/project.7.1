/* ============================================================
 *  pid.c  —— 位置式 PID 控制器实现
 *  对应头文件：Core/Inc/pid.h
 *
 *  公式：
 *      err      = target - actual
 *      integral = integral + err
 *      out      = kp*err + ki*integral + kd*(err - last_err)
 *  输出按 out_max / out_min 限幅
 * ============================================================ */

#include "pid.h"

/* 初始化：设置参数，清零中间量 */
void PID_Init(pid_t *p, float kp, float ki, float kd, float out_max, float out_min)
{
    if (p == 0) {
        return;
    }
    p->kp       = kp;
    p->ki       = ki;
    p->kd       = kd;
    p->target   = 0.0f;
    p->integral = 0.0f;
    p->last_err = 0.0f;
    p->out_max  = out_max;
    p->out_min  = out_min;
}

/* 复位：清积分和上次误差（换目标值时建议调用） */
void PID_Reset(pid_t *p)
{
    if (p == 0) {
        return;
    }
    p->integral = 0.0f;
    p->last_err = 0.0f;
}

/* 计算：输入当前实际值，返回控制量 */
float PID_Calc(pid_t *p, float actual)
{
    float err;
    float out;

    if (p == 0) {
        return 0.0f;
    }

    err = p->target - actual;

    p->integral += err;
    if (p->integral >  100.0f) p->integral =  100.0f;
    if (p->integral < -100.0f) p->integral = -100.0f;
    out = p->kp * err
        + p->ki * p->integral
        + p->kd * (err - p->last_err);

    p->last_err = err;

    if (out > p->out_max) {
        out = p->out_max;
    } else if (out < p->out_min) {
        out = p->out_min;
    }

    return out;
}