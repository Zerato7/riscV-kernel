#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB* TCB::createThread(TCB** handle, Body body, void* arg, char* stack_space, uint64 type) {
    TCB* tmp = (TCB*)MemoryAllocator::memoryAllocate(sizeof(TCB));
    tmp->finished = tmp->sleeping = tmp->blocked = false;
    tmp->body = body;
    tmp->arg = arg;
    tmp->stack = body != nullptr ? stack_space : nullptr;
    tmp->userStack = body != nullptr ? stack_space : nullptr;
    tmp->systemStack = tmp->userStack != nullptr ? stack_space + DEFAULT_STACK_SIZE : nullptr;
    tmp->userSp = tmp->userStack != nullptr ? (uint64)&tmp->userStack[DEFAULT_STACK_SIZE] : 0;
    tmp->systemSp = tmp->systemStack != nullptr ? (uint64)&tmp->userStack[2 * DEFAULT_STACK_SIZE] : 0;
    tmp->timeSlice = DEFAULT_TIME_SLICE;
    tmp->context = {
            (uint64)&TCB::threadWrapper,
            type != 1 ? tmp->userSp : tmp->systemSp
    };
    if (tmp->stack == nullptr && stack_space != nullptr) MemoryAllocator::memoryFree(stack_space);
    *handle = tmp;
    if (body != nullptr) Scheduler::put(tmp);
    if (type == 1 || body == nullptr) {
        tmp->systemThread = true;
    } else tmp->systemThread = false;
    tmp->waiting = 0;
    tmp->listWaiting_threads.initializeList();
    return tmp;
}

void TCB::yield() {
    Riscv::w_a0(0x13);
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB::timeSliceCounter = 0;
    TCB* old = running;
    if (!old->finished && !old->sleeping && !old->blocked && old->waiting == 0) {
        Scheduler::put(old);
    }
    running = Scheduler::get();
    if (old == running) return;
    //Riscv::pushRegisters();
    contextSwitch(&old->context, &running->context);
    //Riscv::popRegisters();
}

void TCB::threadWrapper() {
    if (!running->systemThread) Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished();
    thread_dispatch();
}

void TCB::user_to_sys() {
    if (running != nullptr && !running->systemThread) {
        __asm__ volatile ("mv %[userSp], sp" : [userSp] "=r"(running->userSp));
        __asm__ volatile ("mv sp, %[systemSp]" : : [systemSp] "r"(running->systemSp));
        __asm__ volatile ("addi sp, sp, -16");
    }
    __asm__ volatile ("ld a4, 1 * 8(fp)");
    __asm__ volatile ("ld a5, 0 * 8(fp)");
}

void TCB::sys_to_user() {
    if (running != nullptr && !running->systemThread) {
        __asm__ volatile ("mv sp, %[userSp]" : : [userSp] "r"(running->userSp));
    }
}

void TCB::join(TCB* handle) {
    if (handle == nullptr) return;
    if (handle->finished == true) return;
    handle->listWaiting_threads.addLast(this);
    this->incWaiting();
    TCB::dispatch();
}

void TCB::unjoin() {
    for (TCB* cur = listWaiting_threads.removeFirst(); cur != nullptr; cur = listWaiting_threads.removeFirst()) {
        cur->decWaiting();
        if (cur->getWaiting() == 0) Scheduler::put(cur);
    }
}
