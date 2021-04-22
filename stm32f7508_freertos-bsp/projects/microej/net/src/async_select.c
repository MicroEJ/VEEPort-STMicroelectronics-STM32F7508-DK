/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief Asynchronous network select implementation
 * @author MicroEJ Developer Team
 * @version 1.0.1
 * @date 19 February 2020
 */

#include "async_select.h"
#include "async_select_configuration.h"
#include "async_select_cache.h"
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Sanity check between the expected version of the configuration and the actual version of
 * the configuration.
 * If an error is raised here, it means that a new version of the CCO has been installed and
 * the configuration async_select_configuration.h must be updated based on the one provided
 * by the new CCO version.
 */
#if ASYNC_SELECT_CONFIGURATION_VERSION != 2

	#error "Version of the configuration file async_select_configuration.h is not compatible with this implementation."

#endif

/** @brief  An asynchronous select request */
typedef struct async_select_Request{
	int32_t fd;
	int32_t java_thread_id;
	// Absolute time for timeout in milliseconds, 0 if no timeout
	int64_t absolute_timeout_ms;
	SELECT_Operation operation;
	struct async_select_Request* next;
} async_select_Request;

/**
 * @brief Enter critical section for the async_select component.
 */
extern void async_select_lock(void);
/**
 * @brief Exit critical section for the async_select component.
 */
extern void async_select_unlock(void);

/**
 * @brief External function used to retrieve currentTime (defined in LLMJVM)
 */
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);

/**
 * Gets the system time in milliseconds (int64_t). This time is independent from any user considerations
 * and cannot be changed.
 */
#define async_select_get_current_time_ms()	LLMJVM_IMPL_getCurrentTime__Z(1) // 1 means that system time is required


/*
 * See implementations for descriptions.
 */
static void async_select_do_select(void);
static void async_select_update_notified_requests(void);
static int32_t async_select_get_notify_fd(void);
static async_select_Request* async_select_allocate_request(void);
static async_select_Request* async_select_free_used_request(async_select_Request* request, async_select_Request* previous_request_in_used_fifo);
static void async_select_free_unused_request(async_select_Request* request);
static int32_t async_select_send_new_request(async_select_Request* request);
static void async_select_notify_select(void);
void async_select_request_fifo_init(void);

/**
 * @brief Pool of requests. Used to reserve MAX_NB_ASYNC_SELECT async select requests.
 */
static async_select_Request all_requests[MAX_NB_ASYNC_SELECT];
/**
 * @brief Linked-list of free requests that can be allocated using async_select_allocate_request().
 */
static async_select_Request* free_requests_fifo;
/**
 * @brief Linked-list of used requests.
 */
static async_select_Request* used_requests_fifo;
/**
 * @brief File descriptor set for SELECT_READ requests.
 */
static fd_set read_fds;
/**
 * @brief File descriptor set for SELECT_WRITE requests.
 */
static fd_set write_fds;
/**
 * @brief Used to unblock select() function call.
 */

#ifdef ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION

static int8_t pipe_fds_initialized = 0;
static int32_t pipe_fds[2];

#else

volatile static int32_t notify_fd_cache = -1;

#endif //ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION

/**
 * @brief set to one once the FIFOs are initialized.
 */
volatile static uint8_t async_select_fifo_initialized = 0;

/**
 * @brief Execute a select() for the given file descriptor and operation without blocking.
 *
 * @param fd the file descriptor.
 * @param operation the operation (read or write) we want to monitor with the select().
 *
 * @return select() result.
 */
int32_t non_blocking_select(int32_t fd, SELECT_Operation operation){
	struct timeval zero_timeout;
	zero_timeout.tv_sec = 0;
	zero_timeout.tv_usec = 0;
	fd_set io_fds;
	fd_set* read_fds_ptr;
	fd_set* write_fds_ptr;
	FD_ZERO(&io_fds);
	FD_SET(fd, &io_fds);

	if(operation == SELECT_READ){
		read_fds_ptr = &io_fds;
		write_fds_ptr = NULL;
	}
	else { // SELECT_WRITE
		read_fds_ptr = NULL;
		write_fds_ptr = &io_fds;
	}

	int32_t selectRes = select(fd+1, read_fds_ptr, write_fds_ptr, NULL, &zero_timeout);

	return selectRes;
}

/**
 * @brief Executes asynchronously a select() operation for the given file descriptor.
 *
 * This function will suspend the execution of the current Java thread using
 * SNI_suspendCurrentJavaThreadWithCallback(). Once the select() succeeds the Java
 * thread is resumed and the given SNI callback is called. *
 *
 * @param fd the file descriptor.
 * @param operation the operation (read or write) we want to monitor with the select().
 * @param timeout_ms timeout in millisecond
 * @param the SNI callback to call when the Java thread is resumed or timeout occurs.
 *
 * @return 0 on success, -1 on failure.
 */
int32_t async_select(int32_t fd, SELECT_Operation operation, int64_t timeout_ms, SNI_callback callback){

	int32_t res;

	async_select_Request* request = async_select_allocate_request();
	if(request == NULL){
		// No request available :-(
		return -1;
	}

	int32_t java_thread_id = SNI_getCurrentJavaThreadID();
	if(java_thread_id == SNI_ERROR){
		// Not called from the VM task
		async_select_free_unused_request(request);
		return -1;
	}

	LLNET_DEBUG_TRACE("async_select: async_select on fd=0x%X operation=%s thread 0x%X\n", fd, operation==SELECT_READ ? "read":"write", java_thread_id);
	request->java_thread_id = java_thread_id;
	request->fd = fd;
	request->operation = operation;
	if(timeout_ms != 0){
		request->absolute_timeout_ms = async_select_get_current_time_ms() + timeout_ms;
	}
	else { // infinite timeout
		request->absolute_timeout_ms = 0;
	}

	async_select_set_socket_absolute_timeout_in_cache(fd, request->absolute_timeout_ms);

	SNI_suspendCurrentJavaThreadWithCallback(0, callback, NULL);
	res = async_select_send_new_request(request);

	return res;
}

/**
 * @brief Initializes the requests FIFOs.
 * This function must be called prior to any call of async_select().
 * It can be called several times.
 */
void async_select_request_fifo_init(){
	// Init free requests FIFO
	async_select_lock();
	if(async_select_fifo_initialized == 0){
		free_requests_fifo = &all_requests[0];
		for(int i=0 ; i<MAX_NB_ASYNC_SELECT-1 ; i++){
			all_requests[i].next = &all_requests[i+1];
		}
		all_requests[MAX_NB_ASYNC_SELECT-1].next = NULL;

		// Init used requests FIFO
		used_requests_fifo = NULL;
		async_select_fifo_initialized = 1;
	}
	async_select_unlock();
}


/**
 * @brief Notifies the async_select task that a file descriptor has been closed.
 * On some systems the close of a file descriptor does not unblock the select that's
 * why we need to notify the async_select task.
 */
void async_select_notify_closed_fd(int32_t fd){

// If the close unblock the select we don't need to do anything here
#ifndef ASYNC_SELECT_CLOSE_UNBLOCK_SELECT

	// Search for the file descriptor in the used requests FIFO.
	// For the requests that match the given fd, set the timeout
	async_select_lock();

	async_select_Request* request = used_requests_fifo;
	while(request != NULL){
		if(request->fd == fd){
			// Modify timeout value so that when the task will check this request
			// it will detect a timeout.
			request->absolute_timeout_ms = 1;
		}
		request = request->next;
	}

	async_select_unlock();

	async_select_notify_select();

#endif	//ASYNC_SELECT_CLOSE_UNBLOCK_SELECT

}

/**
 * @brief The entry point for the async_select task.
 * This function must be called from a dedicated task.
 */
void async_select_task_main(){

	llnet_init();

	async_select_request_fifo_init();

	while(true){
		// Execute a select().
		async_select_do_select();
		// Update the received request depending on the select() results.
		async_select_update_notified_requests();
	}
}

/**
 * @brief Returns the file descriptor created just to unlock the select() when
 * we want to notify the async_select task that a new request has been
 * sent.
 */
static int32_t async_select_get_notify_fd(){


#ifdef ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION

	if(pipe_fds_initialized == 0){
		if(pipe(pipe_fds) == -1 ||
		   set_socket_non_blocking(pipe_fds[0], true) != 0 ||
		   set_socket_non_blocking(pipe_fds[1], true) != 0){
			//error : can not create the pipe
			return -1;
		}
		pipe_fds_initialized = 1;
	}
	return pipe_fds[0];

#else

	// Just take and release the lock to ensure that the VM task has not been preempted in the
	// middle of async_select_notify_select() (i.e. notify_fd_cache has been set to -1 but socket
	// has not been closed yet).
	async_select_lock();
	int32_t notify_fd = notify_fd_cache;
	async_select_unlock();
	if(notify_fd != -1){
		// fd already exists
		return notify_fd;
	}

	int domain;

// If IPv6 or IPv4+IPv6 configuration, then use IPv6. Otherwise (only IPv4 configuration) use IPv4.
#if LLNET_AF & LLNET_AF_IPV6
	domain = AF_INET6;
#else // only IPv4
	domain = AF_INET;
#endif
	// Create a simple local TCP server to wait on and call close when we want to unblock the select
	notify_fd = llnet_socket(domain, SOCK_STREAM, IPPROTO_TCP);
	if(notify_fd != -1){


#if LLNET_AF & LLNET_AF_IPV6
		struct sockaddr_in6 sockaddr = {0};
		sockaddr.sin6_family = AF_INET6;
		sockaddr.sin6_port = llnet_htons(0);
		sockaddr.sin6_addr = in6addr_loopback;
#else
		struct sockaddr_in sockaddr = {0};
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(0);
		sockaddr.sin_addr.s_addr = INADDR_LOOPBACK;
#endif
		int32_t ret = llnet_bind(notify_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
		if(ret != -1){
			ret = llnet_listen(notify_fd, 1);
			if(ret != -1){
				// Save it for next time to avoid a new creation.
				notify_fd_cache = notify_fd;
				return notify_fd;
			}
		}

		// Something was wrong when configuring the notify_fd
		llnet_close(notify_fd);
	}

	return -1;

#endif // ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION
}

/**
 * @brief Executes the select() operation for the file descriptors referenced by the received requests.
 */
static void async_select_do_select(){

	async_select_Request* request;

	int32_t notify_fd = async_select_get_notify_fd();
	// Used to save the highest fd found in the requests.
	int32_t max_request_fd = notify_fd;
	// Used to save the lower timeout found in the requests.
	int64_t min_absolute_timeout_ms = INT64_MAX;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);

	// add the notify file descriptor to the read select list
	if(notify_fd != -1){
		FD_SET(notify_fd, &read_fds);
	}
	else {
		// We were not able to create the socket to unlock the select.
		// To prevent an infinite lock of the select we will poll for
		// incoming messages by setting a timeout to the select.
		min_absolute_timeout_ms = async_select_get_current_time_ms() + ASYNC_SELECT_POLLING_MODE_TIMEOUT_MS;
		LLNET_DEBUG_TRACE("async_select: WARNING: notify_fd cannot be allocated, fall back in polling mode\n");
	}

	// -----------------------------------------------------------------
	// Add read/write waiting operations in file descriptors select list
	// -----------------------------------------------------------------
	request = used_requests_fifo;
	while(request != NULL){
		int32_t request_fd = request->fd;
		if(request_fd > max_request_fd){
			// Save the highest fd
			max_request_fd = request_fd;
		}

		int64_t request_absolute_timeout_ms = request->absolute_timeout_ms;
		if(request_absolute_timeout_ms != 0 && request_absolute_timeout_ms < min_absolute_timeout_ms){
			// Save the lowest timeout
			min_absolute_timeout_ms = request_absolute_timeout_ms;
		}

		if(request->operation == SELECT_READ){
			FD_SET(request_fd, &read_fds);
		}
		else { // operation == SELECT_WRITE
			FD_SET(request_fd, &write_fds);
		}

		request = request->next;
	}

	// -----------------------------
	//  Compute select timeout value
	// -----------------------------

	// Timeout to use for the select
	struct timeval select_timeout = {0};
	struct timeval* select_timeout_ptr;

	if(min_absolute_timeout_ms != INT64_MAX){
		// At least one request has a timeout.
		select_timeout_ptr = &select_timeout;
		int64_t min_relative_timeout_ms = min_absolute_timeout_ms - async_select_get_current_time_ms();
		// Saturate the relative timeout to a positive value
		if(min_relative_timeout_ms < 0){
			// 0 means no timeout
			min_relative_timeout_ms = 0;
		}
		time_ms_to_timeval(min_relative_timeout_ms, select_timeout_ptr);
	}
	else {
		// No request has timeout -> NULL timeout means infinite timeout
		select_timeout_ptr = NULL;
	}


	// --------------
	//  Do the select
	// --------------
	LLNET_DEBUG_TRACE("async_select: select (timeout sec=%d usec=%d)\n", (int32_t)select_timeout.tv_sec, (int32_t)select_timeout.tv_usec);
	int32_t res = select(max_request_fd+1, &read_fds, &write_fds, NULL, select_timeout_ptr);

	if(res >= 0 || llnet_errno(-1) == EBADF){
		//errno == EBADF when one of fd in the fdset is invalid/closed
		//We consider that the select was succeeded in this case
		//because all operations through an invalid/closed fd would not block

#ifdef ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION
		//check if notify_fd is selected and cleanup the pipe
		if(FD_ISSET(notify_fd, &read_fds)){
			//cleanup pipe
			char bytes[1];
			while(read(notify_fd, (void*)bytes, 1) > 0); //non blocking pipe fds
		}
#endif

		LLNET_DEBUG_TRACE("async_select: select finished %d sockets available\n", res);
	}
}

/**
 * @brief After the execution of the select() operation, update the status of the requests
 * that have been notified by the select() or have reached the timemout.
 */
static void async_select_update_notified_requests(){

	async_select_Request* request;
	async_select_Request* previous_request = NULL;
	int64_t current_time_ms = async_select_get_current_time_ms();

	async_select_lock();
	// Browse all the requests to find which have been modified
	request = used_requests_fifo;
	while(request != NULL){
		int32_t request_fd = request->fd;
		bool request_timeout_reached;

		// Check if timeout has been reached.
		if(request->absolute_timeout_ms != 0 && request->absolute_timeout_ms <= current_time_ms){
			request_timeout_reached = true;
		}
		else {
			request_timeout_reached = false;
		}

		if((request->operation == SELECT_READ && FD_ISSET(request_fd, &read_fds))	// data received
		|| (request->operation == SELECT_WRITE && FD_ISSET(request_fd, &write_fds))	// or data sent
		|| (request_timeout_reached)												// or timeout reached
		){
			// Request done.
			LLNET_DEBUG_TRACE("async_select: request done for fd=0x%X operation=%s notify thread 0x%X (%s)\n", request_fd, request->operation==SELECT_READ ? "read":"write", request->java_thread_id, request_timeout_reached==true ? "timeout":"no timeout");
			SNI_resumeJavaThread(request->java_thread_id);
			request = async_select_free_used_request(request, previous_request);
			//previous_request is still the same because we have removed request from the used FIFO
		}
		else {
			previous_request = request;
			request = request->next;
		}
	}
	async_select_unlock();
}

/**
 * @brief Remove the given request from the used FIFO and put it in the free FIFO.
 *
 * This function is NOT thread safe.
 *
 * @return the next request in the used FIFO.
 */
static async_select_Request* async_select_free_used_request(async_select_Request* request, async_select_Request* previous_request_in_used_fifo){

	async_select_Request* next_request;

	next_request = request->next;

	// Remove the request from the used FIFO
	if(previous_request_in_used_fifo != NULL){
		previous_request_in_used_fifo->next = next_request;
	}
	else{
		// The request was the first in the used list
		used_requests_fifo = next_request;
	}

	// Add the request into the free FIFO
	request->next = free_requests_fifo;
	free_requests_fifo = request;

	return next_request;
}

/**
 * @brief Put the given request in the free FIFO.
 * The request must not be in the used FIFO.
 *
 * This function is thread safe.
 */
static void async_select_free_unused_request(async_select_Request* request){

	async_select_lock();

	// Add the request into the free FIFO
	request->next = free_requests_fifo;
	free_requests_fifo = request;

	async_select_unlock();
}

/**
 * @brief Notifies the async_select task that a new request must be managed.
 */
static int32_t async_select_send_new_request(async_select_Request* request){

	async_select_lock();
	// Add the request in the used FIFO
	request->next = used_requests_fifo;
	used_requests_fifo = request;
	async_select_unlock();

	// Notify the async_select task
	async_select_notify_select();

	return 0;
}

/**
 * @brief Find a free request and returns it.
 * The returned request is not put it in the used requests FIFO.
 * It must be either put in the used requests FIFO using async_select_send_new_request()
 * or put back in the free requests FIFO on error using async_select_free_unused_request().
 *
 * This function is thread safe.
 *
 * @return null if no request available.
 */
static async_select_Request* async_select_allocate_request(){

	async_select_lock();

	async_select_Request* new_request = free_requests_fifo;
	if(new_request != NULL){
		// Remove the request from the free FIFO
		free_requests_fifo = new_request->next;
	}
	// else: no request available

	async_select_unlock();

	return new_request;
}

/**
 * @brief Unlock the select operation.
 *
 * @return 0 on success, a negative value on error.
 */
static void async_select_notify_select(){

	int32_t res = 0;
	int32_t notify_fd;

#ifdef ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION

	if(pipe_fds_initialized == 1){
		//Write through the pipe to cancel the current (or the next) blocking select operation.
		char bytes[1] = {1};
		notify_fd = pipe_fds[1];
		res = write(notify_fd, (void*)bytes, 1); //pipe_fds[1] refers to the write end of the pipe.
	}
	else {
		notify_fd = -1;
	}

#else

	async_select_lock();

	notify_fd = notify_fd_cache;
	if(notify_fd != -1){
		// async_select task is blocked on select operation. Call
		// close function on notify_fd socket to unblock the select.
		// WARNING: these two operations must be atomic because we don't want
		// the async_selec task to create a new socket while we have not closed
		// this one.
		notify_fd_cache = -1;
		res = llnet_close(notify_fd);
	}
	// else:
	// The notify_fd_cache is not yet defined. It is not a problem because the
	// select task will browse the requests list later (after notify_fd_cache creation).
	// and so manage the request modification.

	async_select_unlock();


#endif // ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION

	if(res == -1){
		LLNET_DEBUG_TRACE("Error on notify select (notify_fd: 0x%X errno: %d)\n", notify_fd, llnet_errno(notify_fd));
	}
}

#ifdef __cplusplus
	}
#endif
