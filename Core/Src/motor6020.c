/*
 * motor6020.c
 *
 *  Created on: 2026年11月3日
 *      Author: 20385
 */


/* ============================================================
 *  motor6020.c  —— 大疆 GM6020 电机 CAN 驱动库（实现文件）
 * ============================================================ */

#include "motor6020.h"

/* ================= 内部数据（外部看不到） ================= */

/* 8 台电机的状态表：下标 0~7 对应 1~8 号电机 */
volatile  motor6020_t s_motor[8];

/* 诊断计数（volatile 防止优化） */
volatile uint32_t motor6020_rx_count = 0;

/* ================= 初始化 ================= */
volatile uint32_t lasttime = 0;
void Motor6020_Init(CAN_HandleTypeDef *hcan)
{
    uint8_t i;

    /* 1. 让 CAN 进入工作状态 */
    HAL_CAN_Start(hcan);

    /* 2. 打开 FIFO0 接收中断 */
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

    /* 3. 配过滤器：全放行（ID 判断放在回调里做） */
    CAN_FilterTypeDef f = {0};
    f.FilterBank           = 0;
    f.FilterMode           = CAN_FILTERMODE_IDMASK;
    f.FilterScale          = CAN_FILTERSCALE_32BIT;
    f.FilterIdHigh         = 0x0000;
    f.FilterIdLow          = 0x0000;
    f.FilterMaskIdHigh     = 0x0000;
    f.FilterMaskIdLow      = 0x0000;
    f.FilterFIFOAssignment = CAN_RX_FIFO0;
    f.FilterActivation     = ENABLE;
    f.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(hcan, &f);

    /* 4. 状态清零 */
    for (i = 0; i < 8; i++) {
        s_motor[i].angle   = 0;
        s_motor[i].speed   = 0;
        s_motor[i].current = 0;
        s_motor[i].temp    = 0;
    }
}

/* ================= 发送电压 ================= */

void Motor6020_SetVolt(uint8_t id, int16_t volt)
{
    CAN_TxHeaderTypeDef tx = {0};
    uint8_t  data[8] = {0};
    uint32_t mbx = 0;

    if (id != 1) return;

    /* 电压限幅 */
    if (volt >  30000) volt =  30000;
    if (volt < -30000) volt = -30000;


    tx.StdId = 0x1FF;
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 8;

    /* 小端：低字节在前 */
    uint8_t idx = (uint8_t)((id - 1) % 4 * 2);
    data[idx]     = (uint8_t)(volt & 0xFF);
    data[idx + 1] = (uint8_t)((volt >> 8) & 0xFF);

    HAL_CAN_AddTxMessage(&hcan1, &tx, data, &mbx);
}

/* ================= 接收回调 ================= */

/* ★ 函数名是 HAL 定死的，一个字都不能改 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx = {0};
    uint8_t  data[8] = {0};
    uint8_t  idx;

    if (hcan->Instance != CAN1) return;

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx, data) != HAL_OK) return;

    if (rx.StdId < 0x205 || rx.StdId > 0x20C) return;

    idx = (uint8_t)(rx.StdId - 0x205);

    s_motor[idx].angle   = (uint16_t)((data[0] << 8) | data[1]);
    s_motor[idx].speed   = (int16_t) ((data[2] << 8) | data[3]);
    s_motor[idx].current = (int16_t) ((data[4] << 8) | data[5]);
    s_motor[idx].temp    = data[6];

    s_motor[idx].last_rx = HAL_GetTick();   /* 新加：记下这帧的时刻 */
    s_motor[idx].online  = 1;               /* 新加：标记在线 */
    motor6020_rx_count++;                   /* 新加：诊断计数 */
    lasttime = HAL_GetTick();
}
float Motor6020_GetSpeed(uint8_t motor_id)
{
    return s_motor[motor_id-1].speed;
}
uint16_t Motor6020_GetAngle(uint8_t motor_id)
{
    return s_motor[motor_id - 1].angle;
}
int16_t Motor6020_GetCurrent(uint8_t motor_id)
{
    if (motor_id < 1 || motor_id > 8) return 0;
    return s_motor[motor_id - 1].current;
}

uint8_t Motor6020_GetTemp(uint8_t motor_id)
{
    if (motor_id < 1 || motor_id > 8) return 0;
    return s_motor[motor_id - 1].temp;
}

motor6020_t *Motor6020_GetState(uint8_t motor_id)
{
    if (motor_id < 1 || motor_id > 8) return 0;
    return &s_motor[motor_id - 1];
}

void Motor6020_UpdateOnline(uint32_t timeout_ms)
{
    uint8_t  i;
    uint32_t now = HAL_GetTick();

    for (i = 0; i < 8; i++) {
        if (s_motor[i].online) {
            if (now - s_motor[i].last_rx > timeout_ms) {
                s_motor[i].online = 0;
            }
        }
    }
}