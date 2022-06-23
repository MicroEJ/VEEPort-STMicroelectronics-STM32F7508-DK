/**
 * C
 *
 * Copyright 2010-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * Header file for Shielded Plug (SP), version 1.0
 */
#ifndef SP_H
#define SP_H

#include <stdint.h>

#define SP_SUCCESS				 		 0	//function succeeded
#define SP_ERR_INVALID_BLOCK_ID			-1	//invalid block ID
#define SP_ERR_EMPTY_BLOCK				-2	//no data available in the block
#define SP_ERR_INTERRUPTED				-3	//current thread has been interrupted
#define SP_ERR_TOO_MANY_WAITING_THREADS	-4	//the limit on the number of threads waiting on a block has been reached

typedef void* ShieldedPlug;

/*
 * Returns the database identified by the given ID, or 0 if ID is undefined.
 */
ShieldedPlug SP_getDatabase(int32_t ID);

/*
 * Returns the number of blocks in the given database.
 */
int32_t SP_getSize(ShieldedPlug sp);

/*
 * Fills the given array with the IDs of the blocks available in this database.
 * If length is lower than the number of blocks in the database, only length IDs are copied.
 * If length is greater than the number of blocks in the database, the array is only partially filled.
 * Returns the number of blocks in the given database.
 */
int32_t SP_getIDs(ShieldedPlug sp, int32_t* blocksIDs, int32_t length);

/*
 * Returns the length in bytes of the block with the given ID.
 * Returns <code>SP_ERR_INVALID_BLOCK_ID</code> if no block is defined with the given ID.
 */
int32_t SP_getLength(ShieldedPlug sp, int32_t blockID);

/*
 * Returns the maximum number of tasks that can wait at the same time on the block defined with the
 * given ID.
 * Returns <code>SP_ERR_INVALID_BLOCK_ID</code> if no block is defined with the given ID.
 */
int32_t SP_getMaxTasks(ShieldedPlug sp, int32_t blockID);

/*
 * Fills the given buffer with data from the block with the given ID.
 * The number of bytes read is equal to the block size.
 * Returns <code>SP_SUCCESS</code> on success, otherwise returns one of the following errors:
 * - <code>SP_ERR_INVALID_BLOCK_ID</code>	if no block is defined with the given ID.
 * - <code>SP_ERR_EMPTY_BLOCK</code>		if no data available in the block.
 */
int32_t SP_read (ShieldedPlug sp, int32_t blockID, void* buff);

/*
 * Writes bytes from the given buffer to the block with the given ID.
 * The number of written bytes is equal to the block size. If any tasks are waiting for
 * data to be written to this block they are all unblocked.
 * Returns <code>SP_SUCCESS</code> on success, otherwise returns one of the following errors:
 * - <code>SP_ERR_INVALID_BLOCK_ID</code>	if no block is defined with the given ID.
 */
int32_t SP_write(ShieldedPlug sp, int32_t blockID, void* buff);

/*
 * Causes current thread to wait until another thread writes data into the block with the given ID.
 * If data has been written in the block since the last read, this method returns immediately.
 * Returns <code>SP_SUCCESS</code> on success, otherwise returns one of the following errors:
 * - <code>SP_ERR_INVALID_BLOCK_ID</code>	if no block is defined with the given ID.
 * - <code>SP_ERR_TOO_MANY_WAITING_THREADS</code>	if the limit on the number of threads waiting on the block has been reached.
 * - <code>SP_ERR_INTERRUPTED</code>		if another thread has interrupted the current thread.
 */
int32_t SP_waitFor(ShieldedPlug sp, int32_t blockID);

/*
 * Causes current thread to wait until another thread writes data into one of the specified blocks.
 * If data has been written in one of the specified blocks since the last read from it,
 * this method returns immediately.
 * Parameters:
 * - blocksIDs:		list of block IDs.
 * - modifiedIDs:	filled with the list of IDs of the blocks that have been written to.
 * - length: 		before the call: the number of IDs in blocksIDs; after the call: the number of IDs in modifiedsIDs.
 * Returns <code>SP_SUCCESS</code> on success, otherwise returns one of the following error:
 * - <code>SP_ERR_INVALID_BLOCK_ID</code>	if one of the ID does not correspond to an existing block.
 * - <code>SP_ERR_TOO_MANY_WAITING_THREADS</code>	if the limit on the number of threads waiting on a block has been reached.
 * - <code>SP_ERR_INTERRUPTED</code>		if another thread has interrupted the current thread.
 */
int32_t SP_waitForSeveral(ShieldedPlug sp, int32_t* blockIDs, int32_t* modifiedIDs, int32_t* length);

/*
 * Returns 1 if data has been written into the block since last read, 0 otherwise.
 * Returns <code>SP_ERR_INVALID_BLOCK_ID</code> if no block is defined with the given ID.
 */
int32_t SP_isPending(ShieldedPlug sp, int32_t blockID);

/*
 * Indicates whether or not data are available in the block with the given ID.
 * Initially no data are available in a block. When data are written in a block, they remain
 * available until method SP_reset(ShieldedPlug, int32_t) is called.
 * Returns 1 if data are available in the block, 0 otherwise.
 * Returns <code>SP_ERR_INVALID_BLOCK_ID</code> if no block is defined with the given ID.
 */
int32_t SP_isDataAvailable(ShieldedPlug sp, int32_t blockID);

/*
 * Resets (clears) data of the block with the given ID.
 * After execution, SP_isDataAvailable method would return
 * 0 (unless data were written after calling SP_reset and before
 * calling SP_isDataAvailable).
 * Returns <code>SP_SUCCESS</code> on success, otherwise returns <code>SP_ERR_INVALID_BLOCK_ID</code>
 * if no block is defined with the given ID.
 */
int32_t SP_reset(ShieldedPlug sp, int32_t blockID);

#endif /* SP_H */
