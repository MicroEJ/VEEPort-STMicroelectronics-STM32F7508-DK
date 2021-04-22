/*
 * C
 *
 * Copyright 2015-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLHAL_GPIO_IMPL
#define LLHAL_GPIO_IMPL

/**
 * @file
 * @brief MicroEJ HAL low level API
 * @author MicroEJ Developer Team
 * @version 1.1.2
 * @date 1 June 2017
 */

#include <intern/LLHAL_GPIO_impl.h>
#include <sni.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/*
 * Specify the pin mode.
 *
 * @see LLHAL_GPIO_IMPL_setPinMode(int32_t, int32_t, int32_t)
 */
typedef enum
{
	/*
	 * Digital input pin mode.
	 */
	LLHAL_GPIO_MODE_DIGITAL_INPUT = 0,

	/*
	 * Digital input with internal pull-up resistor pin mode.
	 */
	LLHAL_GPIO_MODE_DIGITAL_INPUT_PULLUP = 1,

	/*
	 * Digital output pin mode.
	 */
	LLHAL_GPIO_MODE_DIGITAL_OUTPUT = 2,

	/*
	 * Analog input pin mode.
	 */
	LLHAL_GPIO_MODE_ANALOG_INPUT = 3,

	/*
	 * Analog output pin mode.
	 */
	LLHAL_GPIO_MODE_ANALOG_OUTPUT = 4,

} HAL_GPIO_Mode;


/*
 * Specify the digital pin value.
 *
 * @see {@link GPIO#analogRead(int, int)}
 * @see {@link GPIO#analogWrite(int, int, int)}
 */
typedef enum {

	/*
	 * Digital pin low value.
	 */
	LLHAL_GPIO_DIGITAL_LOW = 0,

	/*
	 * Digital pin high value.
	 */
	LLHAL_GPIO_DIGITAL_HIGH = 1,

} HAL_GPIO_Digital_Value;

/*
 * Returned native value when no error
 */
#define HAL_GPIO_OK 0

/*
 * Returned native value when an error has occurred
 */
#define HAL_GPIO_NOK -1

/*
 * Configures the pin mode.
 *
 * @param port
 *            port identifier
 * @param pin
 *            pin number
 * @param mode
 *            pin mode
 * @return HAL_GPIO_OK (no error) or HAL_GPIO_NOK
 */
int32_t LLHAL_GPIO_IMPL_setPinMode(int32_t port, int32_t pin, int32_t mode);

/*
 * Gets the value of the digital pin.<br>
 * The returned value is undefined when the pin is not configured or
 * configured as analog pin.
 *
 * @param port
 *            port identifier
 * @param pin
 *            pin number
 *
 * @return digital pin value (LLHAL_GPIO_DIGITAL_LOW or LLHAL_GPIO_DIGITAL_HIGH),
 *         or HAL_GPIO_NOK when an error has occurred.
 */
int32_t LLHAL_GPIO_IMPL_getDigitalValue(int32_t port, int32_t pin) ;

/*
 * Sets a value on the digital pin.<br>
 *
 * @param port
 *            port identifier
 * @param pin
 *            pin number
 * @param value
 *            digital pin value (LLHAL_GPIO_DIGITAL_LOW or LLHAL_GPIO_DIGITAL_HIGH).
 * @return HAL_GPIO_OK when no error, HAL_GPIO_NOK when an error has occurred.
 */
int32_t LLHAL_GPIO_IMPL_setDigitalValue(int32_t port, int32_t pin, int32_t value) ;

/*
 * Gets the value of the analog pin (a value between <code>0</code> and the
 * targeted hardware ADC maximum value).<br>
 * The returned value is undefined when the pin is not configured or
 * configured as digital pin.
 *
 * @param port
 *            port identifier
 * @param pin
 *            pin number
 *
 * @return analog pin value, or HAL_GPIO_NOK when an error has occurred.
 */
int32_t LLHAL_GPIO_IMPL_getAnalogValue(int32_t port, int32_t pin);

/*
 * Sets a value on the analog pin. The value is a percentage which defines
 * the duty cycle a DAC or a PWM has to produce.<br>
 *
 * @param port
 *            port identifier
 * @param pin
 *            pin number
 * @param percentage
 *            percentage of analog pin value.
 * @return HAL_GPIO_OK when no error, HAL_GPIO_NOK when an error has occurred.
 */
int32_t LLHAL_GPIO_IMPL_setAnalogValue(int32_t port, int32_t pin, int32_t percentage) ;

#ifdef __cplusplus
	}
#endif
#endif
