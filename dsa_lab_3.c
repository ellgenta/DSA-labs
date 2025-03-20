#include <stdlib.h>
#include <assert.h>

typedef enum{
    false,
    true
} bool;

typedef struct _node{
    int data;
    struct _node* prev;
    struct _node* next;
} node;

typedef struct _linked_list{
    node* front;
    node* rear;
} linked_list;

typedef enum{
    SIG_DEF,
    SIG_OS,
    SIG_TRN,
    SIG_IC,
}signal;

typedef enum{
    NONE,
    INT,
    BOOL,
    ERROR
} type;

typedef struct opt{
    void *value;
    type t;
} option;

option push(linked_list* queue,int value){
    option res={0,NONE};

    node* new_rear=calloc(1,sizeof(node));
    if(!new_rear){
        res.value=SIG_OS;
        res.t=ERROR;
        return res;
    }

    new_rear->data=value;
    if(queue->front){
        new_rear->prev=queue->rear;
        queue->rear->next=new_rear;
    }
    else
        queue->front=new_rear;

    queue->rear=new_rear;

    return res;
}

option pop(linked_list* queue){
    option res={0,0};

    if(!queue->front)
        return res;
    if(queue->front==queue->rear){
        free(queue->front);
        queue->front=NULL;
        queue->rear=NULL;
        return res;
    }

    node* aux_ptr=queue->front->next;
    free(queue->front);
    queue->front=aux_ptr;
    queue->front->prev=NULL;

    return res;
}

option size(linked_list* queue){
    option res={0,INT};

    if(!queue->front && !queue->rear){
        res.value=0;
        return res;
    }

    node* aux_ptr=queue->front;

    int count=0;
    while(aux_ptr){
        aux_ptr=aux_ptr->next;
        ++count;
    }

    res.value=count;
    return res;
}

option empty(linked_list* queue){
    option res={0,BOOL};

    if(!queue->front && !queue->rear){
        res.value=1;
        res.t=BOOL;
    }

    return res;
}

option front(linked_list* queue){
    option res={0,INT};

    option r = empty(queue);
    if(r.t != NONE && (bool)r.value == true){
        res.t=ERROR;
        res.value=SIG_IC;
        return res;
    }
    else{
        res.value=queue->front->data;
        return res;
    }
}

option back(linked_list* queue){
    option res={0,INT};

    if((bool)empty(queue).t){
        res.t=ERROR;
        res.value=SIG_IC;
        return res;
    }
    else{
        res.value=queue->rear->data;
        return res;
    }
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

void test_size(){
    linked_list q={calloc(1,sizeof(node)),calloc(1,sizeof(node))};
    assert(q.front);
    assert(q.rear);
    node* sec_node=malloc(sizeof(node));
    assert(sec_node);

    q.front->next=sec_node;
    sec_node->prev=q.front;
    sec_node->next=q.rear;
    q.rear->prev=sec_node;

    assert(size(&q)==3);

    free(q.rear);
    q.rear=sec_node;
    q.rear->next=NULL;
    assert(size(&q)==2);

    free(q.rear);
    q.rear=q.front;
    q.rear->next=NULL;
    assert(size(&q)==1);

    free(q.rear);
    q.rear=NULL;
    q.front=NULL;
    assert(size(&q)==0);
}

void test_empty(){
    linked_list q={calloc(1,sizeof(node))};
    assert(q.front);
    q.rear=q.front;

    assert(empty(&q)==false);

    free(q.front);
    q.front=NULL;
    q.rear=NULL;
    assert(empty(&q)==true);
}

void test_front(){
    linked_list q={calloc(1,sizeof(node)),calloc(1,sizeof(node))};
    assert(q.front);
    q.front->next=q.rear;
    q.rear->prev=q.front;
    q.front->data=1;

    assert(front(&q)==1);

    free(q.front);
    q.front=q.rear;
    q.rear->prev=NULL;
    assert(front(&q)==0);

    free(q.front);
    q.front=NULL;
    q.rear=NULL;
    //a test with !q.front && !q.rear coming up
}

void test_back(){
    linked_list q={calloc(1,sizeof(node)),calloc(1,sizeof(node))};
    assert(q.front);
    q.front->next=q.rear;
    q.rear->prev=q.front;
    q.front->data=1;
    q.rear->data=2;

    assert(back(&q)==2);

    free(q.rear);
    q.rear=q.front;
    q.rear->next=NULL;
    assert(back(&q)==1);

    free(q.front);
    q.front=NULL;
    q.rear=NULL;
    //a test with !q.front && !q.rear coming up
}

int main(){

    test_push();
    test_pop();
    test_size();
    test_empty();
    test_front();
    test_back();

    return 0;
}
