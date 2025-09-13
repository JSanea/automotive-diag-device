/*
 * @file can_cbuffer2.c
 *
 *  Created on: Jul 3, 2025
 *      Author: Josu Alexandru
 */

#include <can_cbuffer.h>


/* Functions prototype */
CBuffer_StatusTypeDef CAN_TxBuff_Add(void* cbuff, void* data);
CBuffer_StatusTypeDef CAN_TxBuff_Get(void* cbuff, void* data);
CBuffer_StatusTypeDef CAN_RxBuff_Add(void* cbuff, void* data);
CBuffer_StatusTypeDef CAN_RxBuff_Get(void* cbuff, void* data);

/* Variables */
CAN_TxMessage_t txData[CAN_TX_BUFFER_SIZE];
CAN_RxMessage_t rxData[CAN_RX_BUFFER_SIZE];


/* Init Tx Buffer */
bool CAN_TxBuff_Init(CAN_TxCBuffer_t* can_cbuff){
	if(can_cbuff == NULL) return false;

	can_cbuff->cbuff.buff = txData;
	can_cbuff->cbuff.size = CAN_TX_BUFFER_SIZE;
	can_cbuff->cbuff.head = 0;
	can_cbuff->cbuff.tail = 0;
	can_cbuff->cbuff.count = 0;
	can_cbuff->cbuff.Add = CAN_TxBuff_Add;
	can_cbuff->cbuff.Get = CAN_TxBuff_Get;

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAttr_t txAttr;
	txAttr.name = "CAN_TX_BUFFER";
	txAttr.attr_bits = osMutexPrioInherit;
	txAttr.cb_mem = NULL;
	txAttr.cb_size = 0;
	can_cbuff->mutex = sMutexNew(&txAttr);
    if(can_cbuff->mutex == NULL){
    	return false;
    }
#endif
    return true;
}

/* Init Rx Buffer */
bool CAN_RxBuff_Init(CAN_RxCBuffer_t* can_cbuff){
	if(can_cbuff == NULL) return false;

	can_cbuff->cbuff.buff = rxData;
	can_cbuff->cbuff.size = CAN_RX_BUFFER_SIZE;
	can_cbuff->cbuff.head = 0;
	can_cbuff->cbuff.tail = 0;
	can_cbuff->cbuff.count = 0;
	can_cbuff->cbuff.Add = CAN_RxBuff_Add;
	can_cbuff->cbuff.Get = CAN_RxBuff_Get;

#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexAttr_t rxAttr;
	rxAttr.name = "CAN_RX_BUFFER";
	rxAttr.attr_bits = osMutexPrioInherit;
	rxAttr.cb_mem = NULL;
	rxAttr.cb_size = 0;
	can_cbuff->mutex = sMutexNew(&rxAttr);
    if(can_cbuff->mutex == NULL){
    	return false;
    }
#endif
    return true;
}


/**
 * @brief Adds a CAN message to the transmission buffer.
 *
 * This function inserts a CAN_TxMessage_t data item into the specified circular buffer.
 * It performs null checks, manages thread safety via mutex locking if multitasking
 * is enabled, checks for buffer fullness, and updates buffer indices accordingly.
 *
 * @param[in] cbuff Pointer to the CAN transmission buffer structure (CAN_TxCBuffer_t).
 * @param[in] data  Pointer to the CAN message to be added (CAN_TxMessage_t).
 *
 * @return CBuffer_State
 * - NULL_PARAM: If either cbuff or data is NULL.
 * - FULL: If the buffer is full and cannot accept new data.
 * - OK: If the data was successfully added to the buffer.
 *
 * @note
 * - If CAN_TX_ENABLE_MULTITASKING is enabled, a mutex lock is acquired before accessing
 *   the buffer and released after the operation to ensure thread safety.
 * - The buffer operates as a circular queue; the tail index wraps around when reaching the buffer size.
 */
CBuffer_StatusTypeDef CAN_TxBuff_Add(void* cbuff, void* data){
	if(cbuff == NULL || data == NULL) return CBUFFER_NULL_PARAM;

	CAN_TxCBuffer_t* const can_cbuff = (CAN_TxCBuffer_t*)cbuff;

	/* Multitasking Mutex Locking */
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAcquire(can_cbuff->mutex, osWaitForever);
#endif

	/* Check if buffer is full */
	if(CBuffer_IsFull(&can_cbuff->cbuff)){
		#if CAN_TX_ENABLE_MULTITASKING == 1
			osMutexRelease(can_cbuff->mutex);
		#endif

		return CBUFFER_FULL;
	}

	/* Copy data into buffer */
	CAN_TxMessage_t* const b = (CAN_TxMessage_t*)can_cbuff->cbuff.buff;
	if(b == NULL){
		return CBUFFER_NULL_PARAM;
	}
	CAN_TxMessage_t* const d = (CAN_TxMessage_t*)data;
	b[can_cbuff->cbuff.tail] = *d;

	/* Update tail and count */
	can_cbuff->cbuff.tail = (can_cbuff->cbuff.tail + 1) % can_cbuff->cbuff.size;
	can_cbuff->cbuff.count += 1;

	/* Release mutex if used */
#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexRelease(can_cbuff->mutex);
#endif

	return CBUFFER_OK;
}

/**
 * @brief Retrieves a CAN message from the transmission buffer.
 *
 * This function removes the oldest CAN message from the specified circular buffer
 * and stores it in the provided data pointer. It performs null checks, manages thread
 * safety via mutex locking if multitasking is enabled, checks for buffer emptiness,
 * and updates buffer indices accordingly.
 *
 * @param[in] cbuff Pointer to the CAN transmission buffer structure (CAN_TxCBuffer_t).
 * @param[out] data  Pointer to store the retrieved CAN message (CAN_TxMessage_t).
 *
 * @return CBuffer_State
 * - NULL_PARAM: If either cbuff or data is NULL.
 * - EMPTY: If the buffer is empty and cannot provide data.
 * - OK: If the data was successfully retrieved from the buffer.
 *
 * @note
 * - If CAN_TX_ENABLE_MULTITASKING is enabled, a mutex lock is acquired before accessing
 *   the buffer and released after the operation to ensure thread safety.
 * - The buffer operates as a circular queue; the head index wraps around when reaching the start.
 */
CBuffer_StatusTypeDef CAN_TxBuff_Get(void* cbuff, void* data){
	if(cbuff == NULL || data == NULL) return CBUFFER_NULL_PARAM;

	CAN_TxCBuffer_t* const can_cbuff = (CAN_TxCBuffer_t*)cbuff;

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexAcquire(can_cbuff->mutex, osWaitForever);
#endif

	if(CBuffer_IsEmpty(&can_cbuff->cbuff)){
		#if CAN_TX_ENABLE_MULTITASKING == 1
			osMutexRelease(can_cbuff->mutex);
		#endif

		return CBUFFER_EMPTY;
	}

	CAN_TxMessage_t* const b = (CAN_TxMessage_t*)can_cbuff->cbuff.buff;
	if(b == NULL){
		return CBUFFER_NULL_PARAM;
	}
	CAN_TxMessage_t* const d = (CAN_TxMessage_t*)data;
	*d = b[can_cbuff->cbuff.head];

	can_cbuff->cbuff.head = (can_cbuff->cbuff.head + 1) % can_cbuff->cbuff.size;
	can_cbuff->cbuff.count -= 1;

#if CAN_TX_ENABLE_MULTITASKING == 1
	osMutexRelease(can_cbuff->mutex);
#endif

	return CBUFFER_OK;
}

/**
 * @brief Adds a CAN receive message to the specified receive buffer.
 *
 * This function inserts a CAN message into the circular buffer associated with
 * the provided buffer handle. It handles synchronization if multitasking is enabled,
 * checks for buffer fullness, and updates buffer indices accordingly.
 *
 * @param cbuff Pointer to the buffer control structure (must be properly initialized).
 * @param data Pointer to the CAN message to add; must point to a valid CAN_RxMessage_t.
 *
 * @return CBuffer_State indicating the result:
 *         - OK: Message successfully added.
 *         - FULL: Buffer is full; message not added.
 *         - NULL_PARAM: Null pointer provided for buffer or data.
 *
 * @note
 * - If multitasking is enabled (CAN_RX_ENABLE_MULTITASKING == 1), mutex acquisition and release
 *   are handled to ensure thread safety.
 * - This function copies the message data into the buffer; the caller retains ownership of the input data.
 */
CBuffer_StatusTypeDef CAN_RxBuff_Add(void* cbuff, void* data){
	if(cbuff == NULL || data == NULL) return CBUFFER_NULL_PARAM;

	CAN_RxCBuffer_t* const can_cbuff = (CAN_RxCBuffer_t*)cbuff;

#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexAcquire(can_cbuff->mutex, osWaitForever);
#endif

	if(CBuffer_IsFull(&can_cbuff->cbuff)){
		#if CAN_RX_ENABLE_MULTITASKING == 1
			osMutexRelease(can_cbuff->mutex);
		#endif

		return CBUFFER_FULL;
	}

	CAN_RxMessage_t* const b = (CAN_RxMessage_t*)can_cbuff->cbuff.buff;
	if(b == NULL){
		return CBUFFER_NULL_PARAM;
	}
	CAN_RxMessage_t* const d = (CAN_RxMessage_t*)data;

	b[can_cbuff->cbuff.tail] = *d;
	can_cbuff->cbuff.tail = (can_cbuff->cbuff.tail + 1) % can_cbuff->cbuff.size;
	can_cbuff->cbuff.count += 1;

#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexRelease(can_cbuff->mutex);
#endif

	return CBUFFER_OK;
}

/**
 * @brief Retrieves a CAN message from the specified receive buffer.
 *
 * This function extracts the next available CAN message from the circular buffer.
 * It handles synchronization if multitasking is enabled, checks for buffer emptiness,
 * copies the message to the provided data pointer, and updates buffer indices.
 *
 * @param cbuff Pointer to the buffer control structure (must be properly initialized).
 * @param data Pointer to a CAN_RxMessage_t where the retrieved message will be stored.
 *
 * @return CBuffer_State indicating the result:
 *         - OK: Message successfully retrieved.
 *         - EMPTY: Buffer is empty; no message to retrieve.
 *         - NULL_PARAM: Null pointer provided for buffer or data.
 *
 * @note
 * - If multitasking is enabled (CAN_RX_ENABLE_MULTITASKING == 1), mutex acquisition and release
 *   are handled to ensure thread safety.
 * - The message is copied into the user-provided data buffer; the caller owns the data.
 */
CBuffer_StatusTypeDef CAN_RxBuff_Get(void* cbuff, void* data){
	if(cbuff == NULL || data == NULL) return CBUFFER_NULL_PARAM;

	CAN_RxCBuffer_t* const can_cbuff = (CAN_RxCBuffer_t*)cbuff;

#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexAcquire(can_cbuff->mutex, osWaitForever);
#endif

	if(CBuffer_IsEmpty(&can_cbuff->cbuff)){
		#if CAN_RX_ENABLE_MULTITASKING == 1
			osMutexRelease(can_cbuff->mutex);
		#endif

		return CBUFFER_EMPTY;
	}

	CAN_RxMessage_t* const b = (CAN_RxMessage_t*)can_cbuff->cbuff.buff;
	if(b == NULL){
		return CBUFFER_NULL_PARAM;
	}
	CAN_RxMessage_t* const d = (CAN_RxMessage_t*)data;

	*d = b[can_cbuff->cbuff.head];
	can_cbuff->cbuff.head = (can_cbuff->cbuff.head + 1) % can_cbuff->cbuff.size;
	can_cbuff->cbuff.count -= 1;

#if CAN_RX_ENABLE_MULTITASKING == 1
	osMutexRelease(can_cbuff->mutex);
#endif

	return CBUFFER_OK;
}












