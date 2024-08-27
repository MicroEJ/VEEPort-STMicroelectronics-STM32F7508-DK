/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_DNS_IMPL
#define LLNET_DNS_IMPL

#include <stdint.h>

#include <intern/LLNET_DNS_impl.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Gets the host name associated to the input host IP address <code>address</code>.
 *
 * The given IP address is stored in network byte order (highest order byte of the address stored in <code>address[0]</code>).
 * Example: Assume that the given IP address is 192.168.4.10, the content of the input <code>address</code> buffer will be: <code>address[0]=192; address[1]=168; address[2]=4; address[3]=10</code>.
 *
 * When this function returns, the resolved host name is stored in the <code>hostname</code> buffer.
 *
 * @param[in] address the IP address whose the host name will be got.
 * @param[in] addressLength the IP address size (4 for IPv4 address or 16 for IPv6 address).
 * @param[out] hostname the output buffer into which the resolved host name will be stored. (The host name must be a null-terminated string).
 * @param[in] hostnameLength the <code>hostname</code> buffer length.
 * @return the host name length.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning host must not be used outside of the VM task or saved.
 */
int32_t LLNET_DNS_IMPL_getHostByAddr(int8_t* address, int32_t addressLength, uint8_t* hostname, int32_t hostnameLength);

/**
 * @brief Gets the IP address at the <code>index</code> position in the resolved IP addresses list of the given host name <code>hostname</code>.
 *
 * The buffer <code>hostname</code> contains the host name whose the IP address will be got.
 *
 * When this method returns, the IP address is stored in the <code>address</code> buffer.
 *
 * The IP address will be stored in network byte order (highest order byte of the address stored in <code>address[0]</code>).
 * Example: Assume that the IP address is 192.168.4.10, the content of the output <code>address</code> buffer will be: <code>address[0]=192; address[1]=168; address[2]=4; address[3]=10</code>.
 *
 * @param[in] index the index of the IP address to get.
 * @param[in] hostname the input host name (null-terminated string).
 * @param[in] hostnameLength the host name length. (The length does not include the terminating null byte).
 * @param[out] address the output buffer into which the IP address will be stored.
 * @param[in] addressLength the output IP address buffer length.
 * @return the IP address size in bytes (4 for IPv4 address or 16 for IPv6 address).
 *
 * @note NativeIOException if an error occurs.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning host must not be used outside of the VM task or saved.
 */
int32_t LLNET_DNS_IMPL_getHostByNameAt(int32_t index, uint8_t* hostname, int32_t hostnameLength, int8_t* address, int32_t addressLength);

/**
 * @brief Returns the number of host addresses associated with the given host name <code>hostname</code>.
 *
 * @param[in] hostname the host name buffer (null-terminated string).
 * @param[in] hostnameLength the host name length. (The length does not include the terminating null byte).
 * @return the number of addresses associated with the host name <code>hostname</code>.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning hostname must not be used outside of the VM task or saved.
 */
int32_t LLNET_DNS_IMPL_getHostByNameCount(uint8_t* hostname, int32_t hostnameLength);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_DNS_IMPL
