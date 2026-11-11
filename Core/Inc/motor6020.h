/* ============================================================
 *  motor6020.h  —— 大疆 GM6020 电机 CAN 驱动库（头文件）
 *  作用：告诉外界"这个库有哪些函数可以调用"
 * ============================================================ */

#ifndef __MOTOR6020_H__
#define __MOTOR6020_H__

#include "main.h"          /* 用到 HAL 的 CAN_HandleTypeDef 等类型 */
#include "can.h"           /* 用到 hcan1 的声明 */

/* ---------------- 电机状态 ----------------
 * 一个电机有 4 个反馈量 + 2 个管理字段
 */
typedef struct {
    uint16_t angle;
    int16_t  speed;
    int16_t  current;
    uint8_t  temp;
    uint32_t last_rx;    /* 最后一次收到反馈的时刻（ms） */
    uint8_t  online;     /* 1 = 在线，0 = 离线 */
} motor6020_t;

/* ---------------- 对外函数 ---------------- */

/* 初始化：Start + 开中断 + 配过滤器 */
void     Motor6020_Init(CAN_HandleTypeDef *hcan);

/* 发送电压指令：id 1~8，volt 单位 mV，范围 -30000 ~ +30000 */
void     Motor6020_SetVolt(uint8_t id, int16_t volt);

/* 读取反馈（4 个 getter） */
uint16_t Motor6020_GetAngle(uint8_t id);
float  Motor6020_GetSpeed(uint8_t id);
int16_t  Motor6020_GetCurrent(uint8_t id);
uint8_t  Motor6020_GetTemp(uint8_t id);
extern volatile uint32_t lasttime;
/* 读整个状态结构体（CubeMonitor 也用得到） */
motor6020_t *Motor6020_GetState(uint8_t id);
void Motor6020_UpdateOnline(uint32_t timeout_ms);
#endif /* __MOTOR6020_H__ */
