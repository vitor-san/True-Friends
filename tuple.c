#include "tuple.h"

void Set(Tuple* t, int a, int b, int c) {
    t->a = a;
    t->b = b;
    t->c = c;
}

int First(Tuple t) {
    return t.a;
}

int Second(Tuple t) {
    return t.b;
}

int Third(Tuple t) {
    return t.c;
}

int compTuple(const void* a,const void* b) {

    Tuple c = *((Tuple*) a);
    Tuple d = *((Tuple*) b);

    if (Third(c) < Third(d)) return -1;
    if (Third(c) > Third(d)) return 1;
    else return 0;

}
