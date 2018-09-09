/*
 * lockfree_queue.h
 *
 *  Created on: 2018年9月8日
 *      Author: garvin
 */

#ifndef LOCKFREE_QUEUE_H_
#define LOCKFREE_QUEUE_H_

enum Value{
	Head = 0,
	Tail,
	Empty
};

enum State{
	Full = 3,
	Normal,
	Empty
};

class lockfree_queue{
private:
	static int headIndex;
	static int tailIndex;
	static int emptyIndex;
};

#endif /* LOCKFREE_QUEUE_H_ */
