#include "../h/ListTCB.hpp"

void ListTCB::initializeList() {
    head = nullptr;
    tail = nullptr;
}

ListTCB& ListTCB::addLast(TCB* x) {
    Elem* tmp = (Elem*)MemoryAllocator::memoryAllocate(sizeof(Elem));
    tmp->data = x, tmp->next = 0;
    if (tail) {
        tail->next = tmp;
        tail = tmp;
    } else {
        head = tmp;
        tail = tmp;
    }
    return *this;
}

TCB* ListTCB::removeFirst() {
    if (!head) return 0;
    Elem* tmp = head;
    head = head->next;
    if (!head) tail = nullptr;
    TCB* ans = tmp->data;
    MemoryAllocator::memoryFree(tmp);
    return ans;
}