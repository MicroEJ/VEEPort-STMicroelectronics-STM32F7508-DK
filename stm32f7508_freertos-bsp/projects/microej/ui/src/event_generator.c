/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * This file converts the input events in MicroUI Event Generator events.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLUI_INPUT.h"

// this h file is created by buildSystemMicroUI step
#include "microui_constants.h"

/* Defines -------------------------------------------------------------------*/

/*
 * Defines the three modes of button "USER"
 * When the button "USER" is pressed/released...
 *
 * - BUTTON_SEND_BUTTON_EVENT: a MicroUI Buttons event is send (press / repeat / release)
 * - BUTTON_SEND_COMMAND_EVENT: a MicroUI Command event is send (press / repeat only )
 * - BUTTON_SEND_HOME_EVENT: a MicroUI Home event is send (press only)
 *
 * The Buttons and Command events are sent to the MicroUI application.
 * The Home event is reserved to the internal framework.
 */
#define BUTTON_SEND_BUTTON_EVENT	(0)
#define BUTTON_SEND_COMMAND_EVENT	(1)
#define BUTTON_SEND_HOME_EVENT		(2)

/*
 * Choose the button mode selected a value in previous list
 */
#define BUTTON_SEND_EVENT BUTTON_SEND_COMMAND_EVENT

/*
 * Specify which MicroUI Command to send when the mode "BUTTON_SEND_COMMAND_EVENT" is selected
 */
#if BUTTON_SEND_EVENT == BUTTON_SEND_COMMAND_EVENT
#define BUTTON_COMMAND_EVENT	LLUI_INPUT_COMMAND_ESC
#endif

/*
 * Specify the value of MicroUI Home event to send when the mode "BUTTON_SEND_HOME_EVENT" is selected
 */
#if BUTTON_SEND_EVENT == BUTTON_SEND_HOME_EVENT
#define HOME (1)
#endif

/* Buttons -------------------------------------------------------------------*/

int32_t EVENT_GENERATOR_button_pressed(int32_t buttonId)
{
#if BUTTON_SEND_EVENT == BUTTON_SEND_BUTTON_EVENT
	// send a MicroUI Buttons "press" event
	return LLUI_INPUT_sendButtonPressedEvent(MICROUI_EVENTGEN_BUTTONS, buttonId);
#elif BUTTON_SEND_EVENT == BUTTON_SEND_COMMAND_EVENT
	// send a MicroUI Command event
	return LLUI_INPUT_sendCommandEvent(MICROUI_EVENTGEN_COMMANDS, BUTTON_COMMAND_EVENT);
#else	// BUTTON_SEND_EVENT == BUTTON_SEND_HOME_EVENT
	// send a MicroUI Home event
	return LLUI_INPUT_sendEvent(MICROUI_EVENTGEN_WADAPPS, HOME);
#endif
}

int32_t EVENT_GENERATOR_button_repeated(int32_t buttonId)
{
#if BUTTON_SEND_EVENT == BUTTON_SEND_BUTTON_EVENT
	// send a MicroUI Buttons "release" event
	return LLUI_INPUT_sendButtonRepeatedEvent(MICROUI_EVENTGEN_BUTTONS, buttonId);
#elif BUTTON_SEND_EVENT == BUTTON_SEND_COMMAND_EVENT
	// send a MicroUI Command event
	return LLUI_INPUT_sendCommandEvent(MICROUI_EVENTGEN_COMMANDS, BUTTON_COMMAND_EVENT);
#else	// BUTTON_SEND_EVENT == BUTTON_SEND_HOME_EVENT
	// do not send a Home event on release event
	return LLUI_INPUT_OK; // the event has been managed
#endif
}

int32_t EVENT_GENERATOR_button_released(int32_t buttonId)
{
#if BUTTON_SEND_EVENT == BUTTON_SEND_BUTTON_EVENT
	// send a MicroUI Buttons "release" event
	return LLUI_INPUT_sendButtonReleasedEvent(MICROUI_EVENTGEN_BUTTONS, buttonId);
#else
	// do not send a Command or Home event on release event
	return LLUI_INPUT_OK; // the event has been managed
#endif
}

/* Touch ---------------------------------------------------------------------*/

int32_t EVENT_GENERATOR_touch_pressed(int32_t x, int32_t y)
{
	return LLUI_INPUT_sendTouchPressedEvent(MICROUI_EVENTGEN_TOUCH, x, y);
}

int32_t EVENT_GENERATOR_touch_moved(int32_t x, int32_t y)
{
	return LLUI_INPUT_sendTouchMovedEvent(MICROUI_EVENTGEN_TOUCH, x, y);
}

int32_t EVENT_GENERATOR_touch_released(void)
{
	return LLUI_INPUT_sendTouchReleasedEvent(MICROUI_EVENTGEN_TOUCH);
}
