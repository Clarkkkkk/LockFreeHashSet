/*
 * BucketList.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#include "BucketList.h"
#include "AtomicMarkablePointer.h"
#include <atomic>

#include <stdio.h>
#include <functional>

using namespace std;

#define INT_MAX 0x7fffffff

BucketList::BucketList() {
	this->head = new Node(0);
	Node* tmpNode = new Node(INT_MAX);
	AtomicMarkablePointer<Node*>* tmp = new AtomicMarkablePointer<Node*>(tmpNode, false);
	this->head->next = tmp;

}

BucketList::BucketList(Node* e) {
	this->head = e;
}

int BucketList::hashCode(int x) {
	uint32_t a = (uint32_t) x;
	a = (a + 0x7ed55d16) + (a << 12);
	a = (a ^ 0xc761c23c) ^ (a >> 19);
	a = (a + 0x165667b1) + (a << 5);
	a = (a + 0xd3a2646c) ^ (a << 9);
	a = (a + 0xfd7046c5) + (a << 3);
	a = (a ^ 0xb55a4f09) ^ (a >> 16);
	int retval = a & MASK;
	return retval;
}
bool BucketList::add(int x) {
	int key = makeRegularKey(x);
	bool splice;
	while (true) {
		// find predecessor and current entries
		Window* window = find(head, key);
		Node* pred = window->pred;
		Node* curr = window->curr;
		if (curr->key == key) {
			return false;
		} else {
			// splice in new entry
			Node* entry = new Node(key, x);
			entry->next->set(curr, false);
			splice = pred->next->compareAndSet(curr, entry, false, false);
			if (splice) {
				return true;
			} else {
				continue;
			}
		}
	}
}

bool BucketList::remove(int x) {
	int key = makeRegularKey(x);
	bool snip;
	while (true) {
		Window* window = find(head, key);
		Node* pred = window->pred;
		Node* curr = window->curr;
		// no such key exist, remove failed
		if (curr->key != key) {
			return false;
		} else {
			snip = pred->next->attemptMark(curr, true);
			if (snip) {
				return true;
			} else {
				continue;
			}
		}
	}
}

bool BucketList::contains(int x) {
	int key = makeRegularKey(x);
	Window* window = find(head, key);
	//Node* pred = window->pred;
	Node* curr = window->curr;
	return (curr->key == key);
}

BucketList* BucketList::getSentinel(int index) {
	int key = makeSentinelKey(index);
	bool splice;
	while (true) {
		// find predecessor and current entries
		Window* window = find(head, key);
		Node* pred = window->pred;
		Node* curr = window->curr;
		if (curr->key == key) {
			return new BucketList(curr);
		} else {
			// splice in new entry
			Node* entry = new Node(key);
			entry->next->set(pred->next->getReference(), false);
			splice = pred->next->compareAndSet(curr, entry, false, false);
			if (splice) {
				return new BucketList(entry);
			} else {
				continue;
			}
		}
	}
}

int BucketList::reverse(int key) {
    int loMask = LO_MASK;
    int hiMask = HI_MASK;
    int result = 0;
    for (int i = 0; i < WORD_SIZE; i++) {
    	if ((key & loMask) != 0) {
    		result |= hiMask;
    	}
    	loMask <<= 1;
    	hiMask >>= 1;
    	hiMask &= 0x7fffffff;
    }
    return result;
}

int BucketList::makeRegularKey(int x) {
	int code = hashCode(x) & MASK;
	return reverse(code | HI_MASK);
}

int BucketList::makeSentinelKey(int key) {
	return reverse(key & MASK);
}

Window* BucketList::find(Node* head, int key) {
	Node* pred = head;
	Node* curr = head->getNext();
	while (curr->key < key) {
		pred = curr;
		curr = pred->getNext();
	}
	return new Window(pred, curr);
}
