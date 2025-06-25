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

CAN_TxRxMessage_t txBuff[CAN_TX_BUFFER_SIZE];
CAN_TxRxMessage_t rxBuff[CAN_RX_BUFFER_SIZE];


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


typedef struct{
	CAN_BufferHeader_t header;
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexId_t mutex;
#endif
}CAN_TxBuffer_t;


bool CAN_TxBuffer_Init(CAN_TxBuffer_t* cb){
    if (cb == NULL) {
        return false;
    }
	memset(txBuff, 0, sizeof(txBuff));
	cb->header.buff = txBuff;
	cb->header.size = CAN_TX_BUFFER_SIZE;
	cb->header.count = 0;
	cb->header.head = 0;
	cb->header.tail = 0;
#if CAN_TX_ENABLE_MULTITASKING == 1
    osMutexAttr_t attr;
    attr.name = "CAN_TX_BUFFER";
    attr.attr_bits = osMutexPrioInherit;
    attr.cb_mem = NULL;
    attr.cb_size = 0;
    cb->mutex = osMutexNew(&attr);
    if(cb->mutex == NULL){
    	return false;
    }
#endif
    return true;
}

static bool CAN_Buffer_IsFull(CAN_BufferHeader_t* h){
	return ((h->tail + 1) % h->size) == h->head;
}

static bool CAN_Buffer_IsEmpty(CAN_BufferHeader_t* h){
	return h->tail == h->head;
}

static void CAN_Buffer_Add(CAN_BufferHeader_t* h, CAN_TxRxMessage_t* data){
	h->buff[h->tail] = *data;
	h->tail = (h->tail + 1) % h->size;
	h->count += 1;
}

static void CAN_Buffer_Get(CAN_BufferHeader_t* h, CAN_TxRxMessage_t* data){
	*data = h->buff[h->head];
	h->head = (h->head + 1) % h->size;
	h->count -= 1;
}

/**
 * @brief Adds a CAN message to the transmission buffer.
 *
 * This function attempts to add the provided CAN message to the transmission buffer.
 * It first checks for null pointers to ensure valid inputs. If multitasking is enabled,
 * it acquires a mutex to ensure thread-safe access to the buffer. The function then
 * checks if the buffer has space available; if the buffer is full, it releases the mutex
 * (if used) and returns false. Otherwise, it adds the message to the buffer, updates
 * the buffer's tail index and message count with wrap-around logic, and releases the mutex
 * (if used). The function returns true upon successful addition.
 *
 * @param cb Pointer to the CAN transmission buffer. Must be initialized.
 * @param data Pointer to the CAN message to add. Must be a valid message pointer.
 * @return true if the message was successfully added; false if input pointers are null or buffer is full.
 */
bool CAN_TxBuffer_Add(CAN_TxBuffer_t* cb, CAN_TxRxMessage_t* data){
    if (cb == NULL || data == NULL) {
        return false;
    }

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAcquire(cb->mutex, osWaitForever);
#endif

	CAN_BufferHeader_t* buffer_header_loc = &cb->header;

	if(CAN_Buffer_IsFull(buffer_header_loc) == true){
		#if CAN_TX_ENABLE_MULTITASKING == 1
			osMutexRelease(cb->mutex);
		#endif

		return false;
	}

	CAN_Buffer_Add(buffer_header_loc, data);

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexRelease(cb->mutex);
#endif

	return true;
}

/**
 * @brief Retrieves a CAN message from the transmission buffer.
 *
 * This function extracts the oldest message from the transmission buffer and stores it
 * in the provided data structure. It begins by validating the input pointers to prevent
 * null dereferencing. If multitasking is enabled, it acquires a mutex to ensure thread-safe
 * access to the buffer during retrieval. The function then checks if the buffer is empty;
 * if so, it releases the mutex (if used) and returns false to indicate no message was retrieved.
 * If the buffer contains messages, it copies the message at the head of the buffer into the provided
 * data pointer, updates the buffer's head index and message count with wrap-around logic,
 * and releases the mutex (if used). It returns true to indicate a successful retrieval.
 *
 * @param cb Pointer to the CAN transmission buffer. Must be initialized.
 * @param data Pointer to the CAN message structure where the retrieved message will be stored.
 * @return true if a message was successfully retrieved; false if the buffer was empty or input is invalid.
 */

bool CAN_TxBuffer_Get(CAN_TxBuffer_t* cb, CAN_TxRxMessage_t* data){
    if (cb == NULL || data == NULL) {
        return false;
    }


#if CAN_TX_ENABLE_MULTITASKING == 1
    osMutexAcquire(cb->mutex, osWaitForever);
#endif

    CAN_BufferHeader_t* buffer_header_loc = &cb->header;

    if (CAN_Buffer_IsEmpty(buffer_header_loc)) {
        #if CAN_TX_ENABLE_MULTITASKING == 1
        	osMutexRelease(cb->mutex);
        #endif

        return false;
    }

    CAN_Buffer_Get(buffer_header_loc, data);

#if CAN_TX_ENABLE_MULTITASKING == 1
    osMutexRelease(cb->mutex);
#endif

    return true;
}








