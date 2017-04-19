#ifndef AtomicMarkablePointer_H
#define AtomicMarkablePointer_H

#include <atomic>
#include <stdint.h>

template<class Pointer>
class AtomicMarkablePointer{
private:
    std::atomic<Pointer> ptr;
    const intptr_t Marked = 0x00000001;
    const intptr_t UnMraked = 0x00000000;
public:
    explicit AtomicMarkablePointer(Pointer p, bool mark){
        ptr.store((Pointer)((intptr_t)p | (intptr_t)mark));
    }
    AtomicMarkablePointer(const AtomicMarkablePointer &a){
        ptr.store(a.ptr.load());
    }
    AtomicMarkablePointer operator = (const AtomicMarkablePointer &a){
        if (*this != a){
            ptr.store(a.ptr.load());
        }
        return *this;
    }
    Pointer getReference() const {
        Pointer p = ptr.load();
        return (bool)((intptr_t)p & Marked) ? (Pointer)((intptr_t)(p)& ~Marked) : p;
    }
    bool isMarked() const {
        Pointer p = ptr.load();
        return (bool)((intptr_t)p & Marked);
    }
    Pointer get(bool &b) const {
        Pointer p = ptr.load();
        b = (bool)((intptr_t)p & Marked);
        return b ? (Pointer)((intptr_t)(p)& ~Marked) : p;
    }
    bool compareAndSet(Pointer expectedPointer, Pointer newPointer,
        bool expectedMark, bool newMark){
        Pointer p = ptr.load();
        bool b = (bool)((intptr_t)p & Marked);
        if (b == expectedMark){
            expectedPointer = (Pointer)((intptr_t)expectedPointer | (intptr_t)b);
            return ptr.compare_exchange_strong(expectedPointer, (Pointer)((intptr_t)newPointer | (intptr_t)newMark));
        }
        return false;
    }
    void set(Pointer newPointer, bool newMark){
        newPointer = (Pointer)((intptr_t)newPointer | (intptr_t)newMark);
        ptr.exchange(newPointer);
    }
    bool attemptMark(Pointer expectedPointer, bool newMark){
        Pointer newPointer = (Pointer)((intptr_t)expectedPointer | (intptr_t)newMark);
        expectedPointer = isMarked() ? (Pointer)((intptr_t)expectedPointer | Marked) : expectedPointer;
        return ptr.compare_exchange_strong(expectedPointer, newPointer);
    }
    /*void print()const{
        std::cout << getReference() << " ";
        std::cout << (isMarked() ? "Marked" : "UnMarked");
    }*/
};

#endif
