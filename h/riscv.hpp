#ifndef _riscv_hpp_
#define _riscv_hpp_

#include "../lib/hw.h"
#include "../h/ListSleepTCB.hpp"
#include "../h/List_sem.hpp"
#include "../h/_Buffer.hpp"

class Riscv {
    static void handleIntr();
    static void handleTimer();
    static void handleConsole();
    static ListSleepTCB listSleeping_threads;

    static bool firstTime;

public:
    static void supervisorTrap();
    static void popSppSpie();

    static void pushRegisters();
    static void popRegisters();

    static List_sem listClosed_sem;
    static _Buffer *getBuff, *putBuff;
    static const unsigned _BUFFER_SIZE = 256;
    static void initBuff();

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

    static void ms_sip(uint64 mask);
    static void mc_sip(uint64 mask);
    static uint64 r_sip();
    static void w_sip(uint64 sip);

    enum BitMaskSStatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);
    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);

    static uint64 r_scause();
    static void w_scause(uint64 scause);
    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);
    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);
    static uint64 r_stval();
    static void w_stval(uint64 stval);

    static void w_stack_a0(uint64 val);
    static void w_a0(uint64 val);
    static uint64 r_a0();
    static void w_a1(uint64 val);
    static uint64 r_a1();
    static void w_a2(uint64 val);
    static uint64 r_a2();
    static void w_a3(uint64 val);
    static uint64 r_a3();
    static void w_a4(uint64 val);
    static uint64 r_a4();
};

inline void Riscv::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip() {
    uint64 volatile ans;
    __asm__ volatile ("csrr %[ans], sip" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_sip(uint64 new_val) {
    __asm__ volatile ("csrw sip, %[new_val]" : : [new_val] "r"(new_val));
}

inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus() {
    uint64 volatile ans;
    __asm__ volatile ("csrr %[ans], sstatus" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_sstatus(uint64 new_val) {
    __asm__ volatile ("csrw sstatus, %[new_val]" : : [new_val] "r"(new_val));
}

inline uint64 Riscv::r_scause() {
    uint64 volatile ans;
    __asm__ volatile ("csrr %[ans], scause" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_scause(uint64 new_val) {
    __asm__ volatile ("csrw scause, %[new_val]" : : [new_val] "r"(new_val));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile ans;
    __asm__ volatile ("csrr %[ans], sepc" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_sepc(uint64 new_val) {
    __asm__ volatile ("csrw sepc, %[new_val]" : : [new_val] "r"(new_val));
}

inline uint64 Riscv::r_stvec() {
    uint64 volatile ans;
    __asm__ volatile ("csrr %[ans], stvec" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_stvec(uint64 new_val) {
    __asm__ volatile ("csrw stvec, %[new_val]" : : [new_val] "r"(new_val));
}

inline uint64 Riscv::r_stval() {
    uint64 volatile ans;
    __asm__ volatile ("csrr %[ans], stval" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_stval(uint64 new_val) {
    __asm__ volatile ("csrw stval, %[new_val]" : : [new_val] "r"(new_val));
}

inline void Riscv::w_stack_a0(uint64 val) {
    __asm__ volatile ("sd %[val], 10 * 8(fp)" : : [val] "r"(val));
}

inline void Riscv::w_a0(uint64 val) {
    __asm__ volatile ("mv a0, %[val]" : : [val] "r"(val));
}

inline uint64 Riscv::r_a0() {
    uint64 volatile ans;
    __asm__ volatile ("ld %[ans], 10 * 8(fp)" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_a1(uint64 val) {
    __asm__ volatile ("mv a1, %[val]" : : [val] "r"(val));
}

inline uint64 Riscv::r_a1() {
    uint64 volatile ans;
    __asm__ volatile ("ld %[ans], 11 * 8(fp)" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_a2(uint64 val) {
    __asm__ volatile ("mv a2, %[val]" : : [val] "r"(val));
}

inline uint64 Riscv::r_a2() {
    uint64 volatile ans;
    __asm__ volatile ("ld %[ans], 12 * 8(fp)" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_a3(uint64 val) {
    __asm__ volatile ("mv a3, %[val]" : : [val] "r"(val));
}

inline uint64 Riscv::r_a3() {
    uint64 volatile ans;
    __asm__ volatile ("ld %[ans], 13 * 8(fp)" : [ans] "=r"(ans));
    return ans;
}

inline void Riscv::w_a4(uint64 val) {
    __asm__ volatile ("mv a4, %[val]" : : [val] "r"(val));
}

inline uint64 Riscv::r_a4() {
    uint64 volatile ans;
    __asm__ volatile ("ld %[ans], 14 * 8(fp)" : [ans] "=r"(ans));
    return ans;
}

#endif