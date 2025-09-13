/*
 * can_cfg.h
 *
 *  Created on: Jun 18, 2025
 *      Author: Josu Alexadnru
 */

#ifndef SRC_COM_CAN_INC_CAN_CFG_H_
#define SRC_COM_CAN_INC_CAN_CFG_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"


/* Defines */
#define CAN_DATA_SIZE ((uint8_t) 8)

#define CAN_TX_ENABLE_MULTITASKING 0
#define CAN_RX_ENABLE_MULTITASKING 0


/* Variables */

/* CAN TxRx header structure */
typedef struct{
	/* Standard ID */
    uint32_t StdId;
    /* Data length */
    uint8_t DLC;
    /* Identifier type (Standard or Extended) */
    uint32_t IDE;
    /* Remote transmission request */
    uint32_t RTR;
}CAN_TxRxHeader_t;

/* CAN TxRx message structure */
typedef struct {
	CAN_TxRxHeader_t* header;
    /* Data bytes */
    uint8_t* data;
} CAN_TxRxMessage_t;

/* CAN Tx message structure */
typedef struct {
	CAN_TxHeaderTypeDef* header;
    /* Data bytes */
	uint8_t* data;
} CAN_TxMessage_t;

/* CAN Rx message structure */
typedef struct {
	CAN_RxHeaderTypeDef* header;
    /* Data bytes */
    uint8_t* data;
} CAN_RxMessage_t;

#endif /* SRC_COM_CAN_INC_CAN_CFG_H_ */
