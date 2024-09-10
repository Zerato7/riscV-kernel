#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

void printStr(char const* string) {
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    while (*string != '\0') {
        Riscv::putBuff->put(*string);
        string++;
    }
    Riscv::w_sstatus(sstatus);
}

void printInteger(uint64 integer) {
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    static char digits[] = "0123456789";
    char buf[16];
    int i = 0, neg = 0;

    if (integer < 0) {
        neg = 1;
        integer = -integer;
    }

    do {
        buf[i++] = digits[integer % 10];
    } while ((integer /= 10) != 0);

    if (neg) buf[i++] = '-';

    while (--i >= 0) {
        Riscv::putBuff->put(buf[i]);
    }

    Riscv::w_sstatus(sstatus);
}