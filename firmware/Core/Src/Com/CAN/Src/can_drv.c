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
#include "can_drv.h"


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
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* Add Rx Message to Rx Buffer */
	CanIf_GetRxMessage(hcan);

	/* Notify the task */
	vTaskNotifyGiveFromISR(canRxTaskHandle, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

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






