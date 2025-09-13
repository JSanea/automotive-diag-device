/*
 * can_if.h
 *
 *  Created on: Jun 30, 2025
 *      Author: alx
 */

#ifndef SRC_COM_CAN_INC_CAN_IF_H_
#define SRC_COM_CAN_INC_CAN_IF_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "can_cbuffer.h"

/* Enums */
typedef enum{
	CANIF_OK,
	CANIF_NOT_OK
}CANIF_StatusTypeDef;


/* Variables */
extern CAN_TxCBuffer_t txBuffer;
extern CAN_RxCBuffer_t rxBuffer;

/* Functions */
extern bool CanIf_Init(void);
extern CANIF_StatusTypeDef CanIf_AddTxMessage(CAN_TxHeaderTypeDef *txHeader, uint8_t data[]);
extern CANIF_StatusTypeDef CanIf_Transmit(void);

#endif /* SRC_COM_CAN_INC_CAN_IF_H_ */
