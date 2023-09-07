/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_NETWORKINTERFACE_IMPL
#define LLNET_NETWORKINTERFACE_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_NETWORKINTERFACE_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Gets the hardware address associated with the given network interface name {@code ifname}.
 * <p>When this method returns, the buffer {@code hwAddr} is filled with the hardware address.
 * @param ifname the network interface name buffer
 * @param ifnameLen the network interface name length
 * @param hwAddr the buffer into which the hardware address will be stored
 * @param hwAddrMaxLength maximum size of hardware address that can be stored in the buffer {@code hwAddr}
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the hardware address size in bytes, 0 if no hardware address found or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 * @warning hwAddr must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getHardwareAddress(int8_t* ifname, int32_t ifnameLen,
		int8_t* hwAddr, int32_t hwAddrMaxLength, uint8_t retry);


/**
 * Gets the network interface name at the {@code index} position in the network interfaces list.
 * When this method returns, the {@link ifname} buffer is filled with the network interface name
 * @param index the network interface index
 * @param ifname the buffer into which the network interface name will be stored
 * @param length the buffer length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the network interface name length, 0 if no interface found or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterface(int32_t index, int8_t* ifname, int32_t length, uint8_t retry);

/**
 * Returns the number of the network interface addresses.
 * @param index the network interface index-position in the interfaces list
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the number of the network interface addresses, or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddressesCount(int32_t index, uint8_t retry);

/**
 * The addresses tagged with this are IPv4 addresses
 * @see {@link LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress}
 */
#define IPV4_ADDR_TAG 4

/**
 * The addresses tagged with this are IPv6 addresses
 * @see {@link LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress}
 */
#define IPV6_ADDR_TAG 6

/**
 * Size of the address info returned by LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress when
 * the address is an IPv4 address.
 * <p> It's computed like this:
 * tag (1) + IP (4) + prefix length (1) + hasBroadcast (1) + broadcast IP (4)
 * @see {@link LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress}
 */
#define IPV4_ADDR_INFO_SIZE 11

/**
 * Size of the address info returned by LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress when
 * the address is an IPv4 address.
 * <p> It's computed like this:
 * tag (1) + IP (16) + prefix length (1)
 * @see {@link LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress}
 */
#define IPV6_ADDR_INFO_SIZE 18

/**
 * Gets a network interface address in the interface addresses list.
 * <p>When this method returns, the {@code addr} buffer is filled with the network interface address information, using the following layout:
 * <ul> a one-byte tag ({@link IPV4_ADDR_INFO_SIZE} or {@link IPV6_ADDR_INFO_SIZE})
 * <ul> the IP address bytes (4 or 16)
 * <ul> the prefix length in bits (encoded values from 0 to 0x80, meaning 1 to 128)
 * <ul> for an IP v4 address, a one-byte boolean value (0 or 1) telling if there's a broadcast address
 * <ul> for an IP v4 address that has broadcast, the 4 bytes of the broadcast address
 * </ul>
 * @param idxIf the index-position of the interface in the interfaces list
 * @param idxAddr the index-position of the address in the interface addresses list
 * @param addr the buffer into which the address will be stored
 * @param length the buffer length ({@link IPV4_ADDR_TAG} or {@link IPV6_ADDR_TAG})
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the network interface address size in bytes or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning addr must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress(int32_t idxIf, int32_t idxAddr, int8_t* addr, int32_t length, uint8_t retry);

/**
 * Returns the number of network interfaces.
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the number of network interfaces or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfacesCount(uint8_t retry);

/**
 * Checks if the given network interface {@code ifname} is a loopback interface.
 * @param ifname the network interface name
 * @param length the network interface name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 if true, 1 if false or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_isLoopback(int8_t* ifname, int32_t length, uint8_t retry);

/**
 * Checks if the given network interface {@code ifname} is a point-to-point interface.
 * @param ifname the network interface name
 * @param length the network interface name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 if true, 1 if false or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_isPointToPoint(int8_t* ifname, int32_t length, uint8_t retry);

/**
 * Checks if the given network interface {@code ifname} is up.
 * @param ifname the network interface name
 * @param length the network interface name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 if true, 1 if false or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_isUp(int8_t* ifname, int32_t length, uint8_t retry);

/**
 * Checks if the given network interface {@code ifname} supports multicast.
 * @param name the network interface name
 * @param length the network interface name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 if true, 1 if false or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_supportsMulticast(int8_t* ifname, int32_t length, uint8_t retry);

/**
 * @param name the network interface name
 * @param length the network interface name length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the MTU as an int, or a negative error code
 * @see {@link NetErrors} for error codes
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getMTU(int8_t* ifname, int32_t length, uint8_t retry);

#ifdef __cplusplus
	}
#endif
#endif
