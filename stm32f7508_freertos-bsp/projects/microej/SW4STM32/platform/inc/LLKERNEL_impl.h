/* 
 * C
 * 
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * This file is auto-generated - DO NOT EDIT IT
 */
/* 
 * Implementation header file. Shall only be included by client implementation C files.
 */
#include <stdint.h>
#include <intern/LLKERNEL_impl.h>
// --------------------------------------------------------------------------------
// -                                  Constants                                   -
// --------------------------------------------------------------------------------

/**
 * Returned value when function call has succeeded.
 */
#define LLKERNEL_OK (0)

/**
 * Returned value when function call has failed.
 */
#define LLKERNEL_ERROR (-1)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError}
 * function when the Installed Feature content is corrupted. The checksum
 * computed during the installation does not match the actual computed
 * checksum.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_CORRUPTED_CONTENT (1)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError} function when
 * the Installed Feature has not been built for this Kernel UID or a compatible Kernel UID.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_INCOMPATIBLE_KERNEL_WRONG_UID (2)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError} function when
 * there is no internal free slot to connect the Installed Feature.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_TOO_MANY_INSTALLED (3)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError} function when
 * the Installed Feature matches an already Installed Feature.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_ALREADY_INSTALLED (4)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError} function when
 * the Installed Feature has been linked on a Kernel Address Table with different content.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_INCOMPATIBLE_KERNEL_WRONG_ADDRESSES (5)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError} function when
 * the Installed Feature ROM area overlaps the ROM area of an already Installed Feature.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_ROM_OVERLAP (6)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError} function when
 * the Installed Feature RAM area overlaps the RAM area of an already Installed Feature.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_RAM_OVERLAP (7)

/**
 * Error code thrown by {@link LLKERNEL_onFeatureInitializationError}
 * function when the RAM address returned by
 * {@link LLKERNEL_getFeatureAddressRAM} is not equals to the RAM address
 * returned when the Feature has been linked on this Kernel.
 */
#define LLKERNEL_FEATURE_INIT_ERROR_RAM_ADDRESS_CHANGED (8)

// --------------------------------------------------------------------------------
// -                      Functions that must be implemented                      -
// --------------------------------------------------------------------------------

/**
 * Allocates a new chunk of memory in the Kernel Working Buffer.
 * 
 * @param size
 *            the size in bytes
 * @return a 32 bits aligned address, or <code>null</code> on error
 */
void* LLKERNEL_IMPL_allocateWorkingBuffer(int32_t size);

/**
 * Releases a chunk of memory previously allocated using
 * {@link LLKERNEL_allocateWorkingBuffer}.
 * 
 * @param chunk_address
 *            the start address of a previously allocated chunk
 */
void LLKERNEL_IMPL_freeWorkingBuffer(void* chunk_address);

/**
 * Allocates a new Feature and reserves its ROM and RAM areas. If the
 * implementation does not support custom Feature allocation, it must return
 * <code>0</code>. In this latter case, the Feature is allocated in the
 * Kernel Working Buffer and installed in-place.
 * <p>
 * Warning: ROM and RAM areas reservation must take care of address alignment
 * constraints required by {@link LLKERNEL_getFeatureAddressROM} and
 * {@link LLKERNEL_getFeatureAddressRAM}.
 * 
 * @param size_ROM
 *            the size in bytes to allocate in ROM
 * 
 * @param size_RAM
 *            the size in bytes to allocate in RAM
 * 
 * @return a handle that uniquely identifies the allocated Feature, or
 *         <code>0</code> if custom Feature allocation is not supported
 */
int32_t LLKERNEL_IMPL_allocateFeature(int32_t size_ROM, int32_t size_RAM);

/**
 * Releases a Feature previously allocated using
 * {@link LLKERNEL_allocateFeature} and frees its
 * ROM and RAM areas.
 * 
 * @param handle
 *            the Feature handle
 */
void LLKERNEL_IMPL_freeFeature(int32_t handle);

/**
 * Gets the current number of allocated Features.
 * <p>
 * This function is called once at the start of the Core Engine. Afterward, the Core Engine will retrieve Feature
 * handles by calling {@link LLKERNEL_getFeatureHandle} for each index in the range <code>[0..allocated_features_count[</code>.
 * 
 * @return the number of Features allocated using {@link LLKERNEL_allocateFeature}, but not yet released by
 *         {@link LLKERNEL_freeFeature}. If there are no Features, it will return <code>0</code>.
 * @see LLKERNEL_getFeatureHandle
 */
int32_t LLKERNEL_IMPL_getAllocatedFeaturesCount(void);

/**
 * Gets the Feature handle that matches the given allocation index.
 * <p>
 * This function is called at the start of the Core Engine for each allocation index from <code>0</code> to
 * <code>{@link LLKERNEL_getAllocatedFeaturesCount}-1</code>.
 * 
 * @param allocation_index
 *            an index in the range <code>[0..{@link LLKERNEL_getAllocatedFeaturesCount}[</code>
 * @return a handle that uniquely identifies the allocated Feature. <code>0</code> is not allowed.
 * @see LLKERNEL_getAllocatedFeaturesCount
 */
int32_t LLKERNEL_IMPL_getFeatureHandle(int32_t allocation_index);

/**
 * Gets the address of the allocated RAM area for the given Feature.
 * The returned address must be <code>8</code> bytes aligned.
 * 
 * @param handle
 *            the Feature handle
 * 
 * @return the start address in bytes of the allocated RAM area.
 */
void* LLKERNEL_IMPL_getFeatureAddressRAM(int32_t handle);

/**
 * Gets the address of the allocated ROM area for the given Feature.
 * The returned address must be <code>16</code> bytes aligned.
 * 
 * @param handle
 *            the Feature handle
 * 
 * @return the start address in bytes of the allocated ROM area.
 */
void* LLKERNEL_IMPL_getFeatureAddressROM(int32_t handle);

/**
 * Copies a sequence of bytes to ROM area. This function does not require
 * that all bytes are really copied to the destination before it returns.
 * 
 * @param dest_address_ROM
 *            the start address in ROM (destination)
 * @param src_address
 *            the start address of bytes to copy (source)
 * @param size
 *            the number of bytes to copy
 * @return {@link LLKERNEL_OK} on success, {@link LLKERNEL_ERROR} on error.
 */
int32_t LLKERNEL_IMPL_copyToROM(void* dest_address_ROM, void* src_address, int32_t size);

/**
 * This function is called once all calls to
 * {@link LLKERNEL_copyToROM} are done for the currently allocated
 * Feature. This function must block until all bytes are really copied to
 * ROM area.
 * 
 * @return {@link LLKERNEL_OK} on success, {@link LLKERNEL_ERROR} on error.
 */
int32_t LLKERNEL_IMPL_flushCopyToROM(void);

/**
 * This function is called during Kernel boot when the initialization of an
 * allocated Feature failed.
 * 
 * @param handle
 *            a Feature handle returned by {@link LLKERNEL_getFeatureHandle}
 * 
 * @param error_code
 *            the error code indicating the initialization error cause (see
 *            <code>FEATURE_INIT_ERROR_*</code> constants)
 * @return {@link LLKERNEL_OK} to skip the initialization of this Feature and
 *         continue with the next allocated Feature, or {@link LLKERNEL_ERROR} if
 *         the Kernel must abruptly stop.
 */
int32_t LLKERNEL_IMPL_onFeatureInitializationError(int32_t handle, int32_t error_code);
