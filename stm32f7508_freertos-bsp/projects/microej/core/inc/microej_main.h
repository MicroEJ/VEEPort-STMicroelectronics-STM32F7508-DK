/*
 * C
 *
 *  Copyright 2020 MicroEJ Corp. All rights reserved.
 *  This library is provided in source code for use, modification and test, subject to license terms.
 *  Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

/**
 * @file
 * @brief MicroEJ startup.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 24 April 2020
 */

#ifndef MICROEJ_MAIN_H_
#define MICROEJ_MAIN_H_

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Creates and starts a MicroEJ instance. This function returns when the MicroEJ execution ends.
 */
void microej_main(void);

#ifdef __cplusplus
    }
#endif

#endif /* MICROEJ_MAIN_H_ */
