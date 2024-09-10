#include "../h/syscall_c.h"
#include "../h/riscv.hpp"

void* mem_alloc(size_t size) {
    uint64 s = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    Riscv::w_a1(s);
    Riscv::w_a0(0x01);
    __asm__ volatile ("ecall");
    void* volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int mem_free(void* p) {
    uint64 h = (uint64)p;
    Riscv::w_a1(h);
    Riscv::w_a0(0x02);
    __asm__ volatile ("ecall");
    int volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) {
    uint64 s = (uint64)MemoryAllocator::memoryAllocate(2 * DEFAULT_STACK_SIZE);
    Riscv::w_a4(s);
    uint64 a = (uint64)arg;
    Riscv::w_a3(a);
    uint64 sr = (uint64)start_routine;
    Riscv::w_a2(sr);
    uint64 h = (uint64)handle;
    Riscv::w_a1(h);
    Riscv::w_a0(0x11);
    __asm__ volatile ("ecall");
    int volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_exit() {
    Riscv::w_a0(0x12);
    __asm__ volatile ("ecall");
    return 0;
    //int volatile ret;
    //__asm__ volatile ("mv %0, a0" : "=r"(ret));
    //return ret;
}

void thread_dispatch() {
    Riscv::w_a0(0x13);
    __asm__ volatile ("ecall");
}

void thread_join(thread_t handle) {
    uint64 h = (uint64)handle;
    Riscv::w_a1(h);
    Riscv::w_a0(0x14);
    __asm__ volatile ("ecall");
}

int sem_open(sem_t* handle, unsigned init) {
    uint64 i = init;
    Riscv::w_a2(i);
    uint64 h = (uint64)handle;
    Riscv::w_a1(h);
    Riscv::w_a0(0x21);
    __asm__ volatile ("ecall");
    int volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_close(sem_t handle) {
    uint64 h = (uint64)handle;
    Riscv::w_a1(h);
    Riscv::w_a0(0x22);
    __asm__ volatile ("ecall");
    int volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_wait(sem_t id) {
    uint64 h = (uint64)id;
    Riscv::w_a1(h);
    Riscv::w_a0(0x23);
    __asm__ volatile ("ecall");
    int volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal(sem_t id) {
    uint64 h = (uint64)id;
    Riscv::w_a1(h);
    Riscv::w_a0(0x24);
    __asm__ volatile ("ecall");
    int volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int time_sleep(time_t t) {
    uint64 h = t;
    Riscv::w_a1(h);
    Riscv::w_a0(0x31);
    __asm__ volatile ("ecall");
    //int volatile ret;
    //__asm__ volatile ("mv %0, a0" : "=r"(ret));
    //return ret;
    return 0;
}

char getc() {
    Riscv::w_a0(0x41);
    __asm__ volatile ("ecall");
    char volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

void putc(char c) {
    uint64 h = (uint64)c;
    Riscv::w_a1(h);
    Riscv::w_a0(0x42);
    __asm__ volatile ("ecall");
}
