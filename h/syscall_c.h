#ifndef _syscall_c_h_
#define _syscall_c_h_

#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/_sem.hpp"

void* mem_alloc(size_t size);

int mem_free(void*);

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

void thread_join(thread_t handle);

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

int time_sleep(time_t);

const int EOF = -1;

char getc();

void putc(char);

#endif
