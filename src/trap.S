.extern _ZN5Riscv10handleIntrEv
.extern _ZN5Riscv11handleTimerEv
.extern _ZN5Riscv13handleConsoleEv
.extern _ZN3TCB11user_to_sysEv
.extern _ZN3TCB11sys_to_userEv

.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function

_ZN5Riscv14supervisorTrapEv:
    j hIntr
    j hTimer
    j nista
    j nista
    j nista
    j nista
    j nista
    j nista
    j nista
    j hConsole

hIntr:
    addi sp, sp, -32
    sd fp, 3 * 8(sp)
    sd ra, 2 * 8(sp)
    sd a4, 1 * 8(sp)
    sd a5, 0 * 8(sp)

    call _ZN3TCB11user_to_sysEv

    addi sp, sp, -256
    .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv10handleIntrEv

    .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    call _ZN3TCB11sys_to_userEv

    ld a5, 0 * 8(sp)
    ld a4, 1 * 8(sp)
    ld ra, 2 * 8(sp)
    ld fp, 3 * 8(sp)
    addi sp, sp, 32
    sret

hTimer:
    addi sp, sp, -32
    sd fp, 3 * 8(sp)
    sd ra, 2 * 8(sp)
    sd a4, 1 * 8(sp)
    sd a5, 0 * 8(sp)

    call _ZN3TCB11user_to_sysEv

    addi sp, sp, -256
    .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv11handleTimerEv

    .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    call _ZN3TCB11sys_to_userEv

    ld a5, 0 * 8(sp)
    ld a4, 1 * 8(sp)
    ld ra, 2 * 8(sp)
    ld fp, 3 * 8(sp)
    addi sp, sp, 32
    sret

hConsole:
    addi sp, sp, -32
    sd fp, 3 * 8(sp)
    sd ra, 2 * 8(sp)
    sd a4, 1 * 8(sp)
    sd a5, 0 * 8(sp)

    call _ZN3TCB11user_to_sysEv

    addi sp, sp, -256
    .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv13handleConsoleEv

    .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    call _ZN3TCB11sys_to_userEv

    ld a5, 0 * 8(sp)
    ld a4, 1 * 8(sp)
    ld ra, 2 * 8(sp)
    ld fp, 3 * 8(sp)
    addi sp, sp, 32
    sret

nista: