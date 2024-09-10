# riscV-kernel

This project is meant to create operating system for RiscV processor that is emulated by QEMU. The operating system relies on `lib/hw.lib` which provides basic interfaces for communicating with the host operating system. The functionalities of operating system are showcased through C and C++ API.

## Operating System Functionalities

### Heap Memory Allocator

- **Allocation:** Continuous and dynamic memory allocation
- **Deallocation:** Allocated memory can be freed, used to prevent memory leaks

### Threads

- **Multithreading:** The OS supports multiple threads
- **Scheduling Algorithm:** FIFO scheduler
- **Context switching:**
  - **Synchronous Context Switching:** Threads can perform predictable and controlled context switching
  - **Asynchronous context Switching:** Threads, triggered by an event, can perform context switching at any time
- **Thread Joining:** Threads can synchronize with one another by getting joined

### Timer

- **Preemptive Multitasking:** Each thread is given a time slice, once it expires OS performs **Context Switching**
- **Thread Sleeping:** Threads can be put to sleep for a specified amount of time units

### Semaphores

- **Wait:** Threads can acquire a semaphore
- **Signal:** Threads can release a semaphore

Note: _This are general purpose semaphores_

### Console I/O

- **getc:** Read character input from the console
- **putc:** Write character output to the console
