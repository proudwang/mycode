#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>


char queue[102400];
int pos = 0;

ucontext_t ctx_put, ctx_get, ctx_main;

static void put2queue()
{
    printf("put one char to queue...\n");
    queue[pos++] = 'a';
    if(swapcontext(&ctx_put, &ctx_get) < 0)
    {
        perror("swap context put to get error");
    }
}

static void get8queue()
{
    printf("get one char frome queue[%c]...\n", queue[pos--]);
    if(swapcontext(&ctx_get, &ctx_main) < 0)
    {
        perror("swap context get to main error");
    }
}

int main()
{
    char getstack[1024*10];
    char putstack[1024*10];

    if(getcontext(&ctx_put) < 0)
    {
        perror("getcontext error");
        return -1;
    }
    ctx_put.uc_stack.ss_sp = putstack;
    ctx_put.uc_stack.ss_size = sizeof(putstack);
    ctx_put.uc_link = &ctx_get;
    makecontext(&ctx_put, put2queue, 0);

    if(getcontext(&ctx_get) < 0)
    {
        perror("getcontext error");
        return -1;
    }
    ctx_get.uc_stack.ss_sp = getstack;
    ctx_get.uc_stack.ss_size = sizeof(getstack);
    ctx_get.uc_link = &ctx_main;
    makecontext(&ctx_get, get8queue, 0);

    sleep(1);
    printf("queue[%s]\n", queue);
    if(swapcontext(&ctx_main, &ctx_put) < 0)
    {
        perror("swap context main to put error");
    }

    return 0;
}

