/*
 * @file can_cbuffer.c
 *
 *  Created on: Jun 25, 2025
 *      Author: Josu Alexandru
 *
 * @brief Implements functions to add and retrieve CAN messages from RX and TX buffers.
 *
 * This file provides the functionality to handle CAN message buffering,
 * allowing messages to be stored in receive (RX) and transmit (TX) buffers,
 * and retrieved when needed for processing or transmission.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "can_cfg.h"
#include "cmsis_os2.h"
#include "can_cbuffer.h"

/* Function declaration */
static bool CAN_Buffer_IsFull(CAN_BufferHeader_t* h);
static bool CAN_Buffer_IsEmpty(CAN_BufferHeader_t* h);
static void CAN_Buffer_Add(CAN_BufferHeader_t* h, CAN_TxRxMessage_t* data);
static void CAN_Buffer_Get(CAN_BufferHeader_t* h, CAN_TxRxMessage_t* data);

/* Init Tx Buffer */
bool CAN_TxBuffer_Init(CAN_TxBuffer_t* cb){
    if (cb == NULL) {
        return false;
    }

	cb->header.buff = TxBuff;
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

/* Init Rx Buffer */
bool CAN_RxBuffer_Init(CAN_TxBuffer_t* cb){
    if (cb == NULL) {
        return false;
    }

	cb->header.buff = RxBuff;
	cb->header.size = CAN_RX_BUFFER_SIZE;
	cb->header.count = 0;
	cb->header.head = 0;
	cb->header.tail = 0;

#if CAN_RX_ENABLE_MULTITASKING == 1
    osMutexAttr_t attr;
    attr.name = "CAN_RX_BUFFER";
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
CAN_BufferStatus CAN_TxBuffer_Add(CAN_TxBuffer_t* cb, CAN_TxRxMessage_t* data){
    if (cb == NULL || data == NULL) {
        return NULL_PARAM;
    }

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAcquire(cb->mutex, osWaitForever);
#endif

	CAN_BufferHeader_t* buffer_header_loc = &cb->header;

	if(CAN_Buffer_IsFull(buffer_header_loc) == true){
		#if CAN_TX_ENABLE_MULTITASKING == 1
			osMutexRelease(cb->mutex);
		#endif

		return FULL;
	}

	CAN_Buffer_Add(buffer_header_loc, data);

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexRelease(cb->mutex);
#endif

	return OK;
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

CAN_BufferStatus CAN_TxBuffer_Get(CAN_TxBuffer_t* cb, CAN_TxRxMessage_t* data){
    if (cb == NULL || data == NULL) {
        return NULL_PARAM;
    }

#if CAN_TX_ENABLE_MULTITASKING == 1
    osMutexAcquire(cb->mutex, osWaitForever);
#endif

    CAN_BufferHeader_t* buffer_header_loc = &cb->header;

    if (CAN_Buffer_IsEmpty(buffer_header_loc)) {
        #if CAN_TX_ENABLE_MULTITASKING == 1
        	osMutexRelease(cb->mutex);
        #endif

        return EMPTY;
    }

    CAN_Buffer_Get(buffer_header_loc, data);

#if CAN_TX_ENABLE_MULTITASKING == 1
    osMutexRelease(cb->mutex);
#endif

    return OK;
}

/**
 * @brief Adds a new CAN message to the receive buffer (ring buffer).
 *
 * This function attempts to enqueue a CAN message into the specified buffer.
 * It performs null checks on input pointers, handles synchronization if multitasking
 * is enabled, checks for buffer fullness, and adds the message atomically.
 *
 * @param cb Pointer to the CAN transmit/receive buffer structure.
 *           Must be initialized prior to calling this function.
 * @param data Pointer to the CAN message data to be added.
 *             Must point to a valid CAN_TxRxMessage_t structure.
 *
 * @retval true  If the message was successfully added to the buffer.
 * @retval false If the buffer is full, or if either pointer is NULL.
 *
 * @note
 * - If CAN_RX_ENABLE_MULTITASKING is enabled (set to 1), this function acquires
 *   a mutex before accessing the buffer to ensure thread safety, and releases it afterward.
 * - If multitasking is disabled, the function operates without mutex locking.
 * - The function first checks for null pointers to prevent invalid memory access.
 * - It then checks if the buffer is full; if so, it returns false to indicate failure.
 * - If space is available, it adds the message to the buffer and returns true.
 */
CAN_BufferStatus CAN_RxBuffer_Add(CAN_TxBuffer_t* cb, CAN_TxRxMessage_t* data){
    if (cb == NULL || data == NULL) {
        return NULL_PARAM;
    }
#if CAN_RX_ENABLE_MULTITASKING == 1
    osMutexAcquire(cb->mutex, osWaitForever);
#endif

	CAN_BufferHeader_t* buffer_header_loc = &cb->header;

	if(CAN_Buffer_IsFull(buffer_header_loc) == true){
		#if CAN_RX_ENABLE_MULTITASKING == 1
			osMutexRelease(cb->mutex);
		#endif

		return FULL;
	}

	CAN_Buffer_Add(buffer_header_loc, data);

#if CAN_RX_ENABLE_MULTITASKING == 1
    osMutexRelease(cb->mutex);
#endif

    return OK;
}

/**
 * @brief Retrieves the oldest CAN message from the receive buffer (ring buffer).
 *
 * This function attempts to dequeue a CAN message from the specified buffer.
 * It performs null checks on input pointers, handles synchronization if multitasking
 * is enabled, checks if the buffer is empty, and retrieves the message atomically.
 *
 * @param cb Pointer to the CAN transmit/receive buffer structure.
 *           Must be initialized prior to calling this function.
 * @param data Pointer to a CAN_TxRxMessage_t structure where the retrieved message will be stored.
 *             Must point to a valid memory location.
 *
 * @retval true  If a message was successfully retrieved from the buffer.
 * @retval false If the buffer is empty, or if either pointer is NULL.
 *
 * @note
 * - If CAN_RX_ENABLE_MULTITASKING is enabled (set to 1), this function acquires
 *   a mutex before accessing the buffer to ensure thread safety, and releases it afterward.
 * - If multitasking is disabled, the function operates without mutex locking.
 * - The function first checks for null pointers to avoid invalid memory access.
 * - It then checks if the buffer is empty; if so, returns false indicating no data available.
 * - If data is available, it retrieves the message from the buffer and returns true.
 */
CAN_BufferStatus CAN_RxBuffer_Get(CAN_TxBuffer_t* cb, CAN_TxRxMessage_t* data){
    if (cb == NULL || data == NULL) {
        return NULL_PARAM;
    }

#if CAN_RX_ENABLE_MULTITASKING == 1
    osMutexAcquire(cb->mutex, osWaitForever);
#endif

    CAN_BufferHeader_t* buffer_header_loc = &cb->header;

    if (CAN_Buffer_IsEmpty(buffer_header_loc)) {
        #if CAN_RX_ENABLE_MULTITASKING == 1
        	osMutexRelease(cb->mutex);
        #endif

        return EMPTY;
    }

    CAN_Buffer_Get(buffer_header_loc, data);


#if CAN_RX_ENABLE_MULTITASKING == 1
    osMutexRelease(cb->mutex);
#endif

    return OK;
}


/* Static functions */

static bool CAN_Buffer_IsFull(CAN_BufferHeader_t* h){
	return ((h->tail + 1) % h->size) == h->head;
}

static bool CAN_Buffer_IsEmpty(CAN_BufferHeader_t* h){
	return h->tail == h->head;
}

static void CAN_Buffer_Add(CAN_BufferHeader_t* h, CAN_TxRxMessage_t* data){
	if(h == NULL || data == NULL){
		return;
	}

	h->buff[h->tail] = *data;
	h->tail = (h->tail + 1) % h->size;
	h->count += 1;
}

static void CAN_Buffer_Get(CAN_BufferHeader_t* h, CAN_TxRxMessage_t* data){
	if(h == NULL || data == NULL){
		return;
	}

	*data = h->buff[h->head];
	h->head = (h->head + 1) % h->size;
	h->count -= 1;
}


