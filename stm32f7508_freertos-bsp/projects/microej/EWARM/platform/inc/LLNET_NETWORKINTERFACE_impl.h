/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_NETWORKINTERFACE_IMPL
#define LLNET_NETWORKINTERFACE_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_NETWORKINTERFACE_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief The addresses tagged with this are IPv4 addresses.
 *
 * @see LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress()
 */
#define IPV4_ADDR_TAG 4

/**
 * @brief The addresses tagged with this are IPv6 addresses.
 *
 * @see LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress()
 */
#define IPV6_ADDR_TAG 6

/**
 * @brief Size of the address info returned by LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress when
 * the address is an IPv4 address.
 * <p> It's computed like this:
 * tag (1) + IP (4) + prefix length (1) + hasBroadcast (1) + broadcast IP (4)</p>
 *
 * @see LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress()
 */
#define IPV4_ADDR_INFO_SIZE 11

/**
 * @brief Size of the address info returned by LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress when
 * the address is an IPv4 address.
 * <p> It's computed like this:
 * tag (1) + IP (16) + prefix length (1)</p>
 *
 * @see LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress()
 */
#define IPV6_ADDR_INFO_SIZE 18

/**
 * @brief Gets the hardware address associated with the given network interface name <code>ifname</code>.
 * <p>When this method returns, the buffer <code>hwAddr</code> is filled with the hardware address.
 *
 * @param[in]	ifname			The network interface name (null-terminated string).
 * @param[in]	ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 * @param[out]	hwAddr 			The buffer into which the hardware address will be stored.
 * @param[in]	hwAddrMaxLength	Maximum size of hardware address that can be stored in the buffer <code>hwAddr</code>.
 *
 * @return The hardware address size in bytes, '0' if no hardware address found.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 * @warning hwAddr must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getHardwareAddress(uint8_t* ifname, int32_t ifnameLength,
		int8_t* hwAddr, int32_t hwAddrMaxLength);

/**
 * @brief Gets the network interface name at the <code>index</code> position in the network interfaces list.
 * When this method returns, the <code>ifnameBuffer</code> buffer is filled with the network interface name.
 *
 * @param[in]	index				The index-position of the interface in the interfaces list.
 * @param[out]	ifnameBuffer		The buffer into which the network interface name will be stored. (The stored name must be null-terminated string).
 * @param[in]	ifnameBufferLength	The length of the network interface buffer.
 *
 * @return The network interface name length (in bytes); 0 or negative value if no interface found. (The returned length must not include the terminating null byte).
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifnameBuffer must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterface(int32_t index, uint8_t* ifnameBuffer, int32_t ifnameBufferLength);

/**
 * @brief Returns the number of the network interface addresses.
 *
 * @param[in] 	index 			The index-position of the interface in the interfaces list.
 * @param[in]	ifname			The network interface name (null-terminated string).
 * @param[in]	ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 *
 * @return The number of the network interface addresses.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddressesCount(int32_t index, uint8_t* ifname, int32_t ifnameLength);

/**
 * @brief Gets a network interface address in the interface addresses list.
 * When this method returns, the <code>addrInfo</code> buffer is filled with the network interface address information, using the following layout:
 * <ul>
 * 	<li>a one-byte tag (#IPV4_ADDR_TAG or #IPV6_ADDR_TAG)</li>
 * 	<li> the IP address bytes (4 or 16)</li>
 * 	<li> the prefix length in bits (encoded values from 0 to 0x80, meaning 1 to 128)</li>
 * 	<li> for an IP v4 address, a one-byte boolean value (0 or 1) telling if there's a broadcast address</li>
 * 	<li> for an IP v4 address that has broadcast, the 4 bytes of the broadcast address</li>
 * </ul>
 *
 * @param[in]	idxIf			The index-position of the interface in the interfaces list.
 * @param[in]	ifname			The network interface name (null-terminated string).
 * @param[in]	ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 * @param[in]	idxAddr			The index-position of the address in the interface addresses list.
 * @param[out]	addrInfo		The buffer into which the address information will be stored.
 * @param[in]	addrInfoLength	The length of the address information buffer.
 *
 * @return The network interface address information size in bytes (IPV4_ADDR_INFO_SIZE for IPv4 or IPV6_ADDR_INFO_SIZE for IPv6).
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 * @warning addrInfo must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress(int32_t idxIf, uint8_t* ifname, int32_t ifnameLength, int32_t idxAddr, int8_t* addrInfo, int32_t addrInfoLength);

/**
 * @brief Returns the number of network interfaces.
 *
 * @return The number of network interfaces.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfacesCount(void);

/**
 * @brief Checks if the given network interface <code>ifname</code> is a loopback interface.
 *
 * @param[in] ifname		The network interface name (null-terminated string).
 * @param[in] ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 *
 * @return '0' if true, '1' if false.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_isLoopback(uint8_t* ifname, int32_t ifnameLength);

/**
 * @brief Checks if the given network interface <code>ifname</code> is a point-to-point interface.
 *
 * @param[in] ifname		The network interface name (null-terminated string).
 * @param[in] ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 *
 * @return '0' if true, '1' if false.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_isPointToPoint(uint8_t* ifname, int32_t ifnameLength);

/**
 * @brief Checks if the given network interface <code>ifname</code> is up.
 *
 * @param[in] ifname 		The network interface name (null-terminated string).
 * @param[in] ifnameLength 	The network interface name length. (The length does not include the terminating null byte).
 *
 * @return '0' if true, '1' if false.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_isUp(uint8_t* ifname, int32_t ifnameLength);

/**
 * @brief Checks if the given network interface <code>ifname</code> supports multicast.
 *
 * @param[in] ifname		The network interface name (null-terminated string).
 * @param[in] ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 *
 * @return '0' if true, '1' if false.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_supportsMulticast(uint8_t* ifname, int32_t ifnameLength);

/**
 * @brief Gets the the Maximum Transmission Unit (MTU) of the interface with the
 * given network interface name <code>ifname</code>.
 *
 * @param[in] ifname		The network interface name (null-terminated string).
 * @param[in] ifnameLength	The network interface name length. (The length does not include the terminating null byte).
 *
 * @return The MTU as an integer.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning ifname must not be used outside of the VM task or saved.
 */
int32_t LLNET_NETWORKINTERFACE_IMPL_getMTU(uint8_t* ifname, int32_t ifnameLength);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_NETWORKINTERFACE_IMPL
