#ifndef _memory_allocator_hpp_
#define _memory_allocator_hpp_

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static void initMemoryAllocator();
    static void* memoryAllocate(size_t s);
    static int memoryFree(void* p);
    struct FreeBlocks {
        size_t size;
        FreeBlocks* next;
        //size_t pad1, pad2;
    };
private:

    static FreeBlocks* head;
    static void mergeFreeBlocks(FreeBlocks* x);
};

#endif