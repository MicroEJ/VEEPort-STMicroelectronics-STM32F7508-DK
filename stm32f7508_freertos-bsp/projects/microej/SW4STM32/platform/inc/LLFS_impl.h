/*
 * C
 *
 * Copyright 2016-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef LLFS_IMPL
#define LLFS_IMPL

/**
 * @file
 * @brief MicroEJ FS low level API
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @date 28 March 2022
 */

#include <stdint.h>
#include <intern/LLFS_impl.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Return code when an action succeeds
 */
#define LLFS_OK (0)

/*
 * Return code for End Of File (EOF)
 */
#define LLFS_EOF (-1)

/*
 * Return code when an error occurs
 */
#define LLFS_NOK (-2)

/*
 * Return code when a file is not created
 * @see LLFS_IMPL_create
 */
#define LLFS_NOT_CREATED (-3)

/*
 * Permission on a file: owner only
 * @see LLFS_IMPL_set_permission
 */
#define LLFS_PERMISSION_OWNER_ONLY  (0)

/*
 * Permission on a file: not owner only
 * @see LLFS_IMPL_set_permission
 */
#define LLFS_PERMISSION_ALL  (1)

/*
 * Enable a permission on a file
 * @see LLFS_IMPL_set_permission
 */
#define LLFS_PERMISSION_ENABLE  (0)

/*
 * Disable a permission on a file
 * @see LLFS_IMPL_set_permission
 */
#define LLFS_PERMISSION_DISABLE  (1)

/*
 * Read access mode
 * @see LLFS_IMPL_set_permission
 * @see LLFS_IMPL_is_accessible
 */
#define LLFS_ACCESS_READ 	(0x04)

/*
 * Write access mode
 * @see LLFS_IMPL_set_permission
 * @see LLFS_IMPL_is_accessible
 */
#define LLFS_ACCESS_WRITE 	(0x02)

/*
 * Execute access mode
 * @see LLFS_IMPL_set_permission
 * @see LLFS_IMPL_is_accessible
 */
#define LLFS_ACCESS_EXECUTE 	(0x01)

/*
 * Free space identifier
 * @see LLFS_IMPL_get_space_size
 */
#define LLFS_FREE_SPACE (0x0)

/*
 * Total space identifier
 * @see LLFS_IMPL_get_space_size
 */
#define LLFS_TOTAL_SPACE (0x1)

/*
 * Usable space identifier
 * @see LLFS_IMPL_get_space_size
 */
#define LLFS_USABLE_SPACE (0x2)

typedef struct {
	int32_t year;//see Java Calendar.YEAR field
	int32_t month;//see Java Calendar.MONTH field
	int32_t day;//see Java Calendar.DAY field
	int32_t hour;//see Java Calendar.HOUR_OF_DAY field
	int32_t minute;//see Java Calendar.MINUTE field
	int32_t second;//see Java Calendar.SECOND field
	int32_t millisecond;//see Java Calendar.MILLISECOND field
} LLFS_date_t;

/*
 * Initializes the C part of the FS library such as the file system
 * middleware, the management tasks and the drivers.
 *
 * @note Throws NativeException on error.
 */
void LLFS_IMPL_initialize(void);

/*
 * Returns the maximum path length managed in native.
 */
int32_t LLFS_IMPL_get_max_path_length(void);

/*
 * Get last modified date of the file. The date must be stored in the given
 * structure.
 *
 * @param path
 *            absolute path of file
 * @param date
 *            the structure where store the date
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path and date must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_get_last_modified(uint8_t* path, LLFS_date_t* date);

/*
 * Update file as read only.
 *
 * @param path
 *            absolute path of file
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_set_read_only(uint8_t* path);

/*
 * Creates a new empty file with the given pathname.
 * <p>
 * Returns {@link #LLFS_OK} if the file was created, {@link #LLFS_NOT_CREATED}
 * if a file or directory with the given pathname already exists, throw a
 * NativeIOException if an I/O error occurs.
 *
 * @param path
 *            absolute path of file
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOT_CREATED}
 *
 * @warning path must not be used outside of the VM task or saved.
 *
 * @note Throws NativeIOException on error.
 */
int32_t LLFS_IMPL_create(uint8_t* path);

/*
 * Opens a directory in order to read from it later. Framework ensures only
 * one directory is opened at the same time closing current directory
 * without opening a new one. The returned value is an integer used
 * as ID to identify the opened directory. This ID will be used later by
 * read and close methods.
 *
 * @param path
 *            absolute path of directory
 * @return an ID out of the range <code>[{@link #LLFS_NOT_CREATED}-{@link #LLFS_OK}]</code> 
 *         or {@link #LLFS_NOK} if the directory could not be open.
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_open_directory(uint8_t* path);

/*
 * Specify the next available file in directory. Returns {@link #LLFS_NOK}
 * when no more file is available.
 *
 * @param dirID
 *            the directory ID
 * @param path
 *            path where store the next available file
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_read_directory(int32_t directory_ID, uint8_t* path);

/*
 * Close the directory.
 *
 * @param dirID
 *            the directory ID
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 */
int32_t LLFS_IMPL_close_directory(int32_t directory_ID);

/*
 * Rename a file.
 *
 * @param path
 *            absolute path of file to modify
 * @param newPath
 *            new absolute path of file
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path and new_path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_rename_to(uint8_t* path, uint8_t* new_path);

/*
 * Get the length, in bytes, of the file denoted by path, or 0L if the file
 * does not exist. Some operating systems may return 0L for pathnames
 * denoting system-dependent entities such as devices or pipes
 *
 * @param path
 *            absolute path of file to test
 * @return The length of the file or {@link #LLFS_NOK} on error.
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int64_t LLFS_IMPL_get_length(uint8_t* path);

/**
 * Returns {@link #LLFS_OK} if and only if the file or directory denoted by
 * this abstract pathname exists, {@link #LLFS_NOK} otherwise.
 *
 * @param path
 *            absolute path of file to test
 * @return {@link #LLFS_OK} when file exists, {@link #LLFS_NOK} otherwise.
 *
 * @warning path must not be used outside of the VM task or saved.

 */
int32_t LLFS_IMPL_exist(uint8_t* path);

/**
 * Depending on <code>spaceType</code> returns the following values:
 * <ul>
 *
 * 	<li>{@link #LLFS_FREE_SPACE}: number of unallocated bytes on the partition or
 * 0L if the path does not name a partition</li>
 *
 * 	<li>{@link #LLFS_TOTAL_SPACE}: size, in bytes, of the partition or 0L if this
 * path does not name a partition</li>
 *
 * 	<li>{@link #LLFS_USABLE_SPACE}: number of available bytes on the partition or
 * 0L if the path does not name a partition. On systems where this information is
 * not available, this method will be equivalent to a call to
 * <code>getSpaceNative(LLFS_FREE_SPACE)</code>.</li>
 *
 * </ul>
 *
 * @param path
 *            absolute path of file in the require partition
 * @param space_type
 * 			  {@link #LLFS_FREE_SPACE}, {@link #LLFS_TOTAL_SPACE} or {@link #LLFS_USABLE_SPACE}.
 *
 * @return The number of bytes or {@link #LLFS_NOK} on error.
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int64_t LLFS_IMPL_get_space_size(uint8_t* path, int32_t space_type);

/*
 * Create the directory.
 *
 * @param path
 *            absolute path of directory with the last entity to create
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_make_directory(uint8_t* path);

/*
 * Tests whether the file named by this abstract pathname is a hidden file.
 * The exact definition of hidden is system-dependent. On UNIX systems, a
 * file is considered to be hidden if its name begins with a period
 * character ('.'). On Microsoft Windows systems, a file is considered to be
 * hidden if it has been marked as such in the filesystem.
 *
 * @param path
 *            absolute path of file to test.
 * @return {@link #LLFS_OK} if and only if the file denoted by this path is
 *         hidden according to the conventions of the underlying platform,
 *         {@link #LLFS_NOK} otherwise
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_is_hidden(uint8_t* path);

/*
 * Check if the file denoted by this abstract pathname exists and is a
 * directory
 *
 * @param path
 *            absolute path of file to test
 * @return {@link #LLFS_OK} (file is a directory) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_is_directory(uint8_t* path);

/*
 * Check if the file denoted by this abstract pathname exists and is a file
 *
 * @param path
 *            absolute path of file to test
 * @return {@link #LLFS_OK} (file is not a directory) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_is_file(uint8_t* path);

/*
 * Change the last modified date of file.
 *
 * @param path
 *            absolute path of file to modified
 * @param date
 *            the new date
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_set_last_modified(uint8_t* path, LLFS_date_t* date);

/*
 * Delete the file or directory denoted by the given absolute pathname.
 *
 * @param path
 *            absolute path of file to modified
 *
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_delete(uint8_t* path);

/*
 * Check whether the file or directory denoted by the given abstract
 * pathname may be accessed by this process. The second argument specifies
 * which access, {@link #ACCESS_READ}, {@link #ACCESS_WRITE} or
 * {@link #ACCESS_EXECUTE}, to check.
 *
 * @param path
 *            absolute path of file to modified
 * @param access
 *            {@link #ACCESS_READ}, {@link #ACCESS_WRITE} or
 *            {@link #ACCESS_EXECUTE}
 *
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_is_accessible(uint8_t* path, int32_t access);

/*
 * Set on or off the access permission (to owner only or to all) to the file
 * or directory denoted by the given abstract pathname.
 *
 * @param path
 *            absolute path of file to modified
 * @param access
 *            {@link #ACCESS_READ}, {@link #ACCESS_WRITE} or
 *            {@link #ACCESS_EXECUTE}
 * @param enable
 *            {@link #LLFS_PERMISSION_ENABLE} or
 *            {@link #LLFS_PERMISSION_DISABLE}
 * @param owner
 *            {@link #LLFS_PERMISSION_OWNER_ONLY} or
 *            {@link #LLFS_PERMISSION_ALL}
 *
 * @return {@link #LLFS_OK} (no error) or {@link #LLFS_NOK}
 *
 * @warning path must not be used outside of the VM task or saved.
 */
int32_t LLFS_IMPL_set_permission(uint8_t* path, int32_t access, int32_t enable, int32_t owner);

#ifdef __cplusplus
}
#endif
#endif
