#include "../h/syscall_cpp.hpp"

void* operator new(size_t size) {
    return mem_alloc(size);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete(void* rp) noexcept{
    mem_free(rp);
}

void operator delete[](void* rp) noexcept{
    mem_free(rp);
}

Thread::Thread(void (*body)(void*), void* arg) {
    this->body = body;
    this->arg = arg;
    myHandle = 0;
}

Thread::~Thread() {
    if (myHandle != nullptr) {
        myHandle->setFinished();
        delete myHandle;
    }
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

void Thread::threadWrapper(void* a) {
    ((Thread*)a)->run();
}

Thread::Thread() {
    body = Thread::threadWrapper;
    arg = (void*)this;
    myHandle = 0;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

void PeriodicThread::terminate() {
    period = 0;
}

PeriodicThread::PeriodicThread(time_t period) : Thread() {
    this->period = period;
}

void PeriodicThread::run() {
    while (period) {
        periodicActivation();
        sleep(period);
    }
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}