#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <unistd.h>

#include "lwp.h"
#include "roundRobinScheduler.c"

#define STACK_SIZE 16384

/*initialize scheduler (global)*/
scheduler currentSched = RoundRobin;
/*global pointer to current thread*/
thread currentThread = NULL;
/*threadID counter*/
tid_t threadCounter = 0;




/* lwp functions */

tid_t lwp_create(lwpfun func, void *arg){
    
    //Allocate memory for new context
    thread newThread = (thread) malloc(sizeof(context));
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

    /* Initialize the stack frame and context so that when 
    that context is loaded in swap rfiles(),
    it will properly return to the lwp’s function with 
    the stack and registers arranged as it will expect*/


    /*set tid to the current counter value then increment the counter*/
    newThread -> tid = ++threadCounter;

    //Initialize thread's status
    newThread -> status = LWP_LIVE;

    /*set stack to new stack initialized*/
    *(newThread -> stack) = newStack;

    /*sets context stacksize to size of stack*/
    newThread -> stacksize = STACK_SIZE; 

 
    /*set state to what it needs to be*/


    //Load argument into rdi register
    newThread -> state.rdi = *((unsigned long *) arg);

    //Threads are never called, so no old RBP or return address
    //So rbp = rsp 
    unsigned long topOfStack = 
        (unsigned long)((newThread -> stack) 
        + (newThread -> stacksize));

    newThread -> state.rbp = topOfStack;
    newThread -> state.rsp = topOfStack;

    //Initialize FPU
    newThread -> state.fxsave = FPU_INIT;


    /*TODO: initialize lib_one and two and sched_one and two if needed*/

    /*TODO: initialize exited if needed*/

    /*admit the new thread into the scheduler*/
    lwp_get_scheduler() -> admit(newThread);

    // Return thread id
    newThread -> tid;
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
}

void  lwp_start(void){
    /*Starts the LWP system. Converts the calling thread 
    into a LWP and lwp yield()s to whichever thread
    the scheduler chooses.*/

    /*TODO: conver calling thread into a LWP*/

    /*yeild to next thread*/
}

void  lwp_yield(void){
    /*Yields control to another LWP.Which one depends on the
     scheduler. Saves the current LWP’s context, picks the 
     next one, restores that thread’s context, and returns.
     If there is no next thread, terminates the program*/

    /*if no next thread exit(3) with termination status of LWP*/
    thread nextThread = lwp_get_scheduler()->next;
    if(nextThread == NULL){
        exit(currentThread->status); //TODO exit with correct status
    }
    
    /*yield to LWP returned by lwp_get_scheduler()->next*/
    swap_rfiles(&currentThread->state, &nextThread->state);
    currentThread = nextThread;
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
}

thread tid2thread(tid_t tid){
    /*Returns the thread corresponding to the given 
    threadID, or NULL if the ID is invalid*/

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
}

void swap_rfiles(rfile *old, rfile *new){

}
