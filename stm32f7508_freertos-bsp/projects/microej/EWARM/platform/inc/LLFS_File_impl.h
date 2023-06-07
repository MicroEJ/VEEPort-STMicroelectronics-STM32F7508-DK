/*
 * C
 *
 * Copyright 2016-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef LLFS_FILE_IMPL
#define LLFS_FILE_IMPL

/**
 * @file
 * @brief MicroEJ FS low level API
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @date 28 March 2022
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
 * File mode for 'READ, WRITE'
 */
#define LLFS_FILE_MODE_READ_WRITE ('B')

/*
 * File mode for 'READ, WRITE, DATA Sync'
 */
#define LLFS_FILE_MODE_READ_WRITE_DATA_SYNC ('D')

/*
 * File mode for 'READ, WRITE, Sync'
 */
#define LLFS_FILE_MODE_READ_WRITE_SYNC ('S')

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

/**
 * Sets the file-pointer offset, measured from the beginning of this file, at which the next read or write occurs.
 * The offset may be set beyond the end of the file. Setting the offset beyond the end of the file does not change
 * the file length. The file length will change only by writing after the offset has been set beyond the end of the
 * file.
 *
 * @param file_id
 *            file identifier
 * @param pos
 *            the offset position, measured in bytes from the beginning of the file, at which to set the file
 *            pointer.
 * @exception NativeIOException on error.
 */
void LLFS_File_IMPL_seek(int32_t file_id, int64_t position);

/*
 * Returns the current offset of the give file descriptor.
 *
 * @param file_id
 *            file identifier
 *
 * @return the offset from the beginning of the file, in bytes, at which the next read or write occurs.
 * @exception NativeIOException on error.
 */
int64_t LLFS_File_IMPL_get_file_pointer(int32_t file_id);

/*
 * Sets the length of the given file.
 *
 * If the present length of the file is greater than the given new length then the file will be truncated.
 * In this case, if the file offset is greater than the given new length then after this method returns the offset will be
 * equal to the given new length.
 *
 * If the present length of the file is smaller than the given new length then the file will be extended.
 * In this case, the contents of the extended portion of the file are not defined.
 *
 * @param file_id
 *            file identifier
 * @param new_length
 *            the desired length of the file
 * @exception NativeIOException on error.
 */
void LLFS_File_IMPL_set_length(int32_t file_id, int64_t new_length);

/**
 * Returns the length of this file.
 *
 * @param fd file identifier
 * @return the length of this file, measured in bytes.
 * @exception NativeIOException on error.
 */
int64_t LLFS_File_IMPL_get_length_with_fd(int32_t file_id);


#ifdef __cplusplus
}
#endif
#endif
