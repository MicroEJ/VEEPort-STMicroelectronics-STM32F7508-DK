/*
 * C
 *
 * Copyright 2008-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * Header file for Simple Native Interface (SNI), version 1.4
 */
#ifndef SNI_H
#define SNI_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
	extern "C" {
#endif

// Version of SNI under the form 0xMMmmPP
// MM: major version
// mm: minor version
// PP: patch version
#define SNI_VERSION	(0x010400)

typedef int8_t			jbyte;    /* signed 8 bits  */
typedef uint8_t         jboolean; /* boolean 8 bits  */
typedef uint16_t        jchar;    /* unsigned 16 bits */
typedef int16_t         jshort;   /* signed 16 bits */
typedef int32_t         jint;     /* signed 32 bits */
typedef float           jfloat;   /* float 32 bits */
typedef double          jdouble;  /* float 64 bits */
typedef int64_t         jlong;    /* signed 64 bits */

// Boolean values
#define JTRUE	(1)
#define JFALSE	(0)

// Null reference
#define JNULL   (0)

// Constant that can be used to indicate that the value returned by a native
// won't be used because an SNI callback will be executed later.
#define SNI_IGNORED_RETURNED_VALUE (0)

// Function succeeded
#define SNI_OK			(0)
// An error was detected
#define SNI_ERROR 		(-1)
// An illegal argument has been given
#define SNI_ILLEGAL_ARGUMENT (-2)

// Returns the length of a Java array (number of elements)
#define SNI_getArrayLength(array) (*((jint*)(((uint8_t*)(array))-sizeof(jint))))



/**
 * Checks if the given Java array is immortal.
 *
 * An immortal Java array remains at a fixed memory location and will not be moved or changed by
 * Java's garbage collection routine.

 * An immortal Java array can be used safely from another native task, an interrupt
 * handler or can be used as a DMA buffer.
 *
 * A non-immortal Java array can be used only during the execution of an SNI native or an
 * SNI callback.
 *
 * A reference to a non-immortal Java array must not be kept in C between two SNI native executions.
 *
 * @param javaArray is a Java array retrieved from an SNI native
 * arguments.
 *
 * @return <code>true</code> if the given Java array is immortal or null, otherwise
 * returns <code>false</code>.
 */
bool SNI_isImmortalArray(void* javaArray);

/**
 * Throws a new <code>ej.sni.NativeException</code> after the end of the current native
 * method or SNI callback. <code>NativeException</code> class is a subclass of
 * <code>java.lang.RuntimeException</code>. <code>NativeException</code> instances are
 * unchecked exceptions, they do not need to be declared in the native method throws
 * clause.
 *
 * The virtual machine keeps a reference to the message until the end of the current native
 * and does a copy of the message only when the current native returns. The message pointer
 * must not reference a memory allocated locally in the stack.
 *
 * If the current Java thread is suspended this function returns <code>SNI_ERROR</code>
 * and no exception is thrown.
 *
 * Calling this function while an exception is already pending will replace the previous
 * exception.
 *
 * If there is not enough space in the Java heap to allocate the exception, then an
 * <code>OutOfMemoryError</code> will be thrown in Java.
 *
 * @param errorCode value that can be retrieved in Java using the
 * <code>NativeException.getErrorCode()</code> method.
 *
 * @param message is a null-terminated string that will be used to
 * generate the message returned by <code>NativeException.getMessage()</code>. May be
 * null.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if this function
 * is not called within the virtual machine task or if the current thread is suspended.
 */
int32_t SNI_throwNativeException(int32_t errorCode, const char* message);

/**
 * Throws a new <code>ej.sni.NativeIOException</code> after the end of the current native
 * method or SNI callback. <code>NativeIOException</code> class is a subclass of
 * <code>java.io.IOException</code>. <code>NativeIOException</code> instances are checked
 * exceptions, they need to be declared in the native method throws clause (e.g.
 * <code>throws IOException</code>).
 *
 * If the native method declaration is not compatible with <code>NativeIOException</code>
 * (i.e. the throws clause does not specify <code>NativeIOException</code> or one of its
 * superclasses) then a code>NativeException</code> is thrown instead.
 *
 * The virtual machine keeps a reference to the message until the end of the current native
 * and does a copy of the message only when the current native returns. The message pointer
 * must not reference a memory allocated locally in the stack.
 *
 * If the current Java thread is suspended this function returns <code>SNI_ERROR</code>
 * and no exception is thrown.
 *
 * Calling this function while an exception is already pending will replace the previous
 * exception.
 *
 * If there is not enough space in the Java heap to allocate the exception, then an
 * <code>OutOfMemoryError</code> will be thrown in Java.
 *
 * @param errorCode is a value that can be retrieved in Java using the
 * <code>NativeIOException.getErrorCode()</code> method.
 *
 * @param message a null-terminated string that will be used to generate the message
 * returned by <code>NativeIOException.getMessage()</code> method. May be null.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if this function
 * is not called within the virtual machine task or if the current thread is suspended.
 */
int32_t SNI_throwNativeIOException(int32_t errorCode, const char* message);

/**
 * Checks if an exception is pending in the current Java thread (i.e. a successful call to
 * <code>SNI_throwNativeException</code> or <code>SNI_throwNativeIOException</code> has
 * been made prior to this call).
 *
 * @return <code>true</code> if an exception is pending in the current Java thread,
 * returns <code>false</code> otherwise.
 */
bool SNI_isExceptionPending(void);

/**
 * Clears any pending exception for the current Java thread.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if the function
 * is not called within the virtual machine task.
 */
int32_t SNI_clearPendingException(void);

/**
 * Pointer to a function that closes the given native resource.
 *
 * @param resource the native resource to close.
 */
typedef void (*SNI_closeFunction)(void* resource);

/**
 * Pointer to a function that gets the description of the given native resource.
 *
 * The function stores the description string in the user-supplied buffer <code>buffer</code> of length
 * <code>bufferLength</code>.
 * At most <code>bufferLength</code> bytes are stored (the string may be truncated if <code>bufferLength</code>
 * is too small). The string always includes a terminating null byte ('\0').
 *
 * @param resource the native resource to describe.
 *
 * @param buffer the buffer where the description is stored.
 *
 * @param bufferLength the length of the given buffer.
 */
typedef void (*SNI_getDescriptionFunction)(void* resource, char* buffer, uint32_t bufferLength);

/**
 * This function must be called when a native resource (file, socket, ...) has been created to register
 * it in the virtual machine.
 *
 * The virtual machine keeps track of the native resources to prevent any memory leak when the virtual
 * machine or an application is stopped.
 *
 * In a Multi-Sandbox environment with KF profile enabled, the given <code>close</code> function is
 * called when the application that has created the resource is stopped. If the resource has been created
 * by the Kernel or if KF profile is disabled, the <code>close</code> function is called when the virtual
 * machine stops.
 *
 * To unregister the given native resource, call <code>SNI_unregisterResource()</code>.
 *
 * A native resource is uniquely identified by the pair <code>resource</code>, <code>close</code>. Several
 * native resources with the same <code>resource</code> value can be registered as long as <code>close</code>
 * is different.
 *
 * If a native resource is registered several times with the same <code>resource</code> and <code>close</code>
 * values, this function returns <code>SNI_ILLEGAL_ARGUMENT</code> (unless it has been unregistered before a
 * second registration).
 *
 * This function can be called once per native call. Calling this function several times within the same
 * native will return <code>SNI_ERROR</code>.
 *
 * If an error occurs while registering the resource at the end of the native, then the <code>close</code>
 * function is called and an exception is thrown when returning in Java.
 *
 * @param resource the created native resource to register.
 *
 * @param close the function called if the virtual machine wants to automatically close the resource.
 *
 * @param getDescription the function called if the virtual machine wants to retrieve a description of the
 * native resource. This pointer function may be null.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ILLEGAL_ARGUMENT</code> if <code>close</code> is
 * null or if the given native resource has already been registered. Returns <code>SNI_ERROR</code> if the
 * function is not called within the virtual machine task or if this function has been called several times
 * in the same native.
 */
int32_t SNI_registerResource(void* resource, SNI_closeFunction close, SNI_getDescriptionFunction getDescription);

/**
 * This function must be called when a native resource previously registered with <code>SNI_registerResource()</code>
 * has been reclaimed. It will unregister the given native resource so the virtual machine won't call later
 * the <code>close</code> function.
 *
 * This method does not call the <code>close</code> function. It is up to the caller to call the appropriate
 * function to close the resource.
 *
 * The <code>close</code> parameter is used to uniquely identify the registered resource. If the given native
 * resource is not found,  this function returns <code>SNI_ILLEGAL_ARGUMENT</code>.
 *
 * @param resource a resource previously registered with <code>SNI_registerResource()</code>
 *
 * @param close the <code>close</code> argument given to <code>SNI_registerResource()</code> when the native
 * resource has been registered.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ILLEGAL_ARGUMENT</code> if the given native resource
 * is not registered. Returns <code>SNI_ERROR</code> if the function is not called within the virtual machine task.
 */
int32_t SNI_unregisterResource(void* resource, SNI_closeFunction close);

/**
 * Registers a scoped native resource.
 *
 * These native resources are similar to standard native resources except they consume less memory, they are
 * faster and they are closed automatically when the native returns back to Java. This function should be used
 * when a resource is allocated in a native and released in a callback of the same native. This will prevent
 * memory leaks that can occur when a thread is suspended using <code>SNI_suspendCurrentJavaThreadWithCallback()</code>
 * and the virtual machine or the current application is stopped.
 *
 * In a Multi-Sandbox environment with KF profile enabled, the given <code>close</code> function is also
 * called when the application that has created the scoped native resource is stopped. If the scoped native
 * resource has been created by the Kernel or if KF profile is disabled, the <code>close</code> function is called when
 * the virtual machine stops.
 *
 * To unregister the given scoped native resource, call <code>SNI_unregisterScopedResource()</code>.

 * If several scoped native resources are registered within the same native context (or subsequent callback),
 * then this function returns <code>SNI_ERROR</code> (unless the previously registered scoped native resource
 * has been unregistered).
 *
 * @param resource the created scoped native resource to register.
 *
 * @param close the function called when the native returns back to Java.
 *
 * @param getDescription the function called if the virtual machine wants to retrieve a description of the
 * scoped native resource. This pointer function may be null.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ILLEGAL_ARGUMENT</code> if <code>close</code> is
 * null. Returns <code>SNI_ERROR</code> if the function is not called within the virtual machine task or if this
 * function has been called several time in the same native context.
 */
int32_t SNI_registerScopedResource(void* resource, SNI_closeFunction close, SNI_getDescriptionFunction getDescription);

/**
 * Unregisters the scoped native resource previously registered with <code>SNI_registerScopedResource()</code>.
 *
 * After a call to this function, a new scoped native resource can be registered.
 *
 * Usually this function does not need to be called because the registered scoped native resource is automatically
 * closed and unregistered when the native returns back to Java.
 *
 * This method does not call the <code>close</code> function given previously to <code>SNI_registerScopedResource()</code>.
 * It is up to the caller to call the appropriate function to close the resource.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if the function is not called within
 * the virtual machine task or if no scoped native resource has been previously registered.
 */
int32_t SNI_unregisterScopedResource(void);

/**
 * Gets the information of the scoped native resource previously registered with <code>SNI_registerScopedResource()</code>.
 *
 * @param resourcePtr output parameter. This function will store in this pointer the <code>resource</code>
 * value previously given to <code>SNI_registerScopedResource()</code>. May be null.
 *
 * @param closePtr output parameter. This function will store in this pointer the <code>close</code>
 * value previously given to <code>SNI_registerScopedResource()</code>. May be null.
 *
 * @param getDescriptionPtr output parameter. This function will store in this pointer the <code>getDescription</code>
 * value previously given to <code>SNI_registerScopedResource()</code>. May be null.

 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if the function is not called within
 * the virtual machine task or if no scoped native resource has been previously registered.
 */
int32_t SNI_getScopedResource(void** resourcePtr, SNI_closeFunction* closePtr, SNI_getDescriptionFunction* getDescriptionPtr);

/**
 * Gets the body of a Java byte array so it can be used outside of the scope of the current native function.
 * <p>
 * If the given Java array is an immortal array, then <code>out_buffer</code> is set to reference directly the
 * element at the offset <code>java_start</code> in the given Java array and <code>out_length</code> is set to
 * <code>java_length</code>.
 * If the given Java array is not an immortal array, then <code>out_buffer</code> is set to reference
 * <code>buffer</code>, <code>*out_length</code> is set to the smaller value of <code>java_length</code>
 * and <code>buffer_length</code>, and then, if <code>refresh_content</code> is true, <code>*out_length</code>
 * bytes are copied from the content of the Java array into <code>buffer</code>.
 * <p>
 * Since the returned array may be a copy of the Java array, changes made to the returned array will not necessarily
 * be reflected in the original Java array until <code>SNI_flushArrayElements()</code> is called.
 * <p>
 * The function <code>SNI_flushArrayElements()</code> doesn't have to be called if changes made to the returned
 * array don't need to be reflected in the original Java array.
 *
 * @param java_array the Java array.
 *
 * @param java_start the start offset in array <code>java_array</code> of the first element to get.
 *
 * @param java_length the maximum number of bytes to get.
 *
 * @param buffer the buffer to use if the given Java array is not immortal.
 *
 * @param buffer_length length in bytes of <code>buffer</code>.
 *
 * @param out_buffer output pointer to the retrieved elements.
 *
 * @param out_length the number of elements retrieved.
 *
 * @param refresh_content specifies whether or not the retrieved array will be initialized with the elements of the given Java array.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ILLEGAL_ARGUMENT</code> if <code>java_start</code> is
 * negative, <code>java_length</code> is negative, <code>java_length</code> is greater than
 * <code>SNI_getArrayLength(java_array) - java_start</code>, or one of the given pointers is null.
 */
int32_t SNI_retrieveArrayElements(jbyte* java_array, jint java_start, jint java_length, int8_t* buffer, uint32_t buffer_length, int8_t** out_buffer, uint32_t* out_length, bool refresh_content);

/**
 * If necessary, this function copies back all the content of <code>buffer</code> to the Java array.
 * <p>
 * If the given Java array is an immortal array, this function does nothing.
 * If the given Java array is not an immortal array, then this function copies back all the content of
 * <code>buffer</code> to the Java array at the offset <code>java_start</code>.
 *
 * @param java_array the Java array.
 *
 * @param java_start the start offset in array <code>java_array</code> of the first element to release.
 *
 * @param java_length the maximum number of bytes to release.
 *
 * @param buffer the buffer to use if the given Java array is not immortal.
 *
 * @param buffer_length length in bytes of <code>buffer</code>.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ILLEGAL_ARGUMENT</code> if <code>java_start</code> is
 * negative, <code>java_length</code> is negative, <code>java_length</code> is greater than
 * <code>SNI_getArrayLength(java_array) - java_start</code>, <code>buffer_length</code> is greater than
 * <code>java_length</code>, or one of the given pointers is null.
 */
int32_t SNI_flushArrayElements(jbyte* java_array, jint java_start, jint java_length, int8_t* buffer, uint32_t buffer_length);

// Pointer to the function to use for the SNI callback.
typedef void (*SNI_callback)(void);

/*
 * Creates and initializes a virtual machine.
 * This function MUST be called once before a call to <code>SNI_startVM()</code>.
 *
 * @return <code>null</code> if an error occurred, otherwise returns a virtual machine
 * instance.
 */
void* SNI_createVM(void);

/**
 * Starts the specified virtual machine and calls the <code>main()</code> method of the
 * Java application with the given String arguments.
 * This function returns when the Java application ends. 
 *
 * The Java application ends when there are no more Java threads to run or when the Java
 * method <code>System.exit(int)</code> is called.
 *
 * @param vm a pointer returned by the <code>SNI_createVM()</code> function.
 *
 * @param argc number of string arguments in <code>argv</code>.
 *
 * @param argv array of string arguments given to the Java <code>main()</code> method.
 * May be null.
 *
 * @return 0 when the virtual machine ends normally or a negative value when an error
 * occurred (see constant values <code>LLMJVM_E_*</code> defined in LLMJVM.h).
 */
int32_t SNI_startVM(void* vm, int32_t argc, char** argv);

/*
 * Call this function after virtual machine execution to get the Java application exit
 * code.
 *
 * @param vm a pointer returned by the <code>SNI_createVM()</code> function.
 *
 * @return the value given to the <code>System.exit(exitCode)</code> or 0 if the Java
 * application ended without calling <code>System.exit(exitCode)</code>.
 */
int32_t SNI_getExitCode(void* vm);

/**
 * Releases all the virtual machine resources. This function must be called after the end
 * of the execution of the virtual machine. The vm pointer is no longer valid after this call.
 *
 * @param vm a pointer returned by the <code>SNI_createVM()</code> function.
 */
void SNI_destroyVM(void* vm);

/**
 * Gets the ID of the current Java thread.
 *
 * The returned ID is the same than the one returned by the Java method
 * <code>Thread.getId()</code>.
 *
 * @return the ID of the current Java thread or <code>SNI_ERROR</code> if this function
 * is not called within the virtual machine task.
 */ 
int32_t SNI_getCurrentJavaThreadID(void);

/**
 * Causes the current Java thread to pause its Java execution after the end of the current
 * native method. This function is not blocking.
 * The current Java thread will resume its execution after the reception of an external
 * event or after <code>timeout</code> milliseconds.
 *
 * If the pending resume flag of the current Java thread is set, the Java thread is not
 * suspended and its pending resume flag is cleared (see <code>SNI_isResumePending()</code>).
 *
 * If an exception is pending, the thread is not suspended and this function returns
 * <code>SNI_ERROR</code>.
 *
 * The result of calling this function several times during the same native execution is
 * unpredictable.
 * 
 * @param timeout duration in milliseconds of the pause. If <code>timeout</code> is zero,
 * the thread waits forever until resumed.
 *
 * @return <code>SNI_OK</code> if the request is accepted. Returns <code>SNI_ERROR</code>
 * if this function is not called within the virtual machine task or if an exception is
 * pending.
 */
int32_t SNI_suspendCurrentJavaThread(int64_t timeout);

/**
 * Causes the current Java thread to pause its Java execution after the end of the current
 * native method. This function is not blocking.
 * The current Java thread will resume its execution after the reception of an external
 * event or after <code>timeout</code> milliseconds.
 *
 * If a non-null SNI callback is specified, then this callback is called when the Java
 * thread is resumed. The callback must have the same signature as the current executed
 * native (same parameters and same return type). The callback will be executed in the
 * virtual machine task.
 *
 * The value given to <code>callbackSuspendArg</code> can be retrieved in the SNI callback
 * using <code>SNI_getCallbackArgs()</code>.
 *
 * If the pending resume flag of the current Java thread is set, the Java thread is not
 * suspended and its pending resume flag is cleared (see <code>SNI_isResumePending()</code>).
 *
 * If an exception is pending, the thread is not suspended, this function returns
 * <code>SNI_ERROR</code>, and the callback is not called.
 *
 * The result of calling this function several times during the same native execution is
 * unpredictable.
 *
 * @param timeout duration in milliseconds of the pause. If <code>timeout</code> is zero,
 * the thread waits forever until resumed.
 *
 * @param sniCallback function called when the Java thread is resumed. May be null.
 *
 * @param callbackSuspendArg argument that can be retrieved in the callback using
 * <code>SNI_getCallbackArgs()</code>. May be null.
 *
 * @return <code>SNI_OK</code> if the request is accepted. Returns <code>SNI_ERROR</code>
 * if this function is not called within the virtual machine task or if an exception is
 * pending.
 */
int32_t SNI_suspendCurrentJavaThreadWithCallback(int64_t timeout, SNI_callback sniCallback, void* callbackSuspendArg);

/**
 * Causes the current Java thread to yield its execution to another Java thread after the
 * end of the current native method and then to call the given callback when it will be
 * scheduled again.
 *
 * The callback must have the same signature as the current executed native (same
 * parameters and same return type). The callback will be executed in the virtual machine
 * task.
 *
 * This function can be used to split the execution of a long native method into several
 * increments leaving some execution time to other Java threads.
 *
 * If an exception is pending, this function returns <code>SNI_ERROR</code>, and the
 * callback is not called.
 *
 * The result of calling this function several times during the same native execution is
 * unpredictable.
 *
 * @param sniCallback function called when the Java thread is scheduled again. May be null.
 *
 * @param callbackArg argument that can be retrieved in the callback using
 * <code>SNI_getCallbackArgs()</code>. May be null.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if this function
 * is not called within the virtual machine task or if an exception is pending.
 */
int32_t SNI_javaThreadYield(SNI_callback sniCallback, void* callbackArg);

/**
 * Retrieves the arguments given to an SNI callback with the function
 * <code>SNI_javaThreadYield()</code>, <code>SNI_suspendCurrentJavaThreadWithCallback()</code>
 * or <code>SNI_resumeJavaThreadWithArg()</code>.
 *
 * This function should be called in an SNI callback. If the functions
 * <code>SNI_suspendCurrentJavaThreadWithCallback()</code> or
 * <code>SNI_javaThreadYield()</code> have not been called previously for the current native
 * or one of its callback, the value returned in <code>callbackSuspendArgPtr</code> is
 * undefined. If the function <code>SNI_resumeJavaThreadWithArg()</code> has not been called
 * previously for the current native or one of its callback, the value returned in
 * <code>callbackResumeArgPtr</code> is undefined.
 *
 * @param callbackSuspendArgPtr pointer to an argument. This function will store in this
 * pointer the <code>callbackSuspendArg</code> value previously given to
 * <code>SNI_suspendCurrentJavaThreadWithCallback()</code> or <code>callbackArg</code> value
 * previously given to <code>SNI_javaThreadYield()</code>. May be null.
 *
 * @param callbackResumeArgPtr pointer to an argument. This function will store in this
 * pointer the <code>callbackResumeArg</code> value previously given to
 * <code>SNI_resumeJavaThreadWithArg()</code>. May be null.
 *
 * @return <code>SNI_OK</code> on success. Returns <code>SNI_ERROR</code> if this function
 * is not called within the virtual machine task.
 */
int32_t SNI_getCallbackArgs(void** callbackSuspendArgPtr, void** callbackResumeArgPtr);

/**
 * Resumes the given Java thread if it is suspended.
 * If the Java thread is not suspended, its pending resume flag is set and the next call of
 * <code>SNI_suspendCurrentJavaThread()</code> will not suspend the thread.
 * 
 * @param javaThreadID ID of the Java thread to resume. The Java thread ID can be
 * retrieved using <code>SNI_getCurrentJavaThreadID()</code> function.
 *
 * @return <code>SNI_ERROR</code> if the given Java thread ID is invalid, otherwise returns
 * <code>SNI_OK</code>.
 */
int32_t SNI_resumeJavaThread(int32_t javaThreadID);

/**
 * Resumes the given Java thread if it is suspended.
 * If the Java thread is not suspended, its pending resume flag is set and the next call of
 * <code>SNI_suspendCurrentJavaThread()</code> will not suspend the thread.
 *
 * @param javaThreadID ID of the Java thread to resume. The Java thread ID can be
 * retrieved using <code>SNI_getCurrentJavaThreadID()</code> function.
 *
 * @param callbackResumeArg argument that can be retrieved in the callback given to
 * <code>SNI_suspendCurrentJavaThreadWithCallback()</code> using
 * <code>SNI_getCallbackArgs()</code>. May be null.
 *
 * @return <code>SNI_ERROR</code> if the given Java thread ID is invalid, otherwise
 * returns <code>SNI_OK</code>.
 */
int32_t SNI_resumeJavaThreadWithArg(int32_t javaThreadID, void* callbackResumeArg);

/**
 * Tests whether the given Java thread has been resumed while it was not suspended.
 *
 * The pending resume flag is set when calling <code>SNI_resumeJavaThread()</code> or
 * <code>SNI_resumeJavaThreadWithArg()</code>. It is cleared when calling
 * <code>SNI_suspendCurrentJavaThread()</code> or <code>SNI_suspendCurrentJavaThreadWithCallback()</code>,
 * or when the Java thread actually resumes its execution. The pending resume flag is
 * not modified by a timeout that occurs during a pause.
 *
 * The pending resume flag of the given Java thread is unaffected by this method.
 *
 * If the given Java thread ID is invalid, this method does nothing and returns false.
 *
 * @param javaThreadID ID of the Java thread to test. The Java thread ID can be
 * retrieved using <code>SNI_getCurrentJavaThreadID()</code> function.
 *
 * @return <code>true</code> if the pending resume flag of the given Java thread is set,
 * <code>false</code> otherwise.
 */
bool SNI_isResumePending(int32_t javaThreadID);

/**
 * Clears the pending resume flag of the current Java thread (see <code>SNI_isResumePending()</code>).
 *
 * If this method is not called within the virtual machine task, it does nothing and returns false.
 *
 * @return <code>true</code> if the pending resume flag of the current Java thread was set before
 * clearing it, <code>false</code> otherwise.
 */
bool SNI_clearCurrentJavaThreadPendingResumeFlag(void);

#ifdef __cplusplus
	}
#endif
#endif /* SNI_H */
