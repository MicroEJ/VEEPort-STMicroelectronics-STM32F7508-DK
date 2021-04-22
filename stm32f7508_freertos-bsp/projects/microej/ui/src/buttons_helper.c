/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * The MicroUI framework is using an internal buffer to store the inputs events. The event
 * will not be added if the internal events buffer is full. In this case the input driver
 * has to adapt itself in order to not send a future invalid event. For instance, if a
 * PRESSED event is not sent, the input driver has not to send a REPEAT or RELEASE event.
 * So it may have a distinction between the real input state and the "software" input
 * state.
 *
 * When an event has not been managed because the system events queue is full:
 * - increase the size of the system events queue (see Memory page of the application launcher
 * - limit rate of events of your drivers (for example, calibrate buttons to send an event
 * at most every X ms)
 */

/* Includes ------------------------------------------------------------------*/

#include "LLUI_INPUT.h"
#include "microej.h"
#include "buttons_helper_configuration.h"
#include "event_generator.h"

/* Defines -------------------------------------------------------------------*/

// Number of pixels to generate a move after a press
#ifndef NUMBER_OF_BUTTONS
#error "Please set the define NUMBER_OF_BUTTONS in buttons_helper_configuration.h"
#endif

/* Global --------------------------------------------------------------------*/

static uint8_t buttons_pressed[NUMBER_OF_BUTTONS];

/* API -----------------------------------------------------------------------*/

void BUTTONS_HELPER_initialize(void)
{
	for(uint32_t i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		buttons_pressed[i] = MICROEJ_FALSE;
	}
}

void BUTTONS_HELPER_pressed(int32_t buttonId)
{
	// button is pressed

	if (buttons_pressed[buttonId] == MICROEJ_TRUE)
	{
		// button was pressed => repeat event  (don't care if event is lost)
		EVENT_GENERATOR_button_repeated(buttonId);
	}
	else
	{
		// button was released => press event
		if (EVENT_GENERATOR_button_pressed(buttonId) == LLUI_INPUT_OK)
		{
			// the event has been managed: we can store the new button state
			// button is pressed now
			buttons_pressed[buttonId] = MICROEJ_TRUE;
		}
		// else: event has been lost: stay in "release" state
	}
}

void BUTTONS_HELPER_repeated(int32_t buttonId)
{
	// manage this repeat event like a press event in order to check "software" button state
	BUTTONS_HELPER_pressed(buttonId);
}

void BUTTONS_HELPER_released(int32_t buttonId)
{
	// button is now released

	if (buttons_pressed[buttonId] == MICROEJ_TRUE)
	{
		// button was pressed => release event
		if (EVENT_GENERATOR_button_released(buttonId) == LLUI_INPUT_OK)
		{
			// the event has been managed: we can store the new button state
			// button is released now
			buttons_pressed[buttonId] = MICROEJ_FALSE;
		}
		// else: event has been lost: stay in "press" state
	}
	// else: already released
}
