#include <stdio.h>
#include <stdlib.h>

#include "lwp.h"

//Linked list node for threads
typedef struct threadNode {
    thread t;  
    struct threadNode* next;
} threadNode;

//Linked list
typedef struct rrQueue {
    threadNode* head;
    threadNode* tail;
    int size;
} rrQueue;

//Global queue
rrQueue q = {NULL, NULL, 0};

// void addToQueue(rrQueue *queue, thread new){
//     threadNode *newNode = (threadNode *) malloc(sizeof(threadNode));

//     &newNode = {new, NULL};
//     if (queue -> size == 0){
//         queue -> head = newNode;
//         queue -> tail = newNode;
//     } else {
//         queue -> tail -> next = newNode;
//         queue -> tail = newNode;
//     }

//     queue -> size++;
// }

// void dequeue(rrQueue *queue){
//     if( )
// }

//init and shutdown have null values

//
void rrAdmit(thread new){
    threadNode *newNode = (threadNode *) malloc(sizeof(threadNode));

    *newNode = (threadNode) {new, NULL};
    if (q.size == 0){
        q.head = newNode;
        q.tail = newNode;
    } else {
        q.tail -> next = newNode;
        q.tail = newNode;
    }

    q.size++;
}

void rrRemove(thread victim){

    if (q.head -> t == victim){
        threadNode *nodeToRemove = q.head;
        q.head = q.head -> next;
        free(nodeToRemove); 
        q.size--;
        return;
    } else {
        threadNode *prevNode = q.head;
        threadNode *currNode = q.head -> next;
        while(currNode != NULL){
            if(currNode -> t == victim){
                threadNode *nodeToRemove = currNode;
                prevNode -> next = currNode -> next;
                free(nodeToRemove);
                q.size--;
                return;
            } else {
                prevNode = currNode;
                currNode = currNode -> next;
            }
        }

        //Error message?

    }
    
}

thread rrNext(void){
    if (q.head == NULL){
        return NULL;
    }
    if (q.size == 1){
        return q.head -> t;
    }

    threadNode *newHead = q.head -> next; 
    q.tail -> next = q.head;
    q.tail = q.head;
    q.head = newHead;

    return q.tail -> t;

}

int rrQLen(void){
    return q.size;
}
