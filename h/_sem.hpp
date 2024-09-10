#ifndef __sem_hpp_
#define __sem_hpp_

//#include "../h/TCB.hpp"
//#include "../h/riscv.hpp"
#include "../h/ListTCB.hpp"

class _sem {
public:
    ~_sem();
    int _sem_wait();
    int _sem_signal();
    int _sem_close();
    static _sem* create_Sem(_sem** handle, unsigned init);
private:
    _sem(unsigned init = 1) : val(init), numMaxThreads(val) {}
    int val, numMaxThreads;
    bool closed;
    ListTCB listBlocked_threads;
    void block();
    void unblock();
    friend class _Buffer;
    void clean();
};

typedef _sem* sem_t;

#endif