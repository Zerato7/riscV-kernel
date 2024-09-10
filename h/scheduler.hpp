#ifndef _scheduler_hpp_
#define _scheduler_hpp_

#include "../h/ListTCB.hpp"

class TCB;

class Scheduler {
private:
    static ListTCB threadsReady;
public:
    static TCB* get();
    static void put(TCB*);
};

#endif