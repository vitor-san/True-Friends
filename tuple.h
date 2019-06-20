#ifndef TUPLE_H
#define TUPLE_H

typedef struct tuple {
    int a, b;
    double c;
} Tuple;

int First(Tuple);
int Second(Tuple);
double Third(Tuple);
void Set(Tuple*, int, int, double);
int compTuple(const void*, const void*);

#endif
