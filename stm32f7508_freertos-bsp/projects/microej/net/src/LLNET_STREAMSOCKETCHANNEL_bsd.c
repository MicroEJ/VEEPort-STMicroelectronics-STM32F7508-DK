/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_STREAMSOCKETCHANNEL 3.0.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 17 June 2022
 */

#include <LLNET_STREAMSOCKETCHANNEL_impl.h>


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "sni.h"
#include "LLNET_configuration.h"
#include <LLNET_CHANNEL_impl.h>
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

#if LLNET_AVAILABLE_IMPL_ALT == LLNET_USE_MSG_PEEK_FOR_AVAILABLE

/* Define the buffers used for LLNET_STREAMSOCKETCHANNEL_IMPL_available() function */

#ifndef LLNET_MSG_PEEK_AVAILABLE_BUFFER_SIZE
// The LLNET_configuration.h must define the size of this buffer: LLNET_MSG_PEEK_AVAILABLE_BUFFER_SIZE.
#error "Please set the macro LLNET_MSG_PEEK_AVAILABLE_BUFFER_SIZE in LLNET_configuration.h to define the size of the buffer to be used to get the available data length."
#endif // LLNET_MSG_PEEK_AVAILABLE_BUFFER_SIZE

#ifdef LLNET_MSG_PEEK_AVAILABLE_BUFFER_SECTION
static uint8_t __attribute__((section(LLNET_MSG_PEEK_AVAILABLE_BUFFER_SECTION))) LLNET_MSG_PEEK_AVAILABLE_BUFFER[LLNET_MSG_PEEK_AVAILABLE_BUFFER_SIZE];
#else
static uint8_t LLNET_MSG_PEEK_AVAILABLE_BUFFER[LLNET_MSG_PEEK_AVAILABLE_BUFFER_SIZE];
#endif //LLNET_MSG_PEEK_AVAILABLE_BUFFER_SECTION

#endif // LLNET_AVAILABLE_IMPL_ALT == LLNET_USE_MSG_PEEK_FOR_AVAILABLE

static void LLNET_STREAMSOCKETCHANNEL_write_callback(int32_t fd, int8_t* src, int32_t offset, int32_t length);
static void LLNET_STREAMSOCKETCHANNEL_write(int32_t fd, int8_t* buffer, int32_t current_written_length, int32_t remaining_length);

static void LLNET_STREAMSOCKETCHANNEL_write_callback(int32_t fd, int8_t* src, int32_t offset, int32_t length){

	int32_t current_written_length = 0;
	int32_t remaining_length = 0;

	//get the current written length;
	SNI_getCallbackArgs((void**)&current_written_length, NULL);

	//compute the new remaining length to be sent
	remaining_length = length-current_written_length;

	LLNET_STREAMSOCKETCHANNEL_write(fd, src+offset, current_written_length, remaining_length);
}

static void LLNET_STREAMSOCKETCHANNEL_write(int32_t fd, int8_t* buffer, int32_t current_written_length, int32_t remaining_length){

	int32_t fd_errno;
	int32_t ret = llnet_send(fd, buffer+current_written_length, remaining_length, 0);

    if(ret == 0){
    	//should not happen: 0 byte written
    	SNI_throwNativeIOException(J_EUNKNOWN, "0 byte written");
    	return;
    }

    if(ret > 0){
		if(ret == remaining_length){
			//successful write: all bytes have been sent
			return;
		}
		//here, written length is less than the requested length.
		//we need to call write again to send the remaining data.
		//update the current written length and simulate an EAGAIN error to be able to call write again from the SNI callback
		current_written_length += ret;
		fd_errno = EAGAIN;
    }else{
    	fd_errno = llnet_errno(fd);
    }

    LLNET_handle_blocking_operation_error(fd, fd_errno, SELECT_WRITE, 0, (SNI_callback)LLNET_STREAMSOCKETCHANNEL_write_callback, (void*)current_written_length);
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_read(int32_t fd, int8_t* dst, int32_t offset, int32_t length, int64_t absoluteTimeout)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
    	SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
        return SNI_IGNORED_RETURNED_VALUE;
    }

	int32_t ret = llnet_recv(fd, (void*)(dst+offset), length, 0);
	LLNET_DEBUG_TRACE("%s: result=%d errno=%d\n", __func__, ret,llnet_errno(fd));

	if(ret > 0){
		return ret;
	}

	if(0 == ret){
		//EOF
		return -1;
	}

	//read error
	LLNET_handle_blocking_operation_error(fd, llnet_errno(fd), SELECT_READ, absoluteTimeout, (SNI_callback)LLNET_STREAMSOCKETCHANNEL_IMPL_read, NULL);
	return SNI_IGNORED_RETURNED_VALUE;
}


void LLNET_STREAMSOCKETCHANNEL_IMPL_write(int32_t fd, int8_t* src, int32_t offset, int32_t length)
{
	LLNET_DEBUG_TRACE("%s[thread %d]\n", __func__, SNI_getCurrentJavaThreadID());
    if(llnet_is_ready() == false){
    	SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
        return;
    }

    LLNET_STREAMSOCKETCHANNEL_write(fd, src+offset, 0, length);
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_available(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

	int32_t fd_errno;

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

#if LLNET_AVAILABLE_IMPL_ALT == LLNET_USE_MSG_PEEK_FOR_AVAILABLE
	int32_t size = llnet_recv(fd, LLNET_MSG_PEEK_AVAILABLE_BUFFER, sizeof(LLNET_MSG_PEEK_AVAILABLE_BUFFER), MSG_PEEK | MSG_DONTWAIT);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X) size=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, size, llnet_errno(fd));
	if(size >= 0){
		return size;
	}
	fd_errno = llnet_errno(fd);
	if(EAGAIN == fd_errno){
		return 0;
	}

	SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	return SNI_IGNORED_RETURNED_VALUE;

#elif LLNET_AVAILABLE_IMPL_ALT == LLNET_USE_SOCK_OPTION_FOR_AVAILABLE
	int32_t size;
	int32_t optlen = sizeof(int32_t);
	int32_t res = llnet_getsockopt (fd, SOL_SOCKET, SO_RXDATA, &size, (socklen_t *)&optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X) size=%d errno=%d res=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, size, llnet_errno(fd), res);
	if(res == 0){
		return size;
	}
	fd_errno = llnet_errno(fd);
	SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	return SNI_IGNORED_RETURNED_VALUE;
#else
	int32_t size;
	int32_t res = llnet_ioctl(fd, FIONREAD, &size);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X) size=%d errno=%d res=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, size, llnet_errno(fd), res);
	if(res == 0){
		return size;
	}
	// error on ioctl
	fd_errno = llnet_errno(fd);
	SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	return SNI_IGNORED_RETURNED_VALUE;
#endif // LLNET_AVAILABLE_IMPL_ALT == LLNET_USE_MSG_PEEK_FOR_AVAILABLE
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_accept(int32_t fd, int64_t absoluteTimeout)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }
    int32_t ret;
    int32_t fd_errno;
    ret = llnet_accept(fd, NULL, NULL);

    if(0 > ret){
    	//error
    	fd_errno = llnet_errno(fd);
    	LLNET_handle_blocking_operation_error(fd, fd_errno, SELECT_READ, absoluteTimeout, (SNI_callback)LLNET_STREAMSOCKETCHANNEL_IMPL_accept, NULL);
    	return SNI_IGNORED_RETURNED_VALUE;
    }

    int32_t client_socket_fd = ret;
    //set the accepted socket in non blocking mode
	ret = LLNET_set_non_blocking(client_socket_fd);

	//set the socket in non blocking mode
	if(0 != ret){
		fd_errno = llnet_errno(client_socket_fd);
		LLNET_DEBUG_TRACE("%s: set_non_blocking errno=%d, err=%d\n", __func__, fd_errno, ret);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		close(client_socket_fd);
		return SNI_IGNORED_RETURNED_VALUE;
	}
	return client_socket_fd;
}

#ifdef __cplusplus
	}
#endif
