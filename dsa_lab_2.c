#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

typedef enum {
    SIG_DEF,
    SIG_OS,
    SIG_TRN,
    SIG_IC
} signal;


#if defined _INC_STDIO
    void error_handling(signal name){
        if(name==SIG_OS)
            printf("The operating system did not allocate the memory\n");
        else if(name==SIG_TRN)
            printf("Passing argument is transcendent\n");
        else if(name==SIG_IC)
            printf("Function can not be called\n");
    }
#endif 

void initialize(linked_list *list,int total_nodes,int value){
    if(total_nodes<=0)
        return;

    node* new_node=calloc(1,sizeof(node));
    if(!new_node)
        return;

    new_node->data=value;
    list->head=new_node;
    
    int count=1;
    while(count<total_nodes){
        new_node->next=calloc(1,sizeof(node));
        if(!new_node->next)
            return;
        new_node=new_node->next;
        new_node->data=value;
        ++count;
    }

    list->tail=new_node;
    list->size=total_nodes;
}

void push_front(linked_list* list,int value){
    node* new_head=malloc(sizeof(node));
    if(!new_head)
        return;

    new_head->data=value;
    new_head->next=list->head;
    list->head=new_head;

    ++list->size;
}

void pop_front(linked_list* list){
    if(list->head==list->tail){
        error_handling(SIG_IC);
        return;
    }

    node* new_head=list->head->next;
    free(list->head);
    list->head=new_head;
    --list->size;

    //if head->next==NULL 
    //if list->size==1
}

void test_initialize(){
    linked_list example={NULL,NULL,0};

    initialize(&example,0,1);
    assert(example.size==0);
    assert(example.head==NULL);
    assert(example.tail==NULL);

    initialize(&example,1,1);
    assert(example.size==1);
    assert(example.head->data==1);
    assert(example.tail==example.head);
    assert(example.tail->next==NULL);

    free(example.head);
    initialize(&example,3,1);
    assert(example.size==3);
    assert(example.head->data==1);
    assert(example.head->next->data==1);
    assert(example.tail=example.head->next->next);
    assert(example.tail->data==1);
    assert(example.tail->next==NULL);

    //example must be freed
}

#ifdef _INC_STDIO

void show_list(linked_list* list){
    node* iterator=list->head;

    for(int i=0;i<list->size;++i){
        printf("%d ",iterator->data);
        iterator=iterator->next;
    }
    printf("\n");
}

#endif

int main(void)
{
    linked_list* example;
    initialize(example,1,1);

    //push_front(example,2);
    //show_list(example);
    pop_front(example);
    //show_list(example);

    return 0;
}
