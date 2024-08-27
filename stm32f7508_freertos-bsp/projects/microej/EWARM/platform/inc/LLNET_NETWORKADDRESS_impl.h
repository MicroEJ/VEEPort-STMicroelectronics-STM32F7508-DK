/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_NETWORKADDRESS_IMPL
#define LLNET_NETWORKADDRESS_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_NETWORKADDRESS_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Gets the host address representing any local address.
 * <p>When this method returns, the <code>anyLocalAddr</code> buffer is filled with the host address.
 *
 * @param[out]	anyLocalAddr 	The buffer into which the host address will be stored.
 * @param[in]	length			The buffer length.
 *
 * @return the host address size in bytes.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning anyLocalAddr must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKADDRESS_IMPL_lookupInaddrAny(int8_t* anyLocalAddr, int32_t length);

/**
 * @brief Gets the local host name.
 * <p>When this method returns, the <code>localHostname</code> buffer is filled with the local host name.
 *
 * @param[out]	localHostname		The buffer into which the local host name will be stored.
 * @param[in]	length				The buffer length.
 *
 * @return the local host name length.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning localHostname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKADDRESS_IMPL_getLocalHostnameNative(uint8_t* localHostname, int32_t length);

/**
 * @brief Gets the loopback address.
 * <p>When this method returns, the <code>loopback</code> buffer is filled with the loopback address.
 *
 * @param[out]	loopback	The buffer into which the loopback address will be stored.
 * @param[in]	length 		The buffer length.
 *
 * @return the loopback address size in bytes.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning loopback must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKADDRESS_IMPL_loopbackAddress(int8_t* loopback, int32_t length);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_NETWORKADDRESS_IMPL
