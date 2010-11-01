// stdarg.h - handle variable argument list
#ifndef __STDARG_H
#define __STDARG_H



typedef int *va_list;

#define va_size(type) \
	( sizeof(type) / (sizeof(typeof(va_list))+1) +1  )

#define va_start(ap, argN) \
	( ap = (typeof(va_list))&argN + va_size(argN) ) 

#define va_copy(dest, src) \
	( dest = src )

#define va_arg(ap, type) \
	( ap += va_size(type), *(type *)(ap - va_size(type)) )

#define va_end(ap)



#endif // __STDARG_H
