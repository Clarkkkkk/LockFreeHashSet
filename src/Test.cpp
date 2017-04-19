#include <iostream>
#include "LockFreeHashSet.h"
#include <pthread.h>
#include <sys/time.h>
using namespace std;
#define START 0
#define END 10000

//#define ADD_REMOVE_TEST
#define MULTI_THREAD_TEST

struct timeval system_start;

/*
 * Util
 */
// in us
unsigned int timeToInt(struct timeval t){
	return (t.tv_sec * 1000000) + t.tv_usec ;
}
// in us
unsigned int getPeriodTime(struct timeval t1, struct timeval t0){
	return timeToInt(t1) - timeToInt(t0);
}
// in ms
double displayNowTime(){
    unsigned int temp = 0;
    struct timeval current_time;
    gettimeofday(&current_time, 0);
    temp = timeToInt(current_time) - timeToInt(system_start);
    return ((double)temp)/1000;
}
// in ms
double displayPeriod(unsigned int period){
    return ((double)period)/1000;
}
// in ms
double displayTime(struct timeval t){
    unsigned int temp = 0;
    temp = timeToInt(t) - timeToInt(system_start);
    return ((double)temp)/1000;
}


/*
 * Serialization Test
 */
void print(bool flag, bool rightFlag) {
	cout << "the output is(correct answer is: " << (rightFlag == true ? 1 : 0) <<"): " << (flag == true ? 1 : 0) << endl;
}

/*
 * Concurrent Test
 */

static void *largetTest(void *arg) {
	LockFreeHashSet* set = (LockFreeHashSet*) arg;
	for (int i = START; i < END; i++) {
		int tmp = rand() % 1000;
		set->add(tmp);
	}
	for (int i = START; i < END; i++) {
		int tmp = rand() % 1000;
		set->remove(tmp);
	}
	for (int i = START; i < END; i++) {
		int tmp = rand() % 1000;
		set->contains(tmp);
	}
	for (int i = START; i < END; i++) {
		int tmp = rand() % 1000;
		set->add(tmp);
	}
	for (int i = START; i < END; i++) {
		int tmp = rand() % 1000;
		set->remove(tmp);
	}
	return 0;
}

void multiTest(int thrNum) {
	struct timeval test_start, test_finish;
	LockFreeHashSet* set = new LockFreeHashSet(thrNum * 10000);
	cout << "start " << thrNum << " thread:  " <<endl;
	pthread_t thrList[thrNum];
	gettimeofday(&test_start,0);

	for (int i = 0; i < thrNum; i++) {
		pthread_create(&(thrList[i]), 0, largetTest, set);
	}

	for (int i = 0; i < thrNum; i++) {
		pthread_join(thrList[i], 0);
	}
	gettimeofday(&test_finish, 0);
	unsigned int time = getPeriodTime(test_finish, test_start);
	cout << "finish" << thrNum << " thread in" << displayPeriod(time) << "ms" << endl;
}


int main() {

#ifdef ADD_REMOVE_TEST
	LockFreeHashSet* set = new LockFreeHashSet(100);

	int start = 1;
	int end = 2;
	cout << "add success test start" << endl;
	for (int i = start; i < end; i++) {
		print(set->add(i), true);
	}
	cout << "add success test finish" << endl;

	cout << "contains success test start" << endl;
	for (int i = start; i < end; i++) {
		print(set->contains(i), true);
	}
	cout << "contains success test finish" << endl;

	cout << "add fail test start" << endl;
	for (int i = start; i < end; i++) {
		print(set->add(i), false);
	}
	cout << "add fail test finish" << endl;

	cout << "remove success test start" << endl;
	for (int i = start; i < end; i++) {
		print(set->remove(i), true);
	}
	cout << "remove success test finish" << endl;

	cout << "contains fail test start" << endl;
	for (int i = start; i < end; i++) {
		print(set->contains(i), false);
	}
	cout << "contains fail test finish" << endl;

	cout << "remove fail test start" << endl;
	for (int i = start; i < end; i++) {
		print(set->remove(i), false);
	}
	cout << "remove fail test finish" <<endl;
#endif

#ifdef MULTI_THREAD_TEST

	multiTest(1);

	multiTest(3);

	multiTest(6);

	multiTest(9);

	multiTest(18);

	multiTest(100);

#endif
}

