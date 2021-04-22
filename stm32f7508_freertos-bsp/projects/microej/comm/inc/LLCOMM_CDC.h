/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLCOMM_CDC
#define _LLCOMM_CDC

/* Includes ------------------------------------------------------------------*/

#include "LLCOMM_BUFFERED_CONNECTION.h"
#include "LLCOMM_CDC_configuration.h"
#include "stm32f7xx_hal.h"
#include "usbh_core.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Defines and Macros --------------------------------------------------------*/

/*
 * Defines the software FIFO size for transmission used by MicroEJ framework.
 * Default size is 5 bytes.
 */
#ifndef LLCOMM_CDC_TX_FIFO_SIZE
#define LLCOMM_CDC_TX_FIFO_SIZE 5
#endif

/*
 * Defines the software FIFO size for reception used by MicroEJ framework
 * Default size is 1024 bytes.
 */
#ifndef LLCOMM_CDC_RX_FIFO_SIZE
#define LLCOMM_CDC_RX_FIFO_SIZE 1*1024
#endif

#define LLCOMM_CDC_RX_BUFFER_SIZE  64
#define LLCOMM_CDC_TX_BUFFER_SIZE  64

/* Enumerations --------------------------------------------------------------*/

typedef enum
{
	CDC_STATUS_IDLE = 0,
	CDC_STATUS_CONNECTED = 1,
	CDC_STATUS_DISCONNECTED = 2,
} cdc_status;

/* Structures ----------------------------------------------------------------*/

typedef struct LLCOMM_CDC
{
	/**
	 * MicroEJ LLCOMM connection header
	 */
	struct LLCOMM_BUFFERED_CONNECTION header;

	/**
	 * Pointer on HAL USBH
	 */
	USBH_HandleTypeDef* hUSBHost;

	/**
	 * USBH device ID (seems useless because ST USBH does manage more than one device
	 * per USB connection)
	 */
	uint8_t id;

	/**
	 * Connection status
	 */
	cdc_status status;

	/**
	 * Software FIFO for transmission used by MicroEJ framework
	 */
	uint8_t txFIFOBuffer[LLCOMM_CDC_TX_FIFO_SIZE];

	/**
	 * Software FIFO for reception used by MicroEJ framework
	 */
	uint8_t rxFIFOBuffer[LLCOMM_CDC_RX_FIFO_SIZE];

	/**
	 * Driver RX buffer.
	 */
	uint8_t rxBuffer[LLCOMM_CDC_RX_BUFFER_SIZE];

	/**
	 * Driver TX buffer.
	 */
	uint8_t txBuffer[LLCOMM_CDC_TX_BUFFER_SIZE];

	/**
	 * Address in TX buffer of the next byte to be wrote
	 */
	uint8_t* txBufferPtr;

	/**
	 * Flag indicating that a TXAsync has been requested
	 */
	uint8_t txInProgress;

	/**
	 * Number of bytes being sent
	 */
	int32_t txLastBuffLength;

	/**
	 * Pending address in RX buffer of the next byte to send to the connection.
	 */
	uint8_t* rxPendingBufferPtrStart;

	/**
	 * Pending address in RX buffer of the last byte + 1 to send to the connection.
	 */
	uint8_t* rxPendingBufferPtrEnd;

	/**
	 * Pending flag indicated transmit buffer is ready (not yet sent to the connection)
	 */
	uint8_t pendingTransmitReady;

	/**
	 * When true, stack is ready to send some data
	 */
	uint8_t isTxEnabled;
	/**
	 * When true, driver is allowed to notify the stack that transmit is ready
	 */
	uint8_t isTxInterruptEnabled;

	/**
	 * When true, stack is ready to accept incoming data
	 */
	uint8_t isRxEnabled;

	/**
	 * When true, driver is allowed to notify the stack that some data has been received
	 */
	uint8_t isRxInterruptEnabled;

	/**
	 * Synchronization on RX buffer
	 */
	SemaphoreHandle_t rxMutex;

	/**
	 * Synchronization on TX buffer
	 */
	SemaphoreHandle_t txMutex;

} LLCOMM_CDC;

/* Public functions ----------------------------------------------------------*/

void LLCOMM_USBH_CDC_ReceiveCallback(LLCOMM_CDC* llcomm);
void LLCOMM_USBH_CDC_TransmitCallback(LLCOMM_CDC* llcomm);

/* EOF -----------------------------------------------------------------------*/

#endif // _LLCOMM_CDC
