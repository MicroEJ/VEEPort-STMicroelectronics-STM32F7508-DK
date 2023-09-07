/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_NETWORKADDRESS_IMPL
#define LLNET_NETWORKADDRESS_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_NETWORKADDRESS_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Gets the host address representing any local address.
 * <p>When this method returns, the {@code anyLocalAddr} buffer is filled with the host address.
 * @param anyLocalAddr the buffer into which the host address will be stored
 * @param length the buffer length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the host address size in bytes or {@link J_EHOSTUNKNOWN} error code
 * if no any local address found or an error occurs
 * @warning anyLocalAddr must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKADDRESS_IMPL_lookupInaddrAny(int8_t* anyLocalAddr, int32_t length, uint8_t retry);

/**
 * Gets the local host name.
 * <p>When this method returns, the {@code localHostname} buffer is filled with the local host name.
 * @param localHostname the buffer into which the local host name will be stored
 * @param length the buffer length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the local host name length or {@link J_EHOSTUNKNOWN} error code
 * if no local host found or an error occurs
 * @warning localHostname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKADDRESS_IMPL_getLocalHostnameNative(int8_t* localHostname, int32_t length, uint8_t retry);

/**
 * Gets the loopback address.
 * <p>When this method returns, the {@code loopback} buffer is filled with the loopback address.
 *
 * @param loopback the buffer into which the loopback address will be stored
 * @param length the buffer length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the loopback address size in bytes  or {@link J_EHOSTUNKNOWN} error code
 * if no loopback address found or an error occurs
 * @warning loopback must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKADDRESS_IMPL_loopbackAddress(int8_t* loopback, int32_t length, uint8_t retry);

#ifdef __cplusplus
	}
#endif
#endif
