#include "../h/ListSleepTCB.hpp"

void ListSleepTCB::decreaseTime() {
    if (!head) return;
    head->relTime--;
    if (!head->relTime) awake();
}
ListSleepTCB& ListSleepTCB::add(TCB* x, time_t t) {
    Elem* tmp = (Elem*)MemoryAllocator::memoryAllocate(sizeof(Elem));
    tmp->data = x;
    tmp->relTime = t;
    tmp->next = nullptr;
    Elem* prev = nullptr;
    for (Elem* cur = head; cur && cur->relTime <= tmp->relTime; cur = cur->next) {
        prev = cur;
        tmp->relTime -= cur->relTime;
    }
    if (prev == nullptr) {
        tmp->next = head;
        if (head) head->relTime -= tmp->relTime;
        head = tmp;
        if (tmp->next == nullptr) tail = tmp;
    } else {
        tmp->next = prev->next;
        if (prev->next) prev->next->relTime -= tmp->relTime;
        prev->next = tmp;
        if (tmp->next == nullptr) tail = tmp;
    }
    /*for (Elem* cur = head; cur; cur = cur->next) {
        printStr((const char*)cur->data);
        printStr("\n");
        printInt(cur->relTime);
        printStr("\n");
    }*/
    return *this;
}
void ListSleepTCB::awake() {
    while (head != nullptr && head->relTime == 0) {
        Elem* tmp = head;
        head = head->next;
        tmp->data->setSleeping(false);
        Scheduler::put(tmp->data);
        MemoryAllocator::memoryFree(tmp);
    }
    if (!head) tail = nullptr;
}