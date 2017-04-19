/*
 * LockFreeHashSet.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#ifndef LOCKFREEHASHSET_H_
#define LOCKFREEHASHSET_H_

#include <vector>
#include <atomic>

#include "BucketList.h"

using namespace std;

class LockFreeHashSet {
public:
	LockFreeHashSet(int capacity);
	bool add(int x);
	bool remove(int x);
	bool contains(int x);
private:
	BucketList* getBucketList(int myBucket);
	void initializeBucket(int myBucket);
	int getParent(int myBucket);

	vector<BucketList*> bucket;
	atomic<int> bucketSize;
	atomic<int> setSize;
	const double THRESHOLD = 4.0;

};

#endif /* LOCKFREEHASHSET_H_ */
