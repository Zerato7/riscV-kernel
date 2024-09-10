#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/print.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/_Buffer.hpp"
//#include "../lib/console.h"

extern void userMain();

TCB* idle;

void idleMain(void*) {
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while (true) {
    }
}

void outputMain(void*) {
    while (true) {
        while ((*((char*)CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT) {
            //__putc('b');
            (*((char*)CONSOLE_TX_DATA)) = Riscv::putBuff->get();
            /*__putc('c');
        */}
    }
}

int main() {
    uint64 base = ((uint64)&Riscv::supervisorTrap)|1UL;
    Riscv::w_stvec(base);
    MemoryAllocator::initMemoryAllocator();
    Riscv::initBuff();

    TCB* mainThread;
    thread_create(&mainThread, nullptr, nullptr);
    TCB::running = mainThread;

    TCB* out;
    TCB::createThread(&out, outputMain, nullptr, (char*)MemoryAllocator::memoryAllocate(2 * DEFAULT_STACK_SIZE), 1);

    TCB::createThread(&idle, idleMain, nullptr, (char*)MemoryAllocator::memoryAllocate(2 * DEFAULT_STACK_SIZE), 1);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);


    TCB* userThread;
    thread_create(&userThread, (TCB::Body)userMain, nullptr);

    printStr("Kreirana glavna korisnicka nit. ");

    thread_join(userThread);

    printStr("Kraj korisnicke niti. ");

    thread_dispatch();

    while (idle != Scheduler::get()) {}

    delete out;

    delete Riscv::getBuff;
    delete Riscv::putBuff;

    for (_sem* cur = Riscv::listClosed_sem.removeFirst(); cur; cur = Riscv::listClosed_sem.removeFirst()) {
        MemoryAllocator::memoryFree(cur);
    }

    delete mainThread;
    delete userThread;
    delete idle;

    return 0;
}