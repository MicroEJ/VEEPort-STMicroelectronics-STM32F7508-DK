/*
 * C
 *
 * Copyright 2015-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef MJVM_MONITOR_TYPES_H
#define MJVM_MONITOR_TYPES_H

/**
 * @file
 * @brief MicroEJ MicroJvm virtual machine monitoring library types API
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 1 June 2017
 */

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Returned value when function call has succeeded.
 */
#define MJVM_MONITOR_OK (0)

/**
 * Returned value when function call has failed.
 */
#define MJVM_MONITOR_ERROR (-1)

/**
 * Returned value when an invalid MJVM_MONITOR_ThreadInfo struct is given to a function.
 */
#define MJVM_MONITOR_INVALID_THREAD_INFO (-2)

/**
 * Returned value when an invalid Java thread ID is given to a function.
 */
#define MJVM_MONITOR_INVALID_THREAD_ID (-3)

/*
 * Specifies the states of the Java threads.
 */
typedef enum
{
	/**
	 * In the ready state, all functional prerequisites for a transition into the
	 * running state exist, and the thread only waits for allocation of the processor.
	 * The scheduler decides which ready thread is executed next.
	 */
	MJVM_MONITOR_STATE_READY = 1,

	/**
	 * In the running state, the CPU is assigned to the thread, so that its instructions can be
	 * executed. Only one thread can be in this state at any point in time, while all the other states can
	 * be adopted simultaneously by several threads.
	 */
	MJVM_MONITOR_STATE_RUNNING = 2,

	/**
	 * Thread state for a waiting thread. A thread is in the waiting state due to calling one of the following
	 * methods:
	 * <ul>
	 * <li><code>Thread.sleep</code></li>
	 * <li><code>Object.wait</code></li>
	 * <li><code>Thread.join</code></li>
	 * <li><code>SNI_suspendCurrentJavaThread</code></li>
	 * </ul>
	 */
	MJVM_MONITOR_STATE_WAITING = 3,

	/**
	 * In the monitor queued state, a thread cannot continue execution because a monitor it tries
	 * to acquire is already owned by another thread. When this monitor is released, it will try to gain
	 * the ownership of the monitor. If it succeeds, it will be transferred into the ready state,
	 * otherwise it will remain in the monitor queued state.
	 */
	MJVM_MONITOR_STATE_MONITOR_QUEUED = 4,

	/**
	 * In the terminated state the thread is passive and has completed its execution.
	 */
	MJVM_MONITOR_STATE_TERMINATED = 6,

}MJVM_MONITOR_state_t;

/*
 * Structure that contains information about a thread.
 */
typedef struct {
	/* ID of the thread. */
	int32_t id;
	/* Java priority of the thread. */
	int32_t priority;
	/* Current state of the thread. */
	MJVM_MONITOR_state_t state;
	/* Maximum Java stack usage in bytes. -1 if not available. */
	int32_t max_stack_usage;
} MJVM_MONITOR_thread_info_t;

#ifdef __cplusplus
	}
#endif
#endif /* MJVM_MONITOR_TYPES_H */
