/*
 * circular_buffer.h
 *
 *  Created on: Jul 3, 2025
 *      Author: Josu Alexandru
 */

#ifndef SRC_UTIL_INC_CBUFFER_H_
#define SRC_UTIL_INC_CBUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct CBuffer CBuffer_t;

/* Enums */
typedef enum{
    CBUFFER_OK,
    CBUFFER_FULL,
    CBUFFER_EMPTY,
    CBUFFER_NULL_PARAM
}CBuffer_StatusTypeDef;


/* Structures */
struct CBuffer {
    void* buff;
    /* Index of the front element */
    uint32_t head;
    /* Index where the next element will be added */
    uint32_t tail;
    /* Buffer size */
    uint32_t size;
    /* Current number of elements in the buffer */
    uint32_t count;
    /* Function to add element */
    CBuffer_StatusTypeDef (*Add)(void* cbuff, void* data);
    /* Function to get element */
    CBuffer_StatusTypeDef (*Get)(void* cbuff, void* data);
};

static inline bool CBuffer_IsFull(CBuffer_t* cbuff){
	if(cbuff == NULL) return true;
	return ((cbuff->tail + 1) % cbuff->size) == cbuff->head;
}

static inline bool CBuffer_IsEmpty(CBuffer_t* cbuff){
	if(cbuff == NULL) return true;
	return cbuff->tail == cbuff->head;
}


#endif /* SRC_UTIL_INC_CBUFFER_H_ */









