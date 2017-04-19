/*
 * BucketList.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#ifndef BUCKETLIST_H_
#define BUCKETLIST_H_

#include "Node.h"
#include "Window.h"

class BucketList {
public:
	static const int WORD_SIZE = 24;
	static const int LO_MASK = 0x00000001;
	static const int HI_MASK = 0x00800000;
	static const int MASK = 0x00FFFFFF;
	Node *head;
	BucketList();
	BucketList(Node* e);
	static int hashCode(int x);
	bool add(int x);
	bool remove(int x);
	bool contains(int x);
	BucketList* getSentinel(int index);
	static int reverse(int key);
	int makeRegularKey(int x);
	Window* find(Node* head, int key);

private:
	int makeSentinelKey(int key);
};

#endif /* BUCKETLIST_H_ */
