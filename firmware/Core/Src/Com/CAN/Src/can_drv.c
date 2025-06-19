/*
 * can_drv.c
 *
 *  Created on: Jun 18, 2025
 *      Author: Josu Alexandru
 */

#include "can_cfg.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"


/**
 * @brief  Callback function called when a CAN transmission mailbox completes.
 *         Notifies the task responsible for CAN transmission.
 *
 * @param  hcan Pointer to the CAN handle structure.
 */

void HAL_CAN_TxMailBoxCompleteCallback(CAN_HandleTypeDef *hcan)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* Notify the task */
	vTaskNotifyGiveFromISR(canTxTaskHandle, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//    if(hcan->Instance == CAN1)
//    {
//        // Read the received message from FIFO0
//        CAN_RxHeaderTypeDef RxHeader;
//        uint8_t RxData[8];
//
//        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
//        {
//            // Process received message
//        }
//    }

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











