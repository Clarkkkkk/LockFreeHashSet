/*
 * Node.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#ifndef NODE_H_
#define NODE_H_

#include <atomic>
#include "AtomicMarkablePointer.h"

using namespace std;

class Node {
public:
	int key;
	int value;
	AtomicMarkablePointer<Node*>* next;
	Node(int key, int object);
	Node(int key);
	Node* getNext();
};


#endif /* NODE_H_ */
