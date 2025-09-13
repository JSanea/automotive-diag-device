/*
 * can_drv.h
 *
 *  Created on: Jun 25, 2025
 *      Author: Josu Alexandru
 */

#ifndef SRC_COM_CAN_INC_CAN_DRV_H_
#define SRC_COM_CAN_INC_CAN_DRV_H_

#include "can_if.h"

/* Variables */
TaskHandle_t canTxTaskHandle;
TaskHandle_t canRxTaskHandle;

#endif /* SRC_COM_CAN_INC_CAN_DRV_H_ */
