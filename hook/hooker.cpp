#include <stdio.h>

__attribute__((constructor))
void hook_begin()
{
    printf("start hook...\n");
}


__attribute__((destructor))
void hook_end()
{
    printf("end hook...\n");
}
