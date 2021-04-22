/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * MicroEJ Corp. PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * @file
 * @brief MicroEJ FS low level API
 * @author MicroEJ Developer Team
 * @version 2.1.0
 * @date 27 May 2020
 */

#define LLFS_File_IMPL_open Java_com_is2t_java_io_FileSystem_openNative
#define LLFS_File_IMPL_write Java_com_is2t_java_io_FileSystem_writeNative
#define LLFS_File_IMPL_write_byte Java_com_is2t_java_io_FileSystem_writeByteNative
#define LLFS_File_IMPL_read Java_com_is2t_java_io_FileSystem_readNative
#define LLFS_File_IMPL_read_byte Java_com_is2t_java_io_FileSystem_readByteNative
#define LLFS_File_IMPL_close Java_com_is2t_java_io_FileSystem_closeNative
#define LLFS_File_IMPL_skip Java_com_is2t_java_io_FileSystem_skipNative
#define LLFS_File_IMPL_available Java_com_is2t_java_io_FileSystem_availableNative
#define LLFS_File_IMPL_flush Java_com_is2t_java_io_FileSystem_flushNative
