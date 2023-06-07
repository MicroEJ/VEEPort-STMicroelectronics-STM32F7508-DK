/**
 * Copyright 2020-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.is2t.microej.fp;

import ej.fp.widget.Button;
import ej.microui.event.EventButton;
import ej.microui.event.EventCommand;
import ej.microui.event.EventGeneric;

public class ButtonListener implements ej.fp.widget.Button.ButtonListener {

	/**
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
	private static final int BUTTON_SEND_BUTTON_EVENT	= 0;
	private static final int BUTTON_SEND_COMMAND_EVENT = 1;
	private static final int BUTTON_SEND_HOME_EVENT = 2;

	/**
	 * Choose the button mode selected a value in previous list
	 */
	private static final int BUTTON_SEND_EVENT = BUTTON_SEND_HOME_EVENT;

	/**
	 * Specify which MicroUI Command to send when the mode "BUTTON_SEND_COMMAND_EVENT" is selected
	 * useful only when BUTTON_SEND_EVENT == BUTTON_SEND_COMMAND_EVENT
	 */
	private static final int BUTTON_COMMAND_EVENT = EventCommand.ESC;

	/**
	 * Specify the value of MicroUI Home event to send when the mode "BUTTON_SEND_HOME_EVENT" is selected
	 * useful only when BUTTON_SEND_EVENT == BUTTON_SEND_HOME_EVENT
	 */
	private static final int HOME = 1;

	@Override
	public void press(Button button) {
		switch (BUTTON_SEND_EVENT) {
		default:
		case BUTTON_SEND_BUTTON_EVENT:
			EventButton.sendPressedEvent(button.getID());
			break;
		case BUTTON_SEND_COMMAND_EVENT:
			EventCommand.sendEvent(BUTTON_COMMAND_EVENT);
			break;
		case BUTTON_SEND_HOME_EVENT:
			EventGeneric.sendEvent("WADAPPS", HOME);
			break;
		}
	}

	@Override
	public void release(Button button) {
		switch (BUTTON_SEND_EVENT) {
		default:
		case BUTTON_SEND_BUTTON_EVENT:
			EventButton.sendPressedEvent(button.getID());
			break;
		case BUTTON_SEND_COMMAND_EVENT:
		case BUTTON_SEND_HOME_EVENT:
			// do not send a Command or Home event on release event
			break;
		}
	}
}
