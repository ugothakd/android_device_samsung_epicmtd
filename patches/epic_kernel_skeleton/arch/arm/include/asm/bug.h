#ifndef _ASMARM_BUG_H
#define _ASMARM_BUG_H


extern void __bug(const char *file, int line) __attribute__((noreturn));

/* give file/line information */
#define BUG()		__bug(__FILE__, __LINE__)


#define HAVE_ARCH_BUG

#include <asm-generic/bug.h>

#endif
