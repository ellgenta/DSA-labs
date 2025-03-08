#include <stdlib.h>
#include <assert.h>

typedef struct _node{
    int data;
    struct _node* prev;
    struct _node* next;
} node;

typedef struct _linked_list{
    node* front;
    node* rear;
} linked_list;

void push(linked_list* queue,int value){
    node* new_rear=calloc(1,sizeof(node));
    if(!new_rear)
        return;

    new_rear->data=value;
    if(queue->front){
        new_rear->prev=queue->rear;
        queue->rear->next=new_rear;
    }
    else
        queue->front=new_rear;
    
    queue->rear=new_rear;

}

void pop(linked_list* queue){
    if(!queue->front)
        return;
    if(queue->front==queue->rear){
        free(queue->front);
        queue->front=NULL;
        queue->rear=NULL;
        return;
    }

    node* aux_ptr=queue->front->next;
    free(queue->front);
    queue->front=aux_ptr;
    queue->front->prev=NULL;

}

void test_push(){
    linked_list q={0};

    push(&q,1);
    assert(q.rear);
    assert(q.front==q.rear);
    assert(!q.rear->next);
    assert(!q.rear->prev);
    assert(q.rear->data==1);

    push(&q,2);
    assert(q.rear!=q.front);
    assert(q.front->next==q.rear);
    assert(q.rear->prev=q.front);
    assert(q.front->data==1);
    assert(!q.front->prev);
    assert(!q.rear->next);

    free(q.rear);
    free(q.front);
}

void test_pop(){
    linked_list q={calloc(1,sizeof(node)),calloc(1,sizeof(node))};
    assert(q.front);
    assert(q.rear);
    q.front->next=q.rear;
    q.rear->prev=q.front;
    q.front->data=1;
    q.rear->data=2;

    pop(&q);
    assert(q.front==q.rear);
    assert(q.front->data==2);
    assert(!q.front->prev);
    assert(!q.front->next);

    pop(&q);
    assert(!q.front);
    assert(!q.rear);
}

int main(){

    test_push();
    test_pop();
    
    return 0;
}
