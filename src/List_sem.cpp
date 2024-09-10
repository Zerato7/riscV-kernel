#include "../h/MemoryAllocator.hpp"
#include "../h/List_sem.hpp"

List_sem& List_sem::addLast(_sem* x) {
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

_sem* List_sem::removeFirst() {
    if (!head) return 0;
    Elem* tmp = head;
    head = head->next;
    if (!head) tail = nullptr;
    _sem* ans = tmp->data;
    MemoryAllocator::memoryFree(tmp);
    return ans;
}