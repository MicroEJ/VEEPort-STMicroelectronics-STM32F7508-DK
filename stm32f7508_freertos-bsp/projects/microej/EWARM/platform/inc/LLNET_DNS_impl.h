/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_DNS_IMPL
#define LLNET_DNS_IMPL

#include <sni.h>
#include <intern/LLNET_DNS_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif


/**
 * Gets the host name associated to the input host address {@code host}.
 * <p>When this method returns, the associated host name is stored in the same buffer {@code host}.
 * @param host the input buffer that contains the host address whose the host name will be got
 * and the output buffer into which the host name will be stored
 * @param offset the offset of the input host address in the buffer
 * @param length the input host address length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the host name length or {@link J_EHOSTUNKNOWN} error code
 * if no host associated to this address or an error occurs
 * @warning host must not be used outside of the VM task or saved.
 */
int32_t LLNET_DNS_IMPL_getHostByAddr(int8_t* host, int32_t offset, int32_t length, uint8_t retry);

/**
 * Gets the host address at the {@code index} position in the host name addresses list.
 * <p>The buffer {@code host} contains the input host name whose the host address will be got and
 * when this method returns, the host address is stored in the same buffer {@code host}.
 * @param index the index of the address to get
 * @param host the input host name buffer and the output buffer into which the host address will be stored
 * @param offset the offset of the host name in the buffer
 * @param length the host name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the host address size in bytes or {@link J_EHOSTUNKNOWN} error code
 * if no host address associated to this host name or an error occurs
 * @warning host must not be used outside of the VM task or saved.
 */
int32_t LLNET_DNS_IMPL_getHostByNameAt(int32_t index, int8_t* host, int32_t offset, int32_t length, uint8_t retry);

/**
 * Returns the number of host addresses associated with the host name {@code hostname}.
 * @param hostname the host name buffer
 * @param offset the offset of the host name in the buffer
 * @param length the host name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the number of addresses associated with the host name {@code hostname}
 * or {@link J_EHOSTUNKNOWN} error code if an error occurs
 * @warning dst must not be used outside of the VM task or saved.
 */
int32_t LLNET_DNS_IMPL_getHostByNameCount(int8_t* hostname, int32_t offset, int32_t length, uint8_t retry);

#ifdef __cplusplus
	}
#endif
#endif
