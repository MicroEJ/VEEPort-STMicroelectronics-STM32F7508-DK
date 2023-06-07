<?xml version="1.0"?>

<!--
	Front Panel

	Copyright 2020 MicroEJ Corp. All rights reserved.
	This library is provided in source code for use, modification and test, subject to license terms.
	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
-->

<frontpanel
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="https://developer.microej.com"
	xsi:schemaLocation="https://developer.microej.com .widget.xsd">

	<device name="STM32F7508DK" skin="Board_stm32f7508.jpg">
		<ej.fp.widget.Display x="71" y="41" width="480" height="272"/>
		<ej.fp.widget.Pointer x="71" y="41" width="480" height="272" touch="true"/>
		<ej.fp.widget.Button label="0" x="19" y="196" skin="But0.png" pushedSkin="But1.png" listenerClass="com.is2t.microej.fp.ButtonListener"/>
		<ej.fp.widget.LED label="0" x="30" y="258" ledOff="LedOff.png" ledOn="LedGreen.png"/>
	</device>
</frontpanel>
