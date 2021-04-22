/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef FAULT_HANDLERS_H
#define FAULT_HANDLERS_H

void HardFault_Handler(void);
void MemFault_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

#endif // FAULT_HANDLERS_H
