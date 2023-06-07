/*
 * C
 *
 * Copyright 2016-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief MicroEJ FS low level API
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @date 28 March 2022
 */

#define LLFS_File_IMPL_open Java_com_is2t_java_io_FileSystem_openNative
#define LLFS_File_IMPL_write Java_com_is2t_java_io_FileSystem_writeNative
#define LLFS_File_IMPL_write_byte Java_com_is2t_java_io_FileSystem_writeByteNative
#define LLFS_File_IMPL_read Java_com_is2t_java_io_FileSystem_readNative
#define LLFS_File_IMPL_read_byte Java_com_is2t_java_io_FileSystem_readByteNative
#define LLFS_File_IMPL_close Java_com_is2t_java_io_FileSystem_closeNative
#define LLFS_File_IMPL_available Java_com_is2t_java_io_FileSystem_availableNative
#define LLFS_File_IMPL_flush Java_com_is2t_java_io_FileSystem_flushNative
#define LLFS_File_IMPL_seek Java_com_is2t_java_io_FileSystem_seekNative
#define LLFS_File_IMPL_get_file_pointer Java_com_is2t_java_io_FileSystem_getFilePointerNative
#define LLFS_File_IMPL_set_length Java_com_is2t_java_io_FileSystem_setLengthNative
#define LLFS_File_IMPL_get_length_with_fd Java_com_is2t_java_io_FileSystem_lengthWithFdNative
