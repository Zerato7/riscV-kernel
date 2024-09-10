#include "../h/_sem.hpp"
#include "../h/riscv.hpp"
//#include "../lib/console.h"

_sem::~_sem() {
    _sem_close();
}

int _sem::_sem_wait() {
    //__putc('B');
    if (closed) return -2;
    --val;
    if (val < 0) {
        //__putc('A');
        block();

    }
    if (closed) {
        return -1;
    }
    return 0;
}

int _sem::_sem_signal() {
    if (closed) return -2;
    ++val;
    if (val <= 0) {
        unblock();
    }
    return 0;
}

int _sem::_sem_close() {
    if (closed) return -1;
    closed = true;
    for (TCB* cur = listBlocked_threads.removeFirst(); cur; cur = listBlocked_threads.removeFirst()) {
        cur->setBlocked(false);
        Scheduler::put(cur);
    }
    Riscv::listClosed_sem.addLast(this);
    return 0;
}

void _sem::block() {
    listBlocked_threads.addLast(TCB::running);
    TCB::running->setBlocked(true);
    TCB::dispatch();
}

void _sem::unblock() {
    TCB* m = listBlocked_threads.removeFirst();
    if (!m) return;
    m->setBlocked(false);
    Scheduler::put(m);
}

void _sem::clean() {
    for (; listBlocked_threads.removeFirst(); ) {}
}

_sem* _sem::create_Sem(_sem** handle, unsigned init) {
    _sem* tmp = (_sem*)MemoryAllocator::memoryAllocate(sizeof(_sem));
    tmp->closed = false;
    tmp->val = init;
    tmp->numMaxThreads = init;
    tmp->listBlocked_threads.initializeList();
    *handle = tmp;
    return tmp;
}