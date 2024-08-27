/**
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef _LLEXT_RES
#define _LLEXT_RES
#include <stdint.h>
#ifdef __cplusplus
	extern "C" {
#endif

/** End Of File */
#define LLEXT_RES_EOF	-1
/** No error */
#define LLEXT_RES_OK	0

/**
 * Resource identifier.
 */
typedef int32_t RES_ID;

/**
 * Open the resource whose name is the string pointed to by path.
 * @param path a null terminated string.
 * @return the resource ID on success, a negative value on error.
 */
RES_ID LLEXT_RES_open(const char* path);

/**
 * Close the resource referenced by the given resourceID
 * @param resourceID an ID returned by the LLEXT_RES_open function.
 * @return LLEXT_RES_OK on success, a negative value on error.
 */
int32_t LLEXT_RES_close(RES_ID resourceID);

/**
 * Returns the resource base address or -1 when not available.
 *
 * The resource base address is an address available in CPU address space range. That means the
 * CPU can access to the resource memory using the same assembler instructions than the CPU internal memories.
 * In this case this memory is not considered as external memory. This address will be used by
 * the caller even if the resource is closed.
 *
 * If the memory is outside the CPU address space range, this function should returns -1. In this
 * case the caller will use this memory as external memory and will have to perform some memory copies in RAM.
 * The memory accesses may be too slow. To force the caller to consider this memory as an external
 * memory whereas this memory is available in CPU address space range, this function should returns -1 too.
 *
 * The returned address is always the resource base address. This address must not change during application
 * execution.
 *
 * @param resourceID an ID returned by the openResource method.
 * @return resource address or -1 when address is outside CPU address space range.
 */
int32_t LLEXT_RES_getBaseAddress(RES_ID resourceID);

/**
 * Try to read size bytes from the stream referenced by the given resourceID.
 * The read bytes are copied at given ptr. Parameter <code>size</code>
 * is a pointer on an integer which defines the maximum number of bytes to
 * read. This value must be updated by the number of bytes read.
 * @param resourceID an ID returned by the LLEXT_RES_open function.
 * @param ptr the buffer into which the data is read.
 * @param size the maximum number of bytes to read.
 * @return LLEXT_RES_OK on success, LLEXT_RES_EOF when end of file is reached, another negative value on error.
 */
int32_t LLEXT_RES_read(RES_ID resourceID, void* ptr, int32_t* size);

/**
 * Returns an estimate of the number of bytes that can be read from the stream referenced by the given resourceID without blocking by the next read.
 * @param resourceID an ID returned by the LLEXT_RES_open function.
 * @return an estimate of the number of bytes that can be read, 0 when end of file is reached, a negative value on error.
 */
int32_t LLEXT_RES_available(RES_ID resourceID);

/**
 * Sets the file position indicator for the stream pointed to by resourceID.
 * The new position, measured in bytes, is obtained by adding offset bytes to the start of the file.
 * @param resourceID an ID returned by the LLEXT_RES_open function.
 * @param offset new value in bytes of the file position indicator.
 * @return LLEXT_RES_OK on success, a negative value on error.
 */
int32_t LLEXT_RES_seek(RES_ID resourceID, int64_t offset);

/**
 * Obtains the current value of the file position indicator for the stream pointed to by resourceID.
 * @param resourceID an ID returned by the LLEXT_RES_open function.
 * @return the current value in bytes of the file position indicator on success, a negative value on error.
 */
int64_t LLEXT_RES_tell(RES_ID resourceID);


#ifdef __cplusplus
	}
#endif
#endif
