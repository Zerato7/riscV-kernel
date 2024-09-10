#include "../h/_Buffer.hpp"
//#include "../lib/console.h"
#include "../h/print.hpp"

_Buffer* _Buffer::create_Buffer(unsigned init) {
    //__putc('d');
    _Buffer* tmp = (_Buffer*)MemoryAllocator::memoryAllocate(sizeof(_Buffer));
    _sem::create_Sem(&tmp->itemAvailable, 0);
    _sem::create_Sem(&tmp->mutex, 1);
    _sem::create_Sem(&tmp->spaceAvailable, init);
    tmp->cnt = 0;
    tmp->head = 0;
    tmp->tail = 0;
    tmp->cap = init;
    tmp->arr = (char*)MemoryAllocator::memoryAllocate(sizeof(char) * init);
    return tmp;
}

void _Buffer::put(char x) {
    /*__putc('Z');
    spaceAvailable->_sem_wait();
*/     //__putc('G');
    char volatile m = x;
    spaceAvailable->_sem_wait();
    mutex->_sem_wait();
    arr[tail] = m;
    tail = (tail + 1) % cap;
    cnt++;
    mutex->_sem_signal();
    itemAvailable->_sem_signal();
}

char _Buffer::get() {
    itemAvailable->_sem_wait();
    mutex->_sem_wait();
    char ans = arr[head];
    head = (head+1) % cap;
    --cnt;
    mutex->_sem_signal();
    spaceAvailable->_sem_signal();
    return ans;
}

_Buffer::~_Buffer() {
    MemoryAllocator::memoryFree(arr);
    itemAvailable->clean();
    MemoryAllocator::memoryFree(itemAvailable);
    mutex->clean();
    spaceAvailable->clean();
    MemoryAllocator::memoryFree(mutex);
    MemoryAllocator::memoryFree(spaceAvailable);
    /*delete itemAvailable;
    delete mutex;
    delete spaceAvailable;*/
}