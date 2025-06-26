/*
 * can_cbuffer.h
 *
 *  Created on: Jun 25, 2025
 *      Author: osu Alexandru
 */

#ifndef SRC_COM_CAN_INC_CAN_CBUFFER_H_
#define SRC_COM_CAN_INC_CAN_CBUFFER_H_


/* Defines */
#define CAN_TX_BUFFER_SIZE ((uint8_t) 32)
#define CAN_RX_BUFFER_SIZE ((uint8_t) 32)

/* Variables */
CAN_TxRxMessage_t TxBuff[CAN_TX_BUFFER_SIZE];
CAN_TxRxMessage_t RxBuff[CAN_RX_BUFFER_SIZE];

/* Enums */

/**
 * @brief Enumeration representing various statuses of the CAN buffer.
 */
typedef enum {
    OK,         /**< Operation was successful */
    NULL_PARAM, /**< A null pointer parameter was detected */
    FULL,       /**< The buffer is full and cannot accept more data */
    EMPTY       /**< The buffer is empty and no data is available */
} CAN_BufferStatus;

typedef struct{
	/* Pointer to data array */
	CAN_TxRxMessage_t* buff;
	/* Buffer size */
	uint8_t size;
	/* Index of the front element */
	uint8_t head;
	/*  Index where the next element will be added */
	uint8_t tail;
	/* Current number of elements in the buffer */
	uint8_t count;
}CAN_BufferHeader_t;


/* CAN Tx Buffer */
typedef struct{
	CAN_BufferHeader_t header;
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexId_t mutex;
#endif
}CAN_TxBuffer_t;

/* CAN Rx Buffer */
typedef struct{
	CAN_BufferHeader_t header;
#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexId_t mutex;
#endif
}CAN_RxBuffer_t;


#endif /* SRC_COM_CAN_INC_CAN_CBUFFER_H_ */
