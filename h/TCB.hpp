#ifndef _TCB_hpp_
#define _TCB_hpp_

#include "../lib/hw.h"
#include "../h/scheduler.hpp"

typedef TCB* thread_t;

class TCB {
public:
    ~TCB() {
        if (stack != nullptr) MemoryAllocator::memoryFree(stack);
    }
    using Body = void (*)(void*);
    bool isFinished() const { return finished; }
    void setFinished() { unjoin(); finished = true; }
    bool isSleeping() const { return sleeping; }
    void setSleeping(bool sleeping) { this->sleeping = sleeping; }
    bool isBlocked() const { return blocked; }
    void setBlocked(bool blocked) { this->blocked = blocked; }
    uint64 getTimeSlice() const { return timeSlice; }
    static TCB* createThread(TCB** handle, Body body, void* arg, char* stack_space, uint64 type);
    static void yield();
    static TCB* running;
    void join(TCB* handle);
    uint64 getWaiting() const { return waiting; }
    void incWaiting() { ++waiting; }
    void decWaiting() { --waiting; }
private:
    char* stack;
    char* userStack, *systemStack;
    uint64 userSp, systemSp;
    bool finished, sleeping, blocked, systemThread;
    void *arg;
    uint64 timeSlice;
    Body body;
    struct Context {
        uint64 ra;
        uint64 sp;
    };
    Context context;
    static void dispatch();
    friend class Riscv;
    static void contextSwitch(Context* oldContext, Context* runningContext);
    static void threadWrapper();
    static uint64 timeSliceCounter;
    static void user_to_sys();
    static void sys_to_user();
    friend class _sem;
    ListTCB listWaiting_threads;
    void unjoin();
    uint64 waiting;
};

#endif