/*
 * can_cbuffer.h
 *
 *  Created on: Jul 3, 2025
 *      Author: Josu Alexandru
 */

#ifndef SRC_COM_CAN_INC_CAN_CBUFFER_H_
#define SRC_COM_CAN_INC_CAN_CBUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include "can_cfg.h"
#include "cmsis_os2.h"

#include "../../../Util/Inc/cbuffer.h"

/* Defines */
#define CAN_TX_BUFFER_SIZE ((uint8_t) 32)
#define CAN_RX_BUFFER_SIZE ((uint8_t) 32)


typedef struct{
	CBuffer_t cbuff;
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexId_t mutex;
#endif
}CAN_TxCBuffer_t;

typedef struct{
	CBuffer_t cbuff;
#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexId_t mutex;
#endif
}CAN_RxCBuffer_t;

bool CAN_TxBuff_Init(CAN_TxCBuffer_t* can_cbuff);
bool CAN_RxBuff_Init(CAN_RxCBuffer_t* can_cbuff);
extern CBuffer_StatusTypeDef CAN_TxBuff_Add(void* cbuff, void* data);
extern CBuffer_StatusTypeDef CAN_TxBuff_Get(void* cbuff, void* data);
extern CBuffer_StatusTypeDef CAN_RxBuff_Add(void* cbuff, void* data);
extern CBuffer_StatusTypeDef CAN_RxBuff_Get(void* cbuff, void* data);

#endif /* SRC_COM_CAN_INC_CAN_CBUFFER_H_ */
