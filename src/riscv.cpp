#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"

ListSleepTCB Riscv::listSleeping_threads;
List_sem Riscv::listClosed_sem;


bool Riscv::firstTime = false;

_Buffer* Riscv::getBuff = nullptr;
_Buffer* Riscv::putBuff = nullptr;

void Riscv::popSppSpie() {
    Riscv::mc_sstatus(Riscv::BitMaskSStatus::SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::initBuff() {
    getBuff = _Buffer::create_Buffer(_BUFFER_SIZE);

    putBuff = _Buffer::create_Buffer(_BUFFER_SIZE);}

void Riscv::handleIntr() {
    uint64 scause = r_scause();
    uint64 volatile a0 = r_a0();
    uint64 volatile a1 = r_a1();
    uint64 volatile a2 = r_a2();
    uint64 volatile a3 = r_a3();
    uint64 volatile a4 = r_a4();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        if (a0 == 0x01) { //void* mem_alloc(size_t size)
            uint64 tmp = (uint64)MemoryAllocator::memoryAllocate(a1 * MEM_BLOCK_SIZE);
            w_stack_a0(tmp);
        } else if (a0 == 0x02) { //int mem_free(void*)
            uint64 tmp = MemoryAllocator::memoryFree((uint64*)a1);
            w_stack_a0(tmp);
        } else if (a0 == 0x11) { //int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg, void* stack_space)
            thread_t* h = (thread_t*)a1;
            TCB::Body sr = (TCB::Body)a2;
            void* a = (void*)a3;
            char* ss = (char*)a4;
            if (TCB::createThread(h, sr, a, ss, 0) != nullptr) {
                Riscv::w_stack_a0(0);
            } else {
                Riscv::w_stack_a0(-1);
            }
        } else if (a0 == 0x12) { //int thread_exit()
            TCB::running->setFinished();
            TCB::dispatch();
        } else if (a0 == 0x13) { //void thread_dispatch()
            TCB::dispatch();
        } else if (a0 == 0x14) { //void thread_join(thread_t handle)
            thread_t h = (thread_t)a1;
            TCB::running->join(h);
        } else if (a0 == 0x21) { //int sem_open(sem_t* handle, unsigned init)
            sem_t* h = (sem_t*)a1;
            unsigned i = (unsigned)a2;
            if (_sem::create_Sem(h, i) != nullptr) {
                Riscv::w_stack_a0(0);
            } else {
                Riscv::w_stack_a0(-1);
            }
        } else if (a0 == 0x22) { //int sem_close(sem_t handle)
            sem_t h = (sem_t)a1;
            int r = h->_sem_close();
            Riscv::w_stack_a0((uint64)r);
        } else if (a0 == 0x23) { //int sem_wait(sem_t id)
            sem_t i = (sem_t)a1;
            int r = i->_sem_wait();
            Riscv::w_stack_a0((uint64)r);
        } else if (a0 == 0x24) { //int sem_signal(sem_t id)
            sem_t i = (sem_t)a1;
            int r = i->_sem_signal();
            Riscv::w_stack_a0((uint64)r);
        } else if (a0 == 0x31) { //int time_sleep(time_t)
            time_t t = (time_t)a1;
            if (t > 0) {
                Riscv::listSleeping_threads.add(TCB::running, t);
                    TCB::running->setSleeping(true);
                    TCB::dispatch();
            }
        } else if (a0 == 0x41) { //char getc()
            w_stack_a0((uint64)getBuff->get());
        } else if (a0 == 0x42) { //void putc(char)
            putBuff->put((char)a1);
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else {
        if (!firstTime) {
            uint64 volatile sstatus = Riscv::r_sstatus();
            uint64 volatile sepc = Riscv::r_sepc();
            printInteger(scause);
            printStr(" <-scause\n");
            printInteger(Riscv::r_sepc());
            printStr(" <-sepc\n");
            printInteger(Riscv::r_stval());
            printStr(" <-stval\n");
            Riscv::w_sstatus(sstatus);
            Riscv::w_sepc(sepc);
            firstTime = true;
        }
    }
}

void Riscv::handleTimer() {
    uint64 scause = r_scause();
    if (scause == 0x8000000000000001UL) {
        //printStr("timer\n");
        Riscv::listSleeping_threads.decreaseTime();
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::running->timeSlice <= TCB::timeSliceCounter) {
            uint64 volatile sstatus = r_sstatus();
            uint64 volatile sepc = r_sepc();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
}

void Riscv::handleConsole() {
    uint64 scause = r_scause();
    if (scause == 0x8000000000000009UL) {
        uint64 volatile sepc = Riscv::r_sepc();
        uint64 volatile sstatus = Riscv::r_sstatus();
        int plic = plic_claim();
        if (plic == CONSOLE_IRQ) {
            while (*((char*)CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {
                getBuff->put(*((char*)CONSOLE_RX_DATA));
            }
        }
        plic_complete(plic);
        Riscv::w_sepc(sepc);
        Riscv::w_sstatus(sstatus);
        Riscv::mc_sip(SIP_SSIP);
    }

}