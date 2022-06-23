/* 
 * Copyright 2012-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLUI_INPUT
#define _LLUI_INPUT 
#ifdef __cplusplus
	extern "C" {
#endif

/*
 * @brief Provides some API to generate some MicroUI input events from the adaptation
 * layer.
 */

#include "sni.h"

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

/**
 * The "escape" command constant.
 * The value <code>0x0000</code> is assigned to <code>COMMAND_ESC</code>.
 */
#define LLUI_INPUT_COMMAND_ESC (0)

/**
 * The "back" command constant.
 * The value <code>0x0001</code> is assigned to <code>COMMAND_BACK</code>.
 */
#define LLUI_INPUT_COMMAND_BACK (1)

/**
 * The "up" command constant.
 * The value <code>0x0002</code> is assigned to <code>COMMAND_UP</code>.
 */
#define LLUI_INPUT_COMMAND_UP (2)

/**
 * The "down" command constant.
 * The value <code>0x0003</code> is assigned to <code>COMMAND_DOWN</code>.
 */
#define LLUI_INPUT_COMMAND_DOWN (3)

/**
 * The "left" command constant.
 * The value <code>0x0004</code> is assigned to <code>COMMAND_LEFT</code>.
 */
#define LLUI_INPUT_COMMAND_LEFT (4)

/**
 * The "right" command constant.
 * The value <code>0x0005</code> is assigned to <code>COMMAND_RIGHT</code>.
 */
#define LLUI_INPUT_COMMAND_RIGHT (5)

/**
 * The "select" command constant.
 * The value <code>0x0006</code> is assigned to <code>COMMAND_SELECT</code>.
 */
#define LLUI_INPUT_COMMAND_SELECT (6)

/**
 * The "cancel" command constant.
 * The value <code>0x0007</code> is assigned to <code>COMMAND_CANCEL</code>.
 */
#define LLUI_INPUT_COMMAND_CANCEL (7)

/**
 * The "help" command constant.
 * The value <code>0x0008</code> is assigned to <code>COMMAND_HELP</code>.
 */
#define LLUI_INPUT_COMMAND_HELP (8)

/**
 * The "menu" command constant.
 * The value <code>0x0009</code> is assigned to <code>COMMAND_MENU</code>.
 */
#define LLUI_INPUT_COMMAND_MENU (9)

/**
 * The "exit" command constant.
 * The value <code>0x000A</code> is assigned to <code>COMMAND_EXIT</code>.
 */
#define LLUI_INPUT_COMMAND_EXIT (10)

/**
 * The "start" command constant.
 * The value <code>0x000B</code> is assigned to <code>COMMAND_START</code>.
 */
#define LLUI_INPUT_COMMAND_START (11)

/**
 * The "stop" command constant.
 * The value <code>0x000C</code> is assigned to <code>COMMAND_STOP</code>.
 */
#define LLUI_INPUT_COMMAND_STOP (12)

/**
 * The "pause" command constant.
 * The value <code>0x000D</code> is assigned to <code>COMMAND_PAUSE</code>.
 */
#define LLUI_INPUT_COMMAND_PAUSE (13)

/**
 * The "resume" command constant.
 * The value <code>0x000E</code> is assigned to <code>COMMAND_RESUME</code>.
 */
#define LLUI_INPUT_COMMAND_RESUME (14)

/**
 * The "copy" command constant.
 * The value <code>0x000F</code> is assigned to <code>COMMAND_COPY</code>.
 */
#define LLUI_INPUT_COMMAND_COPY (15)

/**
 * The "cut" command constant.
 * The value <code>0x0010</code> is assigned to <code>COMMAND_CUT</code>.
 */
#define LLUI_INPUT_COMMAND_CUT (16)

/**
 * The "paste" command constant.
 * The value <code>0x0011</code> is assigned to <code>COMMAND_PASTE</code>.
 */
#define LLUI_INPUT_COMMAND_PASTE (17)

/**
 * The "clockwise" command constant.
 * The value <code>0x0012</code> is assigned to <code>COMMAND_CLOCKWISE</code>.
 */
#define LLUI_INPUT_COMMAND_CLOCKWISE (18)

/**
 * The "anti-clockwise" command constant.
 * The value <code>0x0013</code> is assigned to <code>COMMAND_ANTICLOCKWISE</code>.
 */
#define LLUI_INPUT_COMMAND_ANTICLOCKWISE (19)

/**
 * The "previous" command constant.
 * The value <code>0x0014</code> is assigned to <code>COMMAND_PREVIOUS</code>.
 */
#define LLUI_INPUT_COMMAND_PREVIOUS (20)

/**
 * The "next" command constant.
 * The value <code>0x0015</code> is assigned to <code>COMMAND_NEXT</code>.
 */
#define LLUI_INPUT_COMMAND_NEXT (21)

/**
 * The "display" command constant.
 * The value <code>0x0016</code> is assigned to <code>COMMAND_DISPLAY</code>.
 */
#define LLUI_INPUT_COMMAND_DISPLAY (22)

// --------------------------------------------------------------------------------
// Typedef and Structure
// --------------------------------------------------------------------------------

/*
 * @brief LLUI_INPUT error and status codes.
 *
 * These values are used by several LLUI_INPUT.h and LLUI_INPUT_IMPL.h functions. See
 * each function comment to know which are the available codes.
 */
typedef enum
{
	/*
	 * @brief Value when everything has been correctly executed.
	 */
	LLUI_INPUT_OK = 0,

	/*
	 * @brief Value when an error has occurred or when a function is not able to
	 * implement the expected behavior.
	 */
	LLUI_INPUT_NOK = -1

} LLUI_INPUT_Status;

/*
 * @brief LLUI_INPUT pointer type of coordinates.
 *
 * These values are used to specify the type of pointer coordinates.
 */
typedef enum
{
	/*
	 * @brief Specifies the pointer coordinates are absolute.
	 */
	LLUI_INPUT_ABSOLUTE = 0,

	/*
	 * @brief Specifies the pointer coordinates are relative to the previous pointer
	 * position.
	 */
	LLUI_INPUT_RELATIVE = -1

} LLUI_INPUT_Pointer;

// --------------------------------------------------------------------------------
// Functions provided by the input engine
// --------------------------------------------------------------------------------

/**
 * @brief Add an event to the generic event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>eventGeneratorID</code>) is not a MicroUI <code>EventGenerator</code> 
 * event generator.
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] eventGeneratorID the MicroUI <code>EventGenerator</code> event generator ID.
 * @param[in] event the 32-bit event.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendEvent(jint eventGeneratorID, jint event);

/**
 * @brief Add several events to the generic event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>eventGeneratorID</code>) is not a MicroUI <code>EventGenerator</code> 
 * event generator.
 * The events will not be added if the internal events buffer is or becomes full. If the
 * return value is not 1, no events have been added.
 *
 * @param[in] eventGeneratorID the MicroUI <code>EventGenerator</code> event generator ID.
 * @param[in] eventsArray the address of the 32-bit events array.
 * @param[in] length the number of events (maximum 256 events).
 *
 * @return LLUI_INPUT_OK if all events have been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendEvents(jint eventGeneratorID, jint* eventsArray, jint length);

/**
 * @brief Add a command event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>commandsEventGeneratorID</code>) is not a MicroUI <code>Command</code> 
 * event generator (tag <code>&lt;command&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] commandsEventGeneratorID the MicroUI <code>Command</code> event generator ID.
 * @param[in] command the command to send, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendCommandEvent(jint commandsEventGeneratorID, jint command);

/**
 * @brief Add a PRESSED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>buttonsEventGeneratorID</code>) is not a MicroUI <code>Buttons</code> 
 * event generator (tag <code>&lt;buttons&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full. In this case the
 * caller should not send next RELEASED event (and potential REPEATED event(s)) to 
 * prevent unexpected behavior.
 *
 * @param[in] buttonsEventGeneratorID the MicroUI <code>Buttons</code> event generator ID.
 * @param[in] buttonId the button ID, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendButtonPressedEvent(jint buttonsEventGeneratorID, jint buttonId);

/**
 * @brief Add a RELEASED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>buttonsEventGeneratorID</code>) is not a MicroUI <code>Buttons</code> 
 * event generator (tag <code>&lt;buttons&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full. In this case the
 * caller should not send next PRESSED event to prevent unexpected behavior.
 *
 * @param[in] buttonsEventGeneratorID the MicroUI <code>Buttons</code> event generator ID.
 * @param[in] buttonId the button ID, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendButtonReleasedEvent(jint buttonsEventGeneratorID, jint buttonId);

/**
 * @brief Add a REPEATED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>buttonsEventGeneratorID</code>) is not a MicroUI <code>Buttons</code> 
 * event generator (tag <code>&lt;buttons&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] buttonsEventGeneratorID the MicroUI <code>Buttons</code> event generator ID.
 * @param[in] buttonId the button ID, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendButtonRepeatedEvent(jint buttonsEventGeneratorID, jint buttonId);

/**
 * @brief Add a LONG event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>buttonsEventGeneratorID</code>) is not a MicroUI <code>Buttons</code> 
 * event generator (tag <code>&lt;buttons&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] buttonsEventGeneratorID the MicroUI <code>Buttons</code> event generator ID.
 * @param[in] buttonId the button ID, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendButtonLongEvent(jint buttonsEventGeneratorID, jint buttonId);

/**
 * @brief Add a PRESSED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>pointerEventGeneratorID</code>) is not a MicroUI <code>Pointer</code> 
 * event generator (tag <code>&lt;pointer&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full. In this case the
 * caller should not send next RELEASED event (and potential MOVE event(s)) to 
 * prevent unexpected behavior.
 *
 * @param[in] pointerEventGeneratorID the MicroUI <code>Pointer</code> event generator ID.
 * @param[in] buttonId the button ID, between 0 and 255.
 * @param[in] x the pointer X coordinate.
 * @param[in] y the pointer Y coordinate.
 * @param[in] type {@link LLUI_INPUT_ABSOLUTE} when (x,y) coordinates are absolute,{@link LLUI_INPUT_RELATIVE} when
 * there are relative to the previous pointer position.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendPointerPressedEvent(jint pointerEventGeneratorID, jint buttonID, jint x, jint y, LLUI_INPUT_Pointer type);

/**
 * @brief Add a RELEASED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>pointerEventGeneratorID</code>) is not a MicroUI <code>Pointer</code> 
 * event generator (tag <code>&lt;pointer&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full. In this case the
 * caller should not send next PRESSED event to prevent unexpected behavior.
 *
 * @param[in] pointerEventGeneratorID the MicroUI <code>Pointer</code> event generator ID.
 * @param[in] buttonId the button ID, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendPointerReleasedEvent(jint pointerEventGeneratorID, jint buttonID);

/**
 * @brief Add a MOVE event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>pointerEventGeneratorID</code>) is not a MicroUI <code>Pointer</code> 
 * event generator (tag <code>&lt;pointer&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] pointerEventGeneratorID the MicroUI <code>Pointer</code> event generator ID.
 * @param[in] x the pointer X coordinate.
 * @param[in] y the pointer Y coordinate.
 * @param[in] type {@link LLUI_INPUT_ABSOLUTE} when (x,y) coordinates are absolute,{@link LLUI_INPUT_RELATIVE} when
 * there are relative to the previous pointer position.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendPointerMovedEvent(jint pointerEventGeneratorID, jint x, jint y, LLUI_INPUT_Pointer type);

/**
 * @brief Add a PRESSED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>touchEventGeneratorID</code>) is not a MicroUI <code>Touch</code> 
 * event generator (tag <code>&lt;touch&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full. In this case the
 * caller should not send next RELEASED event (and potential MOVE event(s)) to 
 * prevent unexpected behavior.
 *
 * @param[in] touchEventGeneratorID the MicroUI <code>Touch</code> event generator ID.
 * @param[in] x the pointer X coordinate.
 * @param[in] y the pointer Y coordinate.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendTouchPressedEvent(jint touchEventGeneratorID, jint x, jint y);

/**
 * @brief Add a RELEASED event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>touchEventGeneratorID</code>) is not a MicroUI <code>Pointer</code> 
 * event generator (tag <code>&lt;touch&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full. In this case the
 * caller should not send next PRESSED event to prevent unexpected behavior.
 *
 * @param[in] touchEventGeneratorID the MicroUI <code>Touch</code> event generator ID.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendTouchReleasedEvent(jint touchEventGeneratorID);

/**
 * @brief Add a MOVE event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>touchEventGeneratorID</code>) is not a MicroUI <code>Touch</code> 
 * event generator (tag <code>&lt;touch&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] touchEventGeneratorID the MicroUI <code>Pointer</code> event generator ID.
 * @param[in] x the pointer X coordinate.
 * @param[in] y the pointer Y coordinate.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendTouchMovedEvent(jint touchEventGeneratorID, jint x, jint y);

/**
 * @brief Add a STATE event to the event generator.
 *
 * The effect of this method is undefined when the given event generator (specified by
 * <code>statesEventGeneratorID</code>) is not a MicroUI <code>States</code> 
 * event generator (tag <code>&lt;states&gt;</code> in microui xml file).
 * The event will not be added if the internal events buffer is full.
 *
 * @param[in] statesEventGeneratorID the MicroUI <code>States</code> event generator ID.
 * @param[in] stateID the state machine identifier, between 0 and 255.
 * @param[in] stateValue the new state if the state machine, between 0 and 255.
 *
 * @return LLUI_INPUT_OK if the event has been added, LLUI_INPUT_NOK otherwise.
 */
LLUI_INPUT_Status LLUI_INPUT_sendStateEvent(jint statesEventGeneratorID, jint stateID, jint stateValue);

/**
 * @brief Returns the maximum usage of the native events buffer. This can be useful for
 * tuning the size of the buffer.
 *
 * @return the maximum usage of the native events buffer.
 */
jint LLUI_INPUT_getMaxEventsBufferUsage(void);


/**
 * @brief Dumps the MicroUI FIFO.
 *
 * MicroUI FIFO contains some events and some optional events' data. MicroUI FIFO is not able to
 * interpret these events and data. The interpretation is delegated to LLUI_INPUT_IMPL_log_dump().
 * The default logger does nothing and by consequence a call to LLUI_INPUT_dump() has no
 * effect.
 *
 * Events (and data) are dumped from older to newer. They are dumped in two categories: the events
 * (and data) that are already executed (past) and the events (and data) that are not executed yet
 * (future).
 *
 * This function must only be called from the MicroJvm virtual machine thread context and only from a native function or callback.
 * Calling this function from another context may lead to undefined behavior and should be done only for debug purpose.
 */
void LLUI_INPUT_dump(void);

#ifdef __cplusplus
	}
#endif
#endif
