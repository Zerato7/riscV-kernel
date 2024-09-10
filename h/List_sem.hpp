#ifndef _List__sem_hpp_
#define _List__sem_hpp_


class _sem;

class List_sem {
private:
    struct Elem {
        _sem* data;
        Elem* next;

        Elem(_sem* d, Elem* n) : data(d), next(n) {}
    } *head, *tail;
public:
    List_sem() : head(0), tail(0) {}

    List_sem(const List_sem&) = delete;
    List_sem& operator =(const List_sem&) = delete;

    List_sem& addLast(_sem* x);

    _sem* removeFirst();

};

#endif