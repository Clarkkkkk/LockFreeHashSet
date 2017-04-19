/*
 * Window.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Clark
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "Node.h"

class Window {
public:
	Node* pred;
	Node* curr;
	Window(Node* pred, Node* curr);
};

#endif /* WINDOW_H_ */
