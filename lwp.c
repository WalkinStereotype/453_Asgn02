#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <unistd.h>

#include "lwp.h"

#define STACK_SIZE 16384

/*initialize scheduler (global)*/
scheduler currentSched = &RoundRobin;
/*global pointer to current thread*/
thread currentThread = NULL;




/* lwp functions */

tid_t lwp_create(lwpfun func, void *arg){
    
    //Allocate memory for new context
    thread newThread = malloc(sizeof(thread));
    //Check if allocation fails
    if(newThread == NULL){
        return NO_THREAD;
    }

    //Allocate stack using mmap
    /*TODO: use sysconfig to find _SC_PAGE_SIZE
    and use getrlimit to find RLIMIT_STACK
    if RLIMIT_STACK does not exist or is RLIM_INFINITY
    choose reasonable stack size 
    (8MB specified in assignment spec)*/
    unsigned long newStack = (unsigned long) 
        mmap(NULL, STACK_SIZE, 
            PROT_READ | PROT_WRITE, 
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, 
            -1, 0);
    //error checking for mmap call
    if(mmap == MAP_FAILED)
        return NO_THREAD; 
<<<<<<< HEAD

    /* Initialize the stack frame and context so that when 
    that context is loaded in swap rfiles(),
    it will properly return to the lwp’s function with 
    the stack and registers arranged as it will expect*/

    context newContext; /*create new context*/

    /*TODO: set tid to the current counter value then increment the counter*/
    newContext.tid;

    /*set stack to new stack initialized*/
    newContext.stack = newStack; 

    /*sets context stacksize to size of stack*/
    newContext.stacksize = STACK_SIZE; 

    // newContext.state. 
    /*TODO: set state to what it needs to be*/

    /*TODO: initialize lib_one and two and sched_one and two if needed*/

    /*TODO: initialize exited if needed*/

    /*admit the new thread into the scheduler*/
    lwp_get_scheduler()->admit(newThread);
=======

    /* Initialize the stack frame and context so that when 
    that context is loaded in swap rfiles(),
    it will properly return to the lwp’s function with 
    the stack and registers arranged as it will expect*/

    context newContext; /*create new context*/

    /*TODO: set tid to the current counter value then increment the counter*/
    newContext.tid;

    /*set stack to new stack initialized*/
    newContext.stack = newStack; 

    /*sets context stacksize to size of stack*/
    newContext.stacksize = STACK_SIZE; 

    newContext.state; /*TODO: set state to what it needs to be*/

    /*TODO: initialize lib_one and two and sched_one and two if needed*/

    /*TODO: initialize exited if needed*/

    /*admit the new thread into the scheduler*/
    rradmit(newThread);
}

void  lwp_exit(int status){
    /*Terminates the current LWP and yields to whichever
     thread the scheduler chooses. lwpexit() does not return*/
}

tid_t lwp_gettid(void){
    /*Returns the tid of the calling LWP
     or NOTHREAD if not called by a LWP*/
}

void  lwp_yield(void){
    /*Yields control to another LWP.Which one depends on the
     scheduler. Saves the current LWP’s context, picks the 
     next one, restores that thread’s context, and returns.
     If there is no next thread, terminates the program*/
>>>>>>> 8c3003f1fea63007077fa58c474b98677046441d
}

void  lwp_start(void){
    /*Starts the LWP system. Converts the calling thread 
    into a LWP and lwp yield()s to whichever thread
    the scheduler chooses.*/
<<<<<<< HEAD

    /*TODO: conver calling thread into a LWP*/

    /*yeild to next thread*/
}

void  lwp_yield(void){
    /*Yields control to another LWP.Which one depends on the
     scheduler. Saves the current LWP’s context, picks the 
     next one, restores that thread’s context, and returns.
     If there is no next thread, terminates the program*/

    /*yield to LWP returned by lwp_get_scheduler()->next*/
    /*TODO: set currentThread = next*/

    /*if no next thread exit(3) with termination status of LWP*/
    //TODO exit()
}

void  lwp_exit(int status){
    /*Terminates the current LWP and yields to whichever
     thread the scheduler chooses. lwpexit() does not return*/

    scheduler sched = lwp_get_scheduler();

    /*remove current LWP*/
    //TODO: add victim to remove sched->remove();
    sched->remove(currentThread);

    /*swap to the next LWP */
    //TODO: currentContext =
    thread nextThread = sched->next;
    //swap exited lwp state with next State
    swap_rfiles(&currentThread->state, &nextThread->state)
    //does not return
}

tid_t lwp_wait(int *status){
    /*Waits for a thread to terminate, deallocates its 
    resources, and reports its termination status if 
    status is non-NULL. Returns the tid of the terminated
    thread or NOTHREAD.*/


    /*check for terminated threads in list of exited threads
    and return them in FIFO order*/

    /*if there are no terminated threads block by descheduling
    it and place it on a queue of waiting threads*/

    /*if qlen is not greater than one then return NO_THREAD*/
    if(lwp_get_scheduler()->qlen <= 1){
        return NO_THREAD;
    }
}

tid_t lwp_gettid(void){
    /*Returns the tid of the calling LWP
     or NO_THREAD if not called by a LWP*/
    
    /*return tid of LWP*/

    /*return NO_THREAD if not called by a LWP
    (sched->next returns null)*/
    if(lwp_get_scheduler()->next == NULL){
        return NO_THREAD;
    }
=======
}

tid_t lwp_wait(int *status){
    /*Waits for a thread to terminate, deallocates its 
    resources, and reports its termination status if 
    status is non-NULL. Returns the tid of the terminated
    thread or NOTHREAD.*/


    /*check for terminated threads and return them in FIFO order*/

    /*if there are no terminated threads block by descheduling
    it and place it on a queue of waiting threads*/

    /*if qlen is not greater than one then return NO-THREAD*/
}

void  lwp_set_scheduler(scheduler fun){
    /* Causes the LWP package to use the given scheduler
    to choose the next process to run. Transfers all 
    threads from the old scheduler to the new one in next()
     order. If scheduler is NULL the library should return 
     to round-robin scheduling.*/
}

scheduler lwp_get_scheduler(void){
    /*Returns the pointer to the current scheduler.*/
>>>>>>> 8c3003f1fea63007077fa58c474b98677046441d
}

thread tid2thread(tid_t tid){
    /*Returns the thread corresponding to the given 
    threadID, or NULL if the ID is invalid*/
<<<<<<< HEAD

    /*get the thread using ID*/

    /*if ID is invalid return null*/
}

void  lwp_set_scheduler(scheduler sched){
    /* Causes the LWP package to use the given scheduler
    to choose the next process to run. Transfers all 
    threads from the old scheduler to the new one in next()
     order. If scheduler is NULL the library should return 
     to round-robin scheduling.*/
    
}

scheduler lwp_get_scheduler(void){
    /*Returns the pointer to the current scheduler.*/
    return currentSched;
=======
>>>>>>> 8c3003f1fea63007077fa58c474b98677046441d
}

void swap_rfiles(rfile *old, rfile *new){

}