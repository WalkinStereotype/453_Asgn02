#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <unistd.h>

#include "lwp.h"
#include "roundRobinScheduler.c"

#define STACK_SIZE 16384
#define PREV_THREAD lib_one
#define NEXT_THREAD lib_two
#define EXITED_NEXT exited

/*initialize scheduler (global)*/
scheduler currentSched = &rrPublish;
/*global pointer to current thread*/
thread currentThread = NULL;
/*threadID counter*/
tid_t threadCounter = 0;
/* threadHead and threadTail for doubly linked list */
/* Used for switching between tid and thread */
thread runningHead = NULL;
thread runningTail = NULL;
thread exitedHead;




/* lwp functions */

static void lwp_wrap(lwpfun fun, void *arg) {
    /* Call the given lwpfunction with the given argument.
    * Calls lwp exit() with its return value
    */
    int rval;
    rval = fun(arg);
    lwp_exit(rval);
}

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
    unsigned long *stackPointer =  
        (unsigned long *)mmap(NULL, STACK_SIZE, 
            PROT_READ | PROT_WRITE, 
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, 
            -1, 0);
    //error checking for mmap call
    if(stackPointer == MAP_FAILED){
        return NO_THREAD; 
    }

    /* Initialize the stack frame and context so that when 
    that context is loaded in swap rfiles(),
    it will properly return to the lwp’s function with 
    the stack and registers arranged as it will expect*/


    /*set tid to the current counter value then increment the counter*/
    newThread -> tid = ++threadCounter;

    /*TODO: initialize lib_one and two and sched_one and two if needed*/
    if(runningHead == NULL){
        //Initialize list
        runningHead = newThread;
        runningTail = newThread;

        //Set prev and next pointers
        newThread -> PREV_THREAD = NULL;
        newThread -> NEXT_THREAD = NULL;
    } else {

        //Else, update the tail
        runningTail -> NEXT_THREAD = newThread;
        newThread -> PREV_THREAD = runningTail;
        runningTail = newThread;
    }

    //Initialize thread's status
    newThread -> status = LWP_LIVE;

    /*set stack to new stack initialized*/
    newThread -> stack = stackPointer;

    /*sets context stacksize to size of stack*/
    newThread -> stacksize = STACK_SIZE; 
 

    /* set state to what it needs to be */
    //Threads are never called, so 
    // RBP and return address are not needed
    unsigned long topOfStack = 
        (unsigned long)((unsigned long)(newThread -> stack) 
        + (newThread -> stacksize));

    //Ensure it is aligned by 16
    topOfStack = ((topOfStack + 15) & -16);
    
    
    //Put function as return address
    unsigned long *topPointer = (unsigned long *)topOfStack;
    topPointer--;

    *topPointer = (unsigned long)lwp_wrap;
    
    //Put a filler base pointer
    topPointer--;
    *topPointer = 127;

    //Set rbp and rsp
    newThread -> state.rbp = topOfStack;
    newThread -> state.rsp = topOfStack;
    newThread -> state.rdi = func;
    newThread -> state.rsi = arg;



    //Initialize FPU
    newThread -> state.fxsave = FPU_INIT;


    /*TODO: initialize exited if needed*/
    newThread -> exited = NULL;

    /*admit the new thread into the scheduler*/
    lwp_get_scheduler() -> admit(newThread);

    // Return thread id
    return newThread -> tid;
}


void  lwp_start(void){
    ////printf"\n in start");
    /*Starts the LWP system. Converts the calling thread 
    into a LWP and lwp yield()s to whichever thread
    the scheduler chooses.*/

    /*TODO: conver calling thread into a LWP*/
    thread newThread = (thread) malloc(sizeof(context));
    newThread -> tid = ++threadCounter;
    newThread -> status = LWP_LIVE;
    newThread -> stack = NULL;

    lwp_get_scheduler()->admit(newThread);
    //Check if allocation fails
    if(newThread == NULL){
        return NO_THREAD;
    }


    /*yeild to next thread*/
    lwp_yield();
    return;
}

void  lwp_yield(void){
    ////printf"\nin yield");
    /*Yields control to another LWP.Which one depends on the
     scheduler. Saves the current LWP’s context, picks the 
     next one, restores that thread’s context, and returns.
     If there is no next thread, terminates the program*/

    /*yield to LWP returned by lwp_get_scheduler()->next*/
    /*TODO: set currentThread = next*/

    /*if no next thread exit(3) with termination status of LWP*/
    thread nextThread = lwp_get_scheduler()->next();
    //printf("got next\n");
    if(nextThread == NULL){
        //printf("exiting1");
        exit(currentThread->status); //TODO exit with correct status
    }
    //printf("not exiting");
    /*yield to LWP returned by lwp_get_scheduler()->next*/
    //printf("here0");
    // //printf("swapping(%p and %p)"
    // ,(&currentThread->state, &nextThread->state));
    // exit(1);
    swap_rfiles(NULL, NULL);
    rfile *old;
    rfile *new;
    if(currentThread == NULL || currentThread->stack == NULL){
        old = NULL;
    } else {
        old = &currentThread->state;
    }
    if(nextThread == NULL || nextThread->stack == NULL){
        new = NULL;
    } else {
        new = &nextThread->state;
    }
    swap_rfiles(old, new);
    ////printf"rfiles swaped");
    //printf("exited after swaprfiles");
    // exit(1);
    currentThread = nextThread;
    return;
}

/*Terminates the current LWP and yields to whichever
 thread the scheduler chooses. lwpexit() does not return*/
void  lwp_exit(int status){
    /*Terminates the current LWP and yields to whichever
     thread the scheduler chooses. lwpexit() does not return*/

    scheduler sched = lwp_get_scheduler();

    /*remove current LWP*/
    /*remove current thread from scheduler*/
    lwp_get_scheduler()->remove(currentThread);
    
    /*update status*/
    currentThread->status = LWP_TERM;
    /*TODO: check if any threads are waiting*/
    if(exitedHead == NULL){
        struct threadinfo_st exitedHeadInit;
        exitedHeadInit.tid = -1;
        exitedHeadInit.exited = NULL;
        exitedHead = &exitedHeadInit;
    }

    if(exitedHead->exited == NULL){
        /*nothing is waiting or has exited*/
        exitedHead->exited = currentThread;
        exitedHead->status = LWP_TERM;
    } else if(exitedHead->status == LWP_LIVE){
        /*other lwp(s) are waiting, match with first waiting lwp*/
        thread waitThread = exitedHead->exited; //first waiting thread
        //remove thread from list
        exitedHead->exited = exitedHead->exited->exited;
        waitThread->exited = NULL; //reset exited now that lwp is remove
        lwp_get_scheduler()->admit(waitThread); //readmit waiting thread
    } else {
        /*exited lwp(s) are on queue, add current to the end */
        thread temp = exitedHead;
        while(temp->exited != NULL){
            temp = temp->exited;
        }
        /*adds waiting thread to end of the queue*/
        temp->exited = currentThread;
    }

    /*swap to the next LWP */
    lwp_yield();
    //TODO: currentContext =
    // thread nextThread = sched -> next();
    // //swap exited lwp state with next State
    // if(currentThread == NULL){
    //     //printf("here1");
    //     //printf("\nswapping(NULL and %p)", &nextThread->state);
    //     swap_rfiles(NULL, &nextThread->state);
    // }else {
    //     //printf("\nhere2");
    //     //printf("\nswapping(%p and %p)",
    //     //  (&currentThread->state, &nextThread->state));
    //     swap_rfiles(&currentThread->state, &nextThread->state);
    // }
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
    
    if(exitedHead->exited == NULL){
        /*nothing is waiting or has exited*/
        lwp_get_scheduler()->remove(currentThread);
        exitedHead->exited = currentThread;
        exitedHead->status = LWP_LIVE;
        lwp_yield();
    } else if(exitedHead->status == LWP_LIVE){
        /*other lwps are waiting, add to end of waiting queue
        then deschedule*/
        thread temp = exitedHead;
        while(temp->exited != NULL){
            temp = temp->exited;
        }
        /*adds waiting thread to end of the queue*/
        temp->exited = currentThread;
        lwp_get_scheduler()->remove(currentThread);
        lwp_yield();
    } else {
        /*exited lwp(s) are on queue. deallocates resources of exited thread
        and reports its termination status if non-NULL
        returns tid of terminated thread*/
        thread termThread = exitedHead->exited;
        exitedHead->exited = exitedHead->exited->exited;
        munmap(termThread->stack, termThread->stacksize);
        //TODO report termination status if non-NULL
        *status = termThread->status;
        return termThread->tid;

    }   
    /*if qlen is not greater than one then return NO_THREAD*/
    if(lwp_get_scheduler() -> qlen() <= 1){
        return NO_THREAD;
    }
}

tid_t lwp_gettid(void){

    //If there is no current thread then return NO_THREAD
    if (currentThread == NULL){
        return NO_THREAD;
    } else {
        //Else return currentThread's id
        return currentThread -> tid;
    }

}

thread tid2thread(tid_t tid){

    //Quick checks to see if tid is invalid
    if((tid <= 0) 
        || (tid >= threadCounter)
        || (runningHead == NULL)){

        return NULL;

    }

    //Iterator for the while loop
    thread threadIterator = runningHead;

    //While not at the end of the list
    while(threadIterator != NULL){

        //If we found the tid, return the thread
        if(threadIterator -> tid == tid){
            return threadIterator;
        }

        //Iterate to next
        threadIterator = threadIterator -> NEXT_THREAD;
    }

    //If we got this far, return NULL
    return NULL;


}

void  lwp_set_scheduler(scheduler sched){
    /* Causes the LWP package to use the given scheduler
    to choose the next process to run. Transfers all 
    threads from the old scheduler to the new one in next()
     order. If scheduler is NULL the library should return 
     to round-robin scheduling.*/
    
    if(sched == NULL){
        sched = RoundRobin;
    }

    if (sched != currentSched){
        while(currentSched -> qlen() > 0){
            thread threadToRemove = currentSched -> next();
            currentSched -> remove(threadToRemove);
            sched -> admit(threadToRemove);
        }

        currentSched = sched;
    }

}

scheduler lwp_get_scheduler(void){
    /*Returns the pointer to the current scheduler.*/
    if(currentSched == NULL){
        return RoundRobin;
    }
    return currentSched;
}

