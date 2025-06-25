/*
 * can_cbuffer.c
 *
 *  Created on: Jun 25, 2025
 *      Author: Josu Alexandru
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "can_cfg.h"
#include "cmsis_os2.h"

#define CAN_TX_BUFFER_SIZE ((uint8_t) 32)
#define CAN_RX_BUFFER_SIZE ((uint8_t) 32)

typedef struct{
	/* Index of the front element */
	uint8_t head;
	/*  Index where the next element will be added */
	uint8_t tail;
	/* Current number of elements in the buffer */
	uint8_t count;
}CBufferAttr_t;


/* CAN Tx Buffer */
typedef struct{
	CAN_TxRxMessage_t buff[CAN_TX_BUFFER_SIZE];
	CBufferAttr_t attr;
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexId_t txMutex;
#endif
}CAN_TxCBuffer_t;

/* CAN Rx Buffer */
typedef struct{
	CAN_TxRxMessage_t buff[CAN_RX_BUFFER_SIZE];
	CBufferAttr_t attr;
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexId_t rxMutex;
#endif
}CAN_RxCBuffer_t;


static bool TxCBuffer_IsFull(CAN_TxCBuffer_t *cb){
	return ((cb->attr.tail + 1) % CAN_TX_BUFFER_SIZE) == cb->attr.head;
}

static bool TxCBuffer_IsEmpty(CAN_TxCBuffer_t *cb){
	return cb->attr.tail == cb->attr.head;
}

/**
 * @brief Initializes the CAN transmit buffer.
 *
 * This function clears the buffer, resets the buffer attributes (count, head, tail),
 * and initializes the mutex if multitasking is enabled.
 *
 * @param cb pointer to the CAN_TxCBuffer structure to initialize.
 */
void CAN_TxCBuffer_Init(CAN_TxCBuffer_t* cb){
	memset(cb->buff, 0, sizeof(cb->buff));
	cb->attr.count = 0;
	cb->attr.head = 0;
	cb->attr.tail = 0;
#if CAN_TX_ENABLE_MULTITASKING == 1
    osMutexAttr_t attr = {0};
    cb->txMutex = osMutexNew(&attr);
#endif
}

/**
 * @brief Adds a CAN message to the transmission buffer.
 *
 * This function inserts a new CAN transmission message into the buffer if there
 * is space available. It first checks if the buffer is full; if so, it returns
 * false. If multitasking is enabled, it acquires a mutex to ensure thread safety
 * during the buffer modification. The message is added at the tail position,
 * and the tail index and message count are updated with wrap-around logic. After
 * the insertion, the mutex is released (if used).
 *
 * @param cb pointer to the CAN transmission buffer.
 * @param data pointer to the CAN message to be added.
 * @return true if the message was successfully added; false if the buffer is full.
 */
bool CAN_TxCBuffer_Add(CAN_TxCBuffer_t* cb, CAN_TxRxMessage_t* data){
	if(TxCBuffer_IsFull(cb) == true){
		return false;
	}

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAcquire(cb->txMutex, osWaitForever);
#endif

	cb->buff[cb->attr.tail] = *data;
	cb->attr.tail =  (cb->attr.tail + 1) % CAN_TX_BUFFER_SIZE;
	cb->attr.count += 1;

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexRelease(cb->txMutex);
#endif

	return true;
}

/**
 * @brief Retrieves a CAN message from the transmission buffer.
 *
 * This function removes the oldest message from the buffer and stores it in the
 * provided data pointer. It first checks if the buffer is empty; if so, it
 * returns false to indicate no message could be retrieved. If multitasking is
 * enabled, it acquires a mutex to ensure thread safety during buffer access.
 * The message at the head of the buffer is copied to the provided data pointer,
 * and the head index and message count are updated with wrap-around logic.
 * The mutex is released afterward (if used).
 *
 * @param cb pinter to the CAN transmission buffer.
 * @param data pointer to the CAN message structure where the retrieved message will be stored.
 * @return true if a message was successfully retrieved; false if the buffer was empty.
 */
bool CAN_TxCBuffer_Get(CAN_TxCBuffer_t* cb, CAN_TxRxMessage_t* data){
	if(TxCBuffer_IsEmpty(cb) == true){
		return false;
	}

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAcquire(cb->txMutex, osWaitForever);
#endif

	*data = cb->buff[cb->attr.head];
	cb->attr.head =  (cb->attr.head + 1) % CAN_TX_BUFFER_SIZE;
	cb->attr.count -= 1;

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexRelease(cb->txMutex);
#endif

	return true;
}














