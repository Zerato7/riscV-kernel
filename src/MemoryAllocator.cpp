#include "../h/MemoryAllocator.hpp"

MemoryAllocator::FreeBlocks* MemoryAllocator::head = nullptr;

void MemoryAllocator::mergeFreeBlocks(FreeBlocks* x) {
    if (x == nullptr) return;
    if (x->next == nullptr) return;
    if ((char*)x + x->size * MEM_BLOCK_SIZE == (char*)x->next) {
        FreeBlocks* tmp = x->next;
        x->next = tmp->next;
        x->size += tmp->size;
    }
}

void MemoryAllocator::initMemoryAllocator() {
    head = (FreeBlocks*)HEAP_START_ADDR;
    head->size = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR) / MEM_BLOCK_SIZE;
    head->next = nullptr;
}

int MemoryAllocator::memoryFree(void* p) {
    if (p == nullptr) return 0;
    FreeBlocks* tmp = (FreeBlocks*)((char*)p - sizeof(FreeBlocks));
    FreeBlocks* cur = head, *prev = nullptr;
    while (cur && cur < tmp) {
        prev = cur;
        cur = cur->next;
    }
    if (!prev) {
        head = tmp;
    } else {
        prev->next = tmp;
    }
    tmp->next = cur;
    mergeFreeBlocks(tmp);
    mergeFreeBlocks(prev);
    return 0;
}

void* MemoryAllocator::memoryAllocate(size_t s) {
    s = (s + sizeof(FreeBlocks) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    FreeBlocks* prev = nullptr, *cur = head;
    while (cur && cur->size < s) {
        prev = cur;
        cur = cur->next;

    }
    if (!cur) return nullptr;
    FreeBlocks* ans;
    if (cur->size == s) {
        ans = cur;
        if (prev) prev->next = cur->next; else
            head = cur->next;
    } else {
        cur->size -= s;
        ans = (FreeBlocks*)((char*)cur + cur->size * MEM_BLOCK_SIZE);
        ans->size = s;
    }
    return (char*)ans + sizeof(FreeBlocks);
}