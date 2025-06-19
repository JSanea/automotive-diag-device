/*
 * diag.c
 *
 *  Created on: Jun 14, 2025
 *      Author: Josu Alexandru
 *
 * @file diag.c
 * @brief UDS/OBD-II diagnostic protocol implementation over CAN bus.
 *
 * This module provides basic functions to send and receive UDS diagnostic
 * requests and responses over the CAN bus using STM32 HAL. It supports
 * common service IDs (SIDs) like reading ECU information, fault codes (DTCs),
 * and vehicle identification data (VIN).
 *
 */
