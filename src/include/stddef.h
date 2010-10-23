#ifndef __STDDEF_H
#define __STDDEF_H


#define offsetof(type,member) ((unsigned long) &(((type*)0)->member))

typedef unsigned long size_t;

#define NULL (void*) 0


//dosn't belong here i think

typedef char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned long long qword;


#endif // __STDDEF_H
