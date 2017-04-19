/*
 * Node.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#include "Node.h"
#include "AtomicMarkablePointer.h"
#include <stdio.h>

Node::Node(int key, int object) {
	this->key = key;
	this->value = object;
	AtomicMarkablePointer<Node*>* tmp = new AtomicMarkablePointer<Node*>(NULL, false);
	this->next = tmp;
}

Node::Node(int key) {
	this->key = key;
	this->value = -1;
	AtomicMarkablePointer<Node*>* tmp = new AtomicMarkablePointer<Node*>(NULL, false);
	this->next = tmp;
}


Node* Node::getNext() {
	bool cMarked = false;
	bool sMarked = false;
	Node* entry = this->next->get(cMarked);
	while (cMarked) {
		Node* succ = entry->next->get(sMarked);
		this->next->compareAndSet(entry, succ, true, sMarked);
		entry = this->next->get(cMarked);
	}
	return entry;
}
