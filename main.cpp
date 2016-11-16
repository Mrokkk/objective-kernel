#include <linkage.h>

#define __noreturn __attribute__((noreturn))

asmlinkage __noreturn void main()
{
    while (1);
}

