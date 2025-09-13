/*
 * uds_cfg.h
 *
 *  Created on: Jul 10, 2025
 *      Author: Josu Alexandru
 */

#ifndef SRC_UDS_INC_UDS_CFG_H_
#define SRC_UDS_INC_UDS_CFG_H_

/* CAN IDs / Request IDs */
#define BROADCAST_REQUEST_ID 0x7DF

/* Services IDs */
#define SID_DIAGNOSTIC_SESSION_CONTROL 0x10
#define SID_READ_DTC_INFO              0x19
#define SID_READ_DATA_BY_ID            0x22

/* Sub-Functions */
#define DEFAULT_SESSION             0x01
#define PROGRAMMING_SESSION         0x02
#define EXTENDED_DIAGNOSTIC_SESSION 0x03

/* Data Identifier */
#define DID_MANUFACTURER_ECU_SOFTWARE_VERSION 0xF189
#define DID_VEHICLE_INDENTIFICATION_NUMBER    0xF190


#endif /* SRC_UDS_INC_UDS_CFG_H_ */
