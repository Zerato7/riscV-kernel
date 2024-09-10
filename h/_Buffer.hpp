#ifndef __Buffer_hpp_
#define __Buffer_hpp_

#include "../h/_sem.hpp"

class _Buffer {
private:
    _sem* itemAvailable;
    _sem* mutex;
    _sem* spaceAvailable;
    int head, tail, cnt, cap;
    char* arr;
public:
    static _Buffer* create_Buffer(unsigned init);

    void put(char x);

    char get();

    ~_Buffer();

};

#endif