#include <stdio.h>
#include <stdlib.h>

#include "lwp.h"

//Linked list
typedef struct rrQueue {
    thread head;
    thread tail;
    int size;
} rrQueue;

//Global queue
rrQueue q = {NULL, NULL, 0};


//Adds in a thread into the scheduler
void rrAdmit(thread new){

    //Ensure "next" pointer points to NULL 
    //as it indicates the tail
    new -> sched_one = NULL;

    //If queue is empty, make new thread 
    //both the head and tail
    if (q.size == 0){
        q.head = new;
        q.tail = new;
    } else {
        //Else, add it to the end of the queue
        q.tail -> sched_one = new;
        q.tail = new;
    }

    q.size++;
}


//Removes a thread from the scheduler
void rrRemove(thread victim){

    //If the thread is the head, 
    //move head pointer to next thread 
    //and decrement size
    if (q.head == victim){
        q.head = q.head -> sched_one;
        q.size--;
        return;
    } else {

        //Make pointers for the delinking process
        thread prevThread = q.head;
        thread currThread = q.head -> sched_one;

        //Iterate through queue
        while(currThread != NULL){

            //If victim is found, delink it
            if(currThread  == victim){
                thread threadToRemove = currThread;
                prevThread -> sched_one = currThread -> sched_one;
                q.size--;
                return;
            } else {
                //Else, try next thread
                prevThread = currThread;
                currThread = currThread -> sched_one;
            }
        }

        //Error message?

    }
    
}

//Return next thread to run, 
//put it back in queue
thread rrNext(void){

    //If queue is empty return NULL
    if (q.head == NULL){
        return NULL;
    }

    //If there is only one,
    //return thread, do nothing more
    if (q.size == 1){
        return q.head;
    }

    //Move thread to the back 
    //Update head pointer
    //Return appropriate thread
    thread newHead = q.head -> sched_one; 
    q.tail -> sched_one = q.head;
    q.tail = q.head;
    q.head = newHead;

    return q.tail;

}


//Return length of queue
int rrQLen(void){
    return q.size;
}
