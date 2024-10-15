#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <unistd.h>

#include "lwp.h"

#define STACK_SIZE 16384


/* lwp functions */

tid_t lwp_create(lwpfun func, void *arg){
    
    //Allocate memory for new context
    thread newThread = malloc(sizeof(thread));
    //Check if allocation fails
    if(newThread == NULL){
        return NO_THREAD;
    }

    //Allocate stack using mmap
    unsigned long newStack = (unsigned long) 
        mmap(NULL, STACK_SIZE, 
            PROT_READ | PROT_WRITE, 
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, 
            -1, 0);
}

void  lwp_exit(int status){

}

tid_t lwp_gettid(void){

}

void  lwp_yield(void){

}

void  lwp_start(void){

}

tid_t lwp_wait(int *){

}

void  lwp_set_scheduler(scheduler fun){

}

scheduler lwp_get_scheduler(void){

}

thread tid2thread(tid_t tid){

}

void swap_rfiles(rfile *old, rfile *new){

}