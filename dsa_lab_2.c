#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TESTS

typedef struct _node
{
    int data;
    struct _node *next;
} node;

typedef struct _linked_list
{
    node* head;
    node* tail;
    size_t size;
} linked_list;

typedef enum
{
    SIG_DEF,
    SIG_OS,
    SIG_TRN,
    SIG_IC
} signal;

signal received=SIG_DEF;

void error_handling(signal name){
    #if !defined TESTS
    if(name==SIG_OS)
        perror("SIG_OS: The operating system did not allocate the memory\n");
    else if(name==SIG_TRN)
        perror("SIG_TRN: Passing argument is transcendent\n");
    else if(name==SIG_IC)
        perror("SIG_IC: Function can not be called\n");
    #endif
    received=name;
}

void initialize(linked_list* list){
    list->head=NULL;
    list->tail=NULL;
    list->size=0;

    received=SIG_DEF;
}

void push_front(linked_list* list,int value){
    node* new_head=malloc(sizeof(node));
    if(!new_head){
        error_handling(SIG_OS);
        return;
    }

    new_head->data=value;
    new_head->next=list->head;
    list->head=new_head;
    if(!list->tail)
        list->tail=list->head;

    ++list->size;

    received=SIG_DEF;
}

void pop_front(linked_list* list){
    if(!list->size){
        error_handling(SIG_IC);
        return;
    }

    node* new_head=list->head->next;
    free(list->head);
    list->head=new_head;
    if(!list->head)
        list->tail=NULL;
    --list->size;

    received=SIG_DEF;
}

void push_back(linked_list* list,int value){
    node *new_tail=calloc(1,sizeof(node));
    if(!new_tail){
        error_handling(SIG_OS);
        return;
    }

    new_tail->data=value;
    if(list->head)
        list->tail->next=new_tail;
    else
        list->head=new_tail;
    list->tail=new_tail;

    ++list->size;

    received=SIG_DEF;
}

void pop_back(linked_list* list){
    if(!list->size){
        error_handling(SIG_IC);
        return;
    }

    if(list->head==list->tail){
        free(list->head);
        list->head=NULL;
        list->tail=NULL;
        --list->size;

        received=SIG_DEF;
        return;
    }

    node* tmp=list->head;
    while(tmp->next!=list->tail)
        tmp=tmp->next;

    free(list->tail);
    list->tail=tmp;
    list->tail->next=NULL;
    --list->size;

    received=SIG_DEF;
}

void insert(node* prev,int value,linked_list* list){
    if(prev->next==NULL){
        push_back(list,value);
        return;
    }

    node* new_node=malloc(sizeof(node));
    if(!new_node){
        error_handling(SIG_OS);
        return;
    }

    new_node->data=value;
    new_node->next=prev->next;
    prev->next=new_node;

    ++list->size;

    received=SIG_DEF;
}

void erase(node* prev,linked_list* list){
    if(prev->next==NULL){
        received=SIG_DEF;
        return;
    }

    node* aux_ptr=prev->next->next;
    if(!aux_ptr)
        list->tail=list->head;
    free(prev->next);
    prev->next=aux_ptr;
    --list->size;

    if(!list->size){
        list->head=NULL;
        list->tail=NULL;
    }

    received=SIG_DEF;
}

int front(linked_list* list){
    if(!list->head){
        error_handling(SIG_IC);
        #if !defined TESTS
        #error Returning value can not be used!
        #else
        return SIG_IC;
        #endif
    }
    received=SIG_DEF;

    return list->head->data;
}

int back(linked_list* list){
    if(!list->tail){
        error_handling(SIG_IC);
        #if !defined TESTS
        #error Returning value can not be used!
        #else
        return SIG_IC;
        #endif
    }
    received=SIG_DEF;

    return list->tail->data;
}

void resize(linked_list* list,int newsize){
    if(newsize<0){
        error_handling(SIG_TRN);
        return;
    }

    if(newsize==(int)list->size){
        received=SIG_DEF;
        return;
    }

    int diff=newsize-(int)list->size;
    if(diff>0){
        while(list->size<newsize)
            push_back(list,0);
    }else{
        while(list->size>newsize)
            pop_back(list);
    }

    received=SIG_DEF;
}

size_t size(linked_list* list){
    received=SIG_DEF;

    return list->size;
}

void clear(linked_list* list){
    if(!list->head)
        return;

    node* aux_ptr=list->head;
    node* aux_ptr_next=aux_ptr;

    while(aux_ptr_next){
        aux_ptr_next=aux_ptr->next;
        free(aux_ptr);
        aux_ptr=aux_ptr_next;
        --list->size;
    }

    list->head=aux_ptr;
    list->tail=aux_ptr;

    received=SIG_DEF;
}

void test_initialize(){
    linked_list example;

    initialize(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_push_front(){
    linked_list example={NULL,NULL,0};

    push_front(&example,1);
    assert(received==SIG_DEF);
    assert(example.head);
    assert(example.tail);
    assert(example.head==example.tail);
    assert(example.head->data==1);
    assert(example.size==1);

    push_front(&example,2);
    assert(received==SIG_DEF);
    assert(example.head!=example.tail);
    assert(example.head->next==example.tail);
    assert(example.head->data==2);
    assert(example.tail->data==1);
    assert(example.size==2);

    free(example.tail);
    free(example.head);
}

void test_pop_front(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),2};
    assert(example.head);
    assert(example.tail);
    example.head->next=example.tail;
    example.head->data=1;

    pop_front(&example);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.head->next==NULL);
    assert(example.head->data==0);
    assert(example.size==1);

    pop_front(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    pop_front(&example);
    assert(received==SIG_IC);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_push_back(){
    linked_list example={NULL,NULL,0};

    push_back(&example,1);
    assert(received==SIG_DEF);
    assert(example.head);
    assert(example.head==example.tail);
    assert(example.head->data==1);
    assert(example.size==1);

    push_back(&example,2);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==1);
    assert(example.tail->data==2);
    assert(example.size==2);

    free(example.tail);
    free(example.head);
}

void test_pop_back(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),2};
    assert(example.head);
    assert(example.tail);
    example.head->data=1;
    example.head->next=example.tail;

    pop_back(&example);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.tail->data==1);
    assert(example.size==1);

    pop_back(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    pop_back(&example);
    assert(received==SIG_IC);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_insert(){
    linked_list example;
    example.head=calloc(1,sizeof(node));
    assert(example.head);
    example.tail=example.head;
    example.size=1;

    insert(example.head,2,&example);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==0);
    assert(example.tail->data==2);
    assert(example.size==2);

    insert(example.head,1,&example);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->next->data==1);
    assert(example.tail->data==2);

    free(example.tail);
    free(example.head->next);
    free(example.head);
}

void test_erase(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),4};
    node* sec=malloc(sizeof(node));
    node* thrd=malloc(sizeof(node));
    assert(example.head);
    assert(example.tail);
    assert(sec);
    assert(thrd);
    thrd->next=example.tail;
    thrd->data=1;
    sec->next=thrd;
    sec->data=2;
    example.head->next=sec;
    example.head->data=3;

    erase(example.tail,&example);
    assert(received==SIG_DEF);
    assert(example.head->next==sec);
    assert(sec->next==thrd);
    assert(example.tail==thrd->next);
    assert(example.size==4);

    erase(example.head->next,&example);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->next->data==2);
    assert(example.tail->data==0);
    assert(example.tail->next==NULL);
    assert(example.size==3);

    erase(example.head,&example);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==3);
    assert(example.tail->data==0);
    assert(example.tail->next==NULL);
    assert(example.size==2);

    erase(example.head,&example);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.head->data==3);
    assert(example.tail->next==NULL);
    assert(example.size==1);
}

void test_front(){
    linked_list example={NULL,NULL,0};

    assert(front(&example)==SIG_IC);
    assert(received==SIG_IC);

    example.head=malloc(sizeof(node));
    assert(example.head);
    example.head->next=NULL;
    example.tail=example.head;
    example.head->data=1;
    example.size=1;
    assert(front(&example)==1);
    assert(received==SIG_DEF);

    example.tail=malloc(sizeof(node));
    assert(example.tail);
    example.size=2;
    example.head->next=example.tail;
    example.tail->next=NULL;
    example.tail->data=2;
    assert(front(&example)==1);
    assert(received==SIG_DEF);
}

void test_back(){
    linked_list example={NULL,NULL,0};

    assert(back(&example)==SIG_IC);
    assert(received==SIG_IC);

    example.head=malloc(sizeof(node));
    assert(example.head);
    example.head->next=NULL;
    example.tail=example.head;
    example.head->data=1;
    example.size=1;
    assert(back(&example)==1);
    assert(received==SIG_DEF);

    example.tail=malloc(sizeof(node));
    assert(example.tail);
    example.size=2;
    example.head->next=example.tail;
    example.tail->next=NULL;
    example.tail->data=2;
    assert(back(&example)==2);
    assert(received==SIG_DEF);
}

void test_resize(){
    linked_list example={NULL,NULL,0};

    resize(&example,-1);
    assert(received==SIG_TRN);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    resize(&example,2);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==0);
    assert(example.tail->data==0);
    assert(example.size==2);

    resize(&example,3);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->data==0);
    assert(example.head->next->data==0);
    assert(example.tail->data==0);
    assert(example.size==3);

    resize(&example,1);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.size==1);
    assert(example.head->data==0);
    assert(example.tail->next==NULL);

    resize(&example,0);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_size(){
    linked_list example={calloc(1,sizeof(node)),calloc(1,sizeof(node)),2};
    assert(example.head);
    assert(example.tail);

    example.head->next=example.tail;
    assert(size(&example)==2);
    assert(received==SIG_DEF);

    free(example.tail);
    free(example.head);
    example.size=0;

    assert(size(&example)==0);
    assert(received==SIG_DEF);
}

void test_clear(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),3};
    assert(example.head);
    assert(example.tail);

    node* sec=malloc(sizeof(node));
    assert(sec);
    example.head->next=sec;
    sec->next=example.tail;

    clear(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    clear(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

int main(void)
{
    linked_list* ex;

    test_initialize();
    test_pop_front();
    test_pop_back();
    test_push_front();
    test_push_back();
    test_insert();
    test_erase();
    test_front();
    test_back();
    test_resize();
    test_size();
    test_clear();

    return 0;
}
