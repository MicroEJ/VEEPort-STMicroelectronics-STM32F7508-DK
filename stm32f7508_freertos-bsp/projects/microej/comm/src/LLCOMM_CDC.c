/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLCOMM_CDC.h"
#define   LLCOMM_BUFFERED_CONNECTION_IMPL LLCOMM_CDC
#include "LLCOMM_BUFFERED_CONNECTION_impl.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "interrupts.h"
#include "usbh_core.h"
#include "usbh_cdc.h"

/* Defines -------------------------------------------------------------------*/


/* Enumerations --------------------------------------------------------------*/

typedef enum
{
	IDVENDOR = 0,
	IDPRODUCT = 1,
	IMANUFACTURER = 2,
	IPRODUCT = 3,
	ISERIALNUMBER = 4,
} cdc_property_id;

/* Fields --------------------------------------------------------------------*/

static CDC_LineCodingTypeDef linecodin;

/* Private functions ---------------------------------------------------------*/

/**
 * Request to receive bytes from the driver for the given connection.
 */
static void requestReceive(LLCOMM_CDC* p_cdc_conn)
{
	USBH_CDC_Receive(p_cdc_conn->hUSBHost, p_cdc_conn->rxBuffer, LLCOMM_CDC_RX_BUFFER_SIZE);
}

/**
 * Request to send bytes to the driver for the given connection.
 */
static void requestFlush(LLCOMM_CDC* p_cdc_conn)
{
	int nbBytes = p_cdc_conn->txBufferPtr - (uint8_t*) p_cdc_conn->txBuffer;
	p_cdc_conn->txInProgress = 1;
	p_cdc_conn->txLastBuffLength = nbBytes;
	USBH_CDC_Transmit(p_cdc_conn->hUSBHost, p_cdc_conn->txBuffer, p_cdc_conn->txLastBuffLength);
}

static uint8_t syncEnter(xSemaphoreHandle mutex)
{
	if (xSemaphoreGetMutexHolder(mutex) != xTaskGetCurrentTaskHandle())
	{
		xSemaphoreTake(mutex, portMAX_DELAY);
		return MICROEJ_TRUE;	// have to release the mutex
	}
	else
	{
		// the current task already holds the mutex
		// -> must not take it again to prevent dead lock
		return MICROEJ_FALSE;	// have not to release the mutex
	}
}

static void syncExit(xSemaphoreHandle mutex, uint8_t release)
{
	if (release == MICROEJ_TRUE)
	{
		xSemaphoreGive(mutex);
	}
}

static uint8_t syncTxEnter(LLCOMM_CDC* p_cdc_conn)
{
	return syncEnter(p_cdc_conn->txMutex);
}

static void syncTxExit(LLCOMM_CDC* p_cdc_conn, uint8_t release)
{
	syncExit(p_cdc_conn->txMutex, release);
}

static uint8_t syncRxEnter(LLCOMM_CDC* p_cdc_conn)
{
	return syncEnter(p_cdc_conn->rxMutex);
}

static void syncRxExit(LLCOMM_CDC* p_cdc_conn, uint8_t release)
{
	syncExit(p_cdc_conn->rxMutex, release);
}

static void resetRXBuffer(LLCOMM_CDC* p_cdc_conn)
{
	p_cdc_conn->rxPendingBufferPtrStart = p_cdc_conn->rxBuffer;
	p_cdc_conn->rxPendingBufferPtrEnd = p_cdc_conn->rxBuffer;
}

/**
 * Send data received in RX buffer to the connection until the FIFO is not full.
 * When all received data has been sent to the FIFO, request to receive new bytes.
 */
static void dataReceived(LLCOMM_CDC* p_cdc_conn)
{
	while (p_cdc_conn->rxPendingBufferPtrStart < p_cdc_conn->rxPendingBufferPtrEnd)
	{
		int32_t result = LLCOMM_BUFFERED_CONNECTION_dataReceived((LLCOMM_BUFFERED_CONNECTION*) p_cdc_conn, *p_cdc_conn->rxPendingBufferPtrStart);
		if (result == LLCOMM_BUFFERED_CONNECTION_FIFO_FULL)
		{
			return; // stop
		}
		++p_cdc_conn->rxPendingBufferPtrStart;
	}

	// Here, all bytes have been sent to the FIFO: reset RX buffer and request to receive new bytes
	resetRXBuffer(p_cdc_conn);
	requestReceive(p_cdc_conn);
}

static void LLCOMM_USBH_Get_StringDesc(LLCOMM_BUFFERED_CONNECTION* env, uint8_t string_id)
{
	if (string_id != 0)
	{
		USBH_StatusTypeDef status;
		do
		{
			status = USBH_Get_StringDesc(
					(((LLCOMM_CDC*)env)->hUSBHost),
					string_id,
					(((LLCOMM_CDC*)env)->hUSBHost)->device.Data ,
					0xff);
		} while(status == USBH_BUSY);
	}
}

/* Callback ------------------------------------------------------------------*/

void LLCOMM_USBH_CDC_ReceiveCallback(LLCOMM_CDC* p_cdc_conn)
{
	if (p_cdc_conn->status != CDC_STATUS_CONNECTED)
	{
		// Device disconnected. The connection will be removed.
		// Silently skip this callback.
		return;
	}

	// retrieve available number of incoming data
	int32_t xfer_len = USBH_CDC_GetLastReceivedDataSize(p_cdc_conn->hUSBHost);

	uint8_t release_mutex = syncRxEnter(p_cdc_conn);
	{
		if (p_cdc_conn->isRxEnabled)
		{
			// Here, some application bytes have been received

			// Update ptr start and end
			p_cdc_conn->rxPendingBufferPtrStart = p_cdc_conn->rxBuffer;
			p_cdc_conn->rxPendingBufferPtrEnd = p_cdc_conn->rxBuffer + xfer_len;

			if (p_cdc_conn->isRxInterruptEnabled)
			{
				// Allowed to directly send bytes to the connection
				dataReceived(p_cdc_conn);
			}
			else
			{
				// Not allowed to send bytes to the connection. Store the number of bytes pending.
				// Bytes will be sent on next enableRXDeviceInterrupt call.
			}
		}
		else
		{
			// Connection closed: skip this callback.
		}
	}
	syncRxExit(p_cdc_conn, release_mutex);
}

void LLCOMM_USBH_CDC_TransmitCallback(LLCOMM_CDC* p_cdc_conn)
{
	if (p_cdc_conn->status != CDC_STATUS_CONNECTED)
	{
		// Device disconnected. The connection will be removed.
		// Silently skip this callback.
		return;
	}

	// Transfer complete
	uint8_t release_mutex = syncTxEnter(p_cdc_conn);
	{
		p_cdc_conn->txBufferPtr = (uint8_t*) p_cdc_conn->txBuffer;
		p_cdc_conn->txInProgress = 0;
		p_cdc_conn->txLastBuffLength = 0;
		if (p_cdc_conn->isTxEnabled)
		{
			if (p_cdc_conn->isTxInterruptEnabled)
			{
				// Allowed to notify transmit buffer ready to the connection
				LLCOMM_BUFFERED_CONNECTION_transmitBufferReady((LLCOMM_BUFFERED_CONNECTION*) p_cdc_conn);
			}
			else
			{
				// Not allowed to notify transmit buffer ready to the connection. Store the pending flag.
				p_cdc_conn->pendingTransmitReady = 1;
			}
		}
	}
	syncTxExit(p_cdc_conn, release_mutex);
	requestReceive(p_cdc_conn);
}
/* API -----------------------------------------------------------------------*/

void LLCOMM_BUFFERED_CONNECTION_IMPL_initialize(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = ((LLCOMM_CDC*) env);
	p_cdc_conn->txBufferPtr = (uint8_t*) p_cdc_conn->txBuffer;
	p_cdc_conn->isTxEnabled = 0;
	p_cdc_conn->isTxInterruptEnabled = 0;
	p_cdc_conn->pendingTransmitReady = 0;
	p_cdc_conn->isRxEnabled = 0;
	p_cdc_conn->isRxInterruptEnabled = 0;
	resetRXBuffer(p_cdc_conn);
	p_cdc_conn->txMutex = xSemaphoreCreateMutex();
	p_cdc_conn->rxMutex = xSemaphoreCreateMutex();
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getPlatformId(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_BUFFERED_CONNECTION_NO_PLATFORM_ID;
}

void* LLCOMM_BUFFERED_CONNECTION_IMPL_getName(LLCOMM_BUFFERED_CONNECTION* env)
{
	sprintf((char*) (((LLCOMM_CDC*)env)->hUSBHost)->device.Data, "%s%d", "CDC", (((LLCOMM_CDC*)env)->id));
	return (void*)(((LLCOMM_CDC*)env)->hUSBHost)->device.Data;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_configureDevice(LLCOMM_BUFFERED_CONNECTION* env, int32_t baudrate, int32_t bitsperchar, int32_t stopbits, int32_t parity)
{
	LLCOMM_CDC* p_cdc_conn = ((LLCOMM_CDC*) env);

	// get number of bits
	switch (bitsperchar)
	{
	case LLCOMM_BUFFERED_CONNECTION_DATABITS_5:
		linecodin.b.bDataBits = 5;
		break;

	case LLCOMM_BUFFERED_CONNECTION_DATABITS_6:
		linecodin.b.bDataBits = 6;
		break;

	case LLCOMM_BUFFERED_CONNECTION_DATABITS_7:
		linecodin.b.bDataBits = 7;
		break;

	case LLCOMM_BUFFERED_CONNECTION_DATABITS_8:
		linecodin.b.bDataBits = 8;
		break;

	default:
	case LLCOMM_BUFFERED_CONNECTION_DATABITS_9:
		// not available
		return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_BITSPERCHAR;
	}

	// get number of stop bits
	switch (stopbits)
	{
	case LLCOMM_BUFFERED_CONNECTION_STOPBITS_1:
		linecodin.b.bCharFormat = 0 ;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_STOPBITS_2:
		linecodin.b.bCharFormat = 2 ;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_STOPBITS_1_5:
		linecodin.b.bCharFormat = 1 ;
		break ;

	default:
		return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_STOPBITS;
	}

	// get parity
	switch (parity)
	{
	case LLCOMM_BUFFERED_CONNECTION_PARITY_EVEN:
		linecodin.b.bParityType = 2 ;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_PARITY_NO:
		linecodin.b.bParityType = 0 ;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_PARITY_ODD:
		linecodin.b.bParityType = 1 ;
		break ;

	default:
		return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_PARITY;
	}

	linecodin.b.dwDTERate = baudrate ;
	while (USBH_CDC_SetLineCoding(p_cdc_conn->hUSBHost, &linecodin) != USBH_OK);

	return LLCOMM_BUFFERED_CONNECTION_SUCCESS;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableRX(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	p_cdc_conn->isRxEnabled = 1;
	requestReceive(p_cdc_conn);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableRX(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncRxEnter(p_cdc_conn);
	{
		p_cdc_conn->isRxEnabled = 0;
		resetRXBuffer(p_cdc_conn);
	}
	syncRxExit(p_cdc_conn, release_mutex);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableRXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncRxEnter(p_cdc_conn);
	{
		p_cdc_conn->isRxInterruptEnabled = 1;
		if (p_cdc_conn->rxPendingBufferPtrEnd > p_cdc_conn->rxBuffer)
		{
			// pending bytes in the RX buffer that have to be sent to the connection
			dataReceived(p_cdc_conn);
		}
	}
	syncRxExit(p_cdc_conn, release_mutex);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableRXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncRxEnter(p_cdc_conn);
	{
		((LLCOMM_CDC*) env)->isRxInterruptEnabled = 0;
	}
	syncRxExit(p_cdc_conn, release_mutex);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableTX(LLCOMM_BUFFERED_CONNECTION* env)
{
	((LLCOMM_CDC*) env)->isTxEnabled = 1;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableTX(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncTxEnter(p_cdc_conn);
	{
		p_cdc_conn->isTxEnabled = 0;
		p_cdc_conn->pendingTransmitReady = 0; // reset pending flag
		p_cdc_conn->txInProgress = 0;
	}
	syncTxExit(p_cdc_conn, release_mutex);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableTXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncTxEnter(p_cdc_conn);
	{
		if (p_cdc_conn->txBufferPtr > (uint8_t*) p_cdc_conn->txBuffer)
		{
			if (!p_cdc_conn->txInProgress)
			{
				// some bytes to send
				requestFlush(p_cdc_conn);
			}
		}

		if (p_cdc_conn->pendingTransmitReady)
		{
			// pending transmit buffer ready flag has to be sent to the connection
			p_cdc_conn->pendingTransmitReady = 0;
			LLCOMM_BUFFERED_CONNECTION_transmitBufferReady((LLCOMM_BUFFERED_CONNECTION*) p_cdc_conn);
		}
		p_cdc_conn->isTxInterruptEnabled = 1;
	}
	syncTxExit(p_cdc_conn, release_mutex);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableTXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncTxEnter(p_cdc_conn);
	{
		((LLCOMM_CDC*) env)->isTxInterruptEnabled = 0;
	}
	syncTxExit(p_cdc_conn, release_mutex);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getRXBufferStartAddress(LLCOMM_BUFFERED_CONNECTION* env)
{
	return (int32_t)(((LLCOMM_CDC*)env)->rxFIFOBuffer);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getRXBufferLength(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_CDC_RX_FIFO_SIZE;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getTXBufferStartAddress(LLCOMM_BUFFERED_CONNECTION* env)
{
	return (int32_t)(((LLCOMM_CDC*)env)->txFIFOBuffer);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getTXBufferLength(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_CDC_TX_FIFO_SIZE;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_useTXHardwareFIFO(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_BUFFERED_CONNECTION_SUCCESS;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_txFifoFull(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	return ((p_cdc_conn->txBufferPtr) == ((uint8_t*) p_cdc_conn->txBuffer + LLCOMM_CDC_TX_BUFFER_SIZE)) ? LLCOMM_BUFFERED_CONNECTION_HARDWARE_FIFO_FULL : LLCOMM_BUFFERED_CONNECTION_HARDWARE_FIFO_NOT_FULL;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_sendData(LLCOMM_BUFFERED_CONNECTION* env, int32_t data)
{
	// push data in tx buffer
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	*(p_cdc_conn->txBufferPtr) = data;
	++(p_cdc_conn->txBufferPtr);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_isTransmissionComplete(LLCOMM_BUFFERED_CONNECTION* env)
{
	int32_t errorCode;
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	uint8_t release_mutex = syncTxEnter(p_cdc_conn);
	{
		if (p_cdc_conn->txInProgress)
		{
			errorCode = LLCOMM_BUFFERED_CONNECTION_TRANSMISSION_TIMEOUT;
		}
		else
		{
			if (p_cdc_conn->txBufferPtr > (uint8_t*) p_cdc_conn->txBuffer)
			{
				// some bytes to flush
				requestFlush(p_cdc_conn);
				errorCode = LLCOMM_BUFFERED_CONNECTION_TRANSMISSION_TIMEOUT;
			}
			else
			{
				errorCode = LLCOMM_BUFFERED_CONNECTION_TRANSMISSION_COMPLETE;
			}
		}
	}
	syncTxExit(p_cdc_conn, release_mutex);
	return errorCode;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getNbProperties(LLCOMM_BUFFERED_CONNECTION* env)
{
	return 5;
}

void* LLCOMM_BUFFERED_CONNECTION_IMPL_getProperty(LLCOMM_BUFFERED_CONNECTION* env, void* propertyName)
{
	// default returned string: an empty string
	(((LLCOMM_CDC*)env)->hUSBHost)->device.Data[0] = '\0';

	if (strcmp(propertyName, "usb/idVendor") == 0)
	{
		sprintf((char*) (((LLCOMM_CDC*)env)->hUSBHost)->device.Data, "0x%04x", (((LLCOMM_CDC*)env)->hUSBHost)->device.DevDesc.idVendor);
	}
	else if (strcmp(propertyName, "usb/idProduct") == 0)
	{
		sprintf((char*) (((LLCOMM_CDC*)env)->hUSBHost)->device.Data, "0x%04x", (((LLCOMM_CDC*)env)->hUSBHost)->device.DevDesc.idProduct);
	}
	else if (strcmp(propertyName, "usb/iManufacturer") == 0)
	{
		LLCOMM_USBH_Get_StringDesc(env, (((LLCOMM_CDC*)env)->hUSBHost)->device.DevDesc.iManufacturer);
	}
	else if (strcmp(propertyName, "usb/iProduct") == 0)
	{
		LLCOMM_USBH_Get_StringDesc(env, (((LLCOMM_CDC*)env)->hUSBHost)->device.DevDesc.iProduct);
	}
	else if (strcmp(propertyName, "usb/iSerialNumber") == 0)
	{
		LLCOMM_USBH_Get_StringDesc(env, (((LLCOMM_CDC*)env)->hUSBHost)->device.DevDesc.iSerialNumber);
	}

	return (void*)(((LLCOMM_CDC*)env)->hUSBHost)->device.Data;
}

void* LLCOMM_BUFFERED_CONNECTION_IMPL_getPropertyName(LLCOMM_BUFFERED_CONNECTION* env, int32_t propertyId)
{
	switch (propertyId)
	{
	default:
	case IDVENDOR:
		return "usb/idVendor";
	case IDPRODUCT:
		return "usb/idProduct";
	case IMANUFACTURER:
		return "usb/iManufacturer";
	case IPRODUCT:
		return "usb/iProduct";
	case ISERIALNUMBER:
		return "usb/iSerialNumber";
	}
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_released(LLCOMM_BUFFERED_CONNECTION* env)
{
	LLCOMM_CDC* p_cdc_conn = (LLCOMM_CDC*) env;
	p_cdc_conn->status = CDC_STATUS_IDLE;
}

