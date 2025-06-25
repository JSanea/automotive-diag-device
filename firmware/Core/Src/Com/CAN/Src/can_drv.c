/*
 * can_drv.c
 *
 *  Created on: Jun 18, 2025
 *      Author: Josu Alexandru
 */

#include "can_cfg.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"


/* Variables */
TaskHandle_t canTxTaskHandle;
TaskHandle_t canRxTaskHandle;

static void CAN_TxMailBoxCompleteCallback(CAN_HandleTypeDef *hcan);


void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	CAN_TxMailBoxCompleteCallback(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	CAN_TxMailBoxCompleteCallback(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	CAN_TxMailBoxCompleteCallback(hcan);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance != CAN1) return;

    CAN_RxHeaderTypeDef header;
    uint8_t data[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
        return;

    if (header.IDE != CAN_ID_STD || header.RTR != CAN_RTR_DATA)
        return;
//
//    CAN_RxFrame frame;
//    frame.std_id = header.StdId;
//    frame.len = header.DLC;
//    memcpy(frame.data, data, frame.len);
//
//    CAN_RxBuffer_Push(&frame);
}

/**
 * @brief  Called when a CAN transmission mailbox completes.
 *         Notifies the task responsible for CAN transmission.
 *
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 */

static void CAN_TxMailBoxCompleteCallback(CAN_HandleTypeDef *hcan)
{
	(void)hcan;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* Notify the task */
	vTaskNotifyGiveFromISR(canTxTaskHandle, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}










