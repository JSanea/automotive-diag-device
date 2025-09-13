/*
 * can_if.c
 *
 *  Created on: Jun 30, 2025
 *      Author: Josu Alexandru
 *
 */
#include <can_cbuffer.h>
#include <stdbool.h>
#include "can_if.h"
#include "can_cfg.h"

extern CAN_HandleTypeDef hcan1;

CAN_TxCBuffer_t txBuffer;
CAN_RxCBuffer_t rxBuffer;


bool CanIf_Init(void){
	return CAN_TxBuff_Init(&txBuffer) && CAN_RxBuff_Init(&rxBuffer);
}


CANIF_StatusTypeDef CanIf_AddTxMessage(CAN_TxHeaderTypeDef *txHeader, uint8_t data[]){
	CAN_TxMessage_t msg;

	if(txHeader == NULL || data == NULL) return false;

	/* Prepare CAN message */
	msg.data = data;
	msg.header = txHeader;

	/* Add message to Tx Buffer */
	if(txBuffer.cbuff.Add(&txBuffer, &msg) != CBUFFER_OK){
		return CANIF_NOT_OK;
	}

	return CANIF_OK;
}

CANIF_StatusTypeDef CanIf_Transmit(void){
	CAN_TxMessage_t msg;
	uint32_t txMailbox;

	if(txBuffer.cbuff.Get(&txBuffer, &msg) == CBUFFER_OK){
		if(HAL_CAN_AddTxMessage(&hcan1, msg.header, msg.data, &txMailbox) == HAL_OK){
			return CANIF_OK;
		}
	}

	return CANIF_NOT_OK;
}

CANIF_StatusTypeDef CanIf_Receive(CAN_RxMessage_t* msg){
	if(msg == NULL) return CANIF_NOT_OK;


	if(rxBuffer.cbuff.Get(&rxBuffer, msg) != CBUFFER_OK){
		return CANIF_NOT_OK;
	}

	return CANIF_OK;
}


void CanIf_GetRxMessage(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef header;
    uint8_t data[CAN_DATA_SIZE];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
        return;

    if (header.IDE != CAN_ID_STD || header.RTR != CAN_RTR_DATA)
        return;

    /* Prepare CAN message */
    CAN_RxMessage_t msg;
    msg.header = &header;
    msg.data = data;

    /* Add CAN message to Rx Buffer */
    rxBuffer.cbuff.Add(&rxBuffer, &msg);
}


