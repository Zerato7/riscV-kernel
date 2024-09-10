#include "../h/scheduler.hpp"

ListTCB Scheduler::threadsReady;

extern TCB* idle;

TCB* Scheduler::get() {
    TCB* tmp = threadsReady.removeFirst();
    return tmp == nullptr ? idle : tmp;
}

void Scheduler::put(TCB* x) {
    if (x == idle) return;
    threadsReady.addLast(x);
}