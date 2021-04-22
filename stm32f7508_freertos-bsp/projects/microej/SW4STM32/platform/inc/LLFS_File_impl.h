/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * MicroEJ Corp. PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLFS_FILE_IMPL
#define LLFS_FILE_IMPL

/**
 * @file
 * @brief MicroEJ FS low level API
 * @author MicroEJ Developer Team
 * @version 2.1.0
 * @date 27 May 2020
 */

#include <stdint.h>
#include <LLFS_impl.h>
#include <intern/LLFS_File_impl.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * File mode for 'READ'
 */
#define LLFS_FILE_MODE_READ ('R')

/*
 * File mode for 'WRITE'
 */
#define LLFS_FILE_MODE_WRITE ('W')

/*
 * File mode for 'APPEND'
 */
#define LLFS_FILE_MODE_APPEND ('A')

/*
 * Open a file according given mode. The returned value is a positive
 * integer used to identify the file later.
 *
 * @param path
 *            absolute path of file to open
 * @param mode
 *            opening mode : {@link #LLFS_FILE_MODE_READ},
 *            {@link #LLFS_FILE_MODE_WRITE} or
 *            {@link #LLFS_FILE_MODE_APPEND}
 *
 * @return a unique ID
 *
 * @note Throws NativeIOException on error.
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_File_IMPL_open(uint8_t* path, uint8_t mode);

/*
 * Writes data to the channel.
 *
 * @param fileID
 *            file identifier
 * @param data
 *            pointer to byte array
 * @param offset
 * 		      the start offset in the data.
 * @param length
 *            number of data to write
 * @return The number of bytes written. Possibly zero if there was nothing to write, for instance.
 *
 * @note Throws NativeIOException on error.
 *
 * @warning data must not be used outside of the VM task or saved.
 */
int32_t LLFS_File_IMPL_write(int32_t file_id, uint8_t* data, int32_t offset, int32_t length);

/*
 * Writes the given byte in file.
 *
 * @param fileID
 *            file identifier
 * @param data
 *            the byte to write
 *
 * @note Throws NativeIOException on error.
 */
void LLFS_File_IMPL_write_byte(int32_t file_id, int32_t data);

/*
 * Reads data from the channel.
 *
 * @param fileID
 *            file identifier
 * @param data
 *            pointer to byte array
 * @param offset
 * 			  the start offset in the destination array <code>data</code>
 * @param length
 *            number of data to read
 * @return number of bytes read or {@link #LLFS_EOF} on EOF.
 *
 * @note Throws NativeIOException on error.
 *
 * @warning data must not be used outside of the VM task or saved.
 */
int32_t LLFS_File_IMPL_read(int32_t file_id, uint8_t* data, int32_t offset, int32_t length);

/*
 * Read one byte from the file.
 *
 * @param fileID
 *            file identifier
 * @return the read byte as an unsigned integer or {@link #LLFS_EOF} on EOF.
 *
 * @note Throws NativeIOException on error.
 */
int32_t LLFS_File_IMPL_read_byte(int32_t file_id);

/*
 * Close the file.
 *
 * @param fileID
 *            file identifier
 *
 * @note Throws NativeIOException on error.
 */
void LLFS_File_IMPL_close(int32_t file_id);


/*
 * Skips over and discards <code>n</code> bytes of data from the input
 * stream.
 *
 * <p>
 * The <code>skip</code> method may, for a variety of reasons, end up
 * skipping over some smaller number of bytes, possibly <code>0</code>. If
 * <code>n</code> is negative, the method will try to skip backwards. In
 * case the backing file does not support backward skip at its current
 * position, a NativeIOException is thrown. The actual number of bytes
 * skipped is returned. If it skips forwards, it returns a positive value.
 * If it skips backwards, it returns a negative value.
 *
 * <p>
 * This method may skip more bytes than what are remaining in the backing
 * file. This produces no exception and the number of bytes skipped may include
 * some number of bytes that were beyond the EOF of the backing file.
 * Attempting to read from the stream after skipping past the end will
 * result in {@link #LLFS_EOF} indicating the end of the file.
 *
 * @param fileID
 *            file identifier
 * @param n
 *            the number of bytes to be skipped.
 * @return the actual number of bytes skipped.
 *
 * @note Throws NativeIOException on error.
 */
int64_t LLFS_File_IMPL_skip(int32_t file_id, int64_t n);

/*
 * Returns an estimate of the number of remaining bytes that can be read (or
 * skipped over) from this channel without blocking by the next invocation
 * of a method for this channel. Returns 0 when the file position is beyond
 * EOF.
 *
 * @param fileID
 *            file identifier
 * @return an estimate of the number of remaining bytes that can be read (or
 *         skipped over) from this channel without blocking.
 *
 * @note Throws NativeIOException on error.
 */
int32_t LLFS_File_IMPL_available(int32_t file_id);

/*
 * For output streams, this method forces a write of all the
 * buffered data.
 * The open status of the stream is unaffected.
 * @param fileID
 *            file identifier
 * @note Throws NativeIOException on error.
 */
void LLFS_File_IMPL_flush(int32_t file_id);

#ifdef __cplusplus
}
#endif
#endif
