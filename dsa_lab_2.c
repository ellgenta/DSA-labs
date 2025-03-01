#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _node{
    int data;
    struct _node *next;
} node;

typedef struct _linked_list{
    node* head;
    node* tail;
    size_t size;
} linked_list;

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

void show_list(linked_list* list){
    node* iterator=list->head;

    for(int i=0;i<list->size;++i){
        printf("%d ",iterator->data);
        iterator=iterator->next;
    }
}

int main(void)
{
    linked_list* example;
    initialize(example,1,1);

    test_initialize();
    show_list(example);
    return 0;
}
