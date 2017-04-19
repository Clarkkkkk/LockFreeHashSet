/*
 * LockFreeHashSet.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#include "LockFreeHashSet.h"
#include <stdlib.h>

LockFreeHashSet::LockFreeHashSet(int capacity) {
	bucket.resize(capacity);
	for (int i = 0; i < capacity; i++) {
		bucket[i] = NULL;
	}
	bucket[0] = new BucketList();
	bucketSize = 2;
	setSize = 0;
}

bool LockFreeHashSet::add(int x) {
	int hashVal = BucketList::hashCode(x);
	int myBucket = abs((int)(hashVal % bucketSize));
	BucketList* b = this->getBucketList(myBucket);
	if (!(b->add(x))) {
		return false;
	}
	setSize++;
	int setSizeNow = setSize;
	int bucketSizeNow = bucketSize;
	if (setSizeNow / (double) bucketSizeNow > THRESHOLD) {
		bucketSize.compare_exchange_strong(bucketSizeNow, 2 * bucketSizeNow);
	}
	return true;
}

bool LockFreeHashSet::remove(int x) {
	int hashVal = BucketList::hashCode(x);
	int myBucket = abs((int)(hashVal % bucketSize));
	BucketList* b = getBucketList(myBucket);
	if (!b->remove(x)) {
		return false;
	}
	return true;
}

bool LockFreeHashSet::contains(int x) {
	int hashVal = BucketList::hashCode(x);
	int myBucket = abs((int)(hashVal % bucketSize));
	BucketList *b = getBucketList(myBucket);
	return b->contains(x);
}

BucketList* LockFreeHashSet::getBucketList(int myBucket) {
	if (bucket[myBucket] == NULL) {
		initializeBucket(myBucket);
	}
	return bucket[myBucket];
}

void LockFreeHashSet::initializeBucket(int myBucket) {
	int parent = getParent(myBucket);
	if (bucket[parent] == NULL) {
		initializeBucket(parent);
	}
	BucketList* b = bucket[parent]->getSentinel(myBucket);
	if (b != NULL) {
		bucket[myBucket] = b;
	}
}

int LockFreeHashSet::getParent(int myBucket) {
	int parent = bucketSize;
	do {
		parent = parent >> 1;
	} while (parent > myBucket);
	parent = myBucket - parent;
	return parent;
}
