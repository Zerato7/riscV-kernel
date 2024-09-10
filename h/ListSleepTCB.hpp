#ifndef _list_Sleep_TCB_hpp_
#define _list_Sleep_TCB_hpp_

#include "../h/TCB.hpp"

class ListSleepTCB {
private:
    struct Elem {
        TCB* data;
        time_t relTime;
        Elem* next;
    } *head, *tail;
public:
    ListSleepTCB() : head(nullptr), tail(nullptr) {}
    ListSleepTCB(const ListSleepTCB&) = delete;
    ListSleepTCB& operator = (const ListSleepTCB&) = delete;
    void decreaseTime();
    ListSleepTCB& add(TCB* x, time_t t);
    void awake();
};

#endif