#ifndef TUPLE_H
#define TUPLE_H

typedef struct tuple {
    int a,b,c;
} Tuple;

int First(Tuple);
int Second(Tuple);
int Third(Tuple);
void Set(Tuple*,int,int,int);
int compTuple(const void*,const void*);

#endif
