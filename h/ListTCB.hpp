#ifndef _list_TCB_hpp_
#define _list_TCB_hpp_

#include "../h/MemoryAllocator.hpp"

class TCB;

class ListTCB {
private:
    struct Elem {
        TCB* data;
        Elem* next;

        Elem(TCB* d, Elem* n) : data(d), next(n) {}
    } *head, *tail;
public:
    ListTCB() : head(0), tail(0) {}

    void initializeList();

    ListTCB(const ListTCB&) = delete;
    ListTCB& operator =(const ListTCB&) = delete;

    ListTCB& addLast(TCB* x);

    TCB* removeFirst();

};

#endif