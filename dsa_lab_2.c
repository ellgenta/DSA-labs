//#include <stdio.h>
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
        printf("SIG_OS: The operating system did not allocate the memory\n");
    else if(name==SIG_TRN)
        printf("SIG_TRN: Passing argument is transcendent\n");
    else if(name==SIG_IC)
        printf("SIG_IC: Function can not be called\n");
    #endif
    received=name;
    //how to "message" about a received signal if stdio.h isn't included?
}

void initialize(linked_list* list,int total_nodes,int value){
    if(total_nodes<=0){
        error_handling(SIG_TRN);
        return;
    }

    node* new_node=calloc(1,sizeof(node));
    if(!new_node){
        error_handling(SIG_OS);
        return;
    }

    new_node->data=value;
    list->head=new_node;
    
    int count=1;
    while(count<total_nodes){
        new_node->next=calloc(1,sizeof(node));
        if(!new_node->next){
            error_handling(SIG_OS);
            return;
        }
        new_node=new_node->next;
        new_node->data=value;
        ++count;
    }

    list->tail=new_node;
    list->size=total_nodes;

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

void insert(linked_list* list,int indx,int value){
    if(indx>=(int)list->size){
        error_handling(SIG_TRN);
        return;
    }else if(indx<0){
        error_handling(SIG_IC);
        return;
    }

    if(!indx){
        push_front(list,value);
        return;
    }else if(indx==list->size-1){
        push_back(list,value);
        return;
    }

    node* ins_node=malloc(sizeof(node));
    if(!ins_node){
        error_handling(SIG_OS);
        return;
    }
    node* temp=list->head;

    int count=0;
    while(count++<indx-1)
        ins_node=ins_node->next;

    ins_node->next=temp->next;
    temp->next=ins_node;
    ins_node->data=value;
    ++list->size;

    received=SIG_DEF;
}

void erase(linked_list* list,int indx){
    if(indx>=(int)list->size){
        error_handling(SIG_TRN);
        return;
    }else if(indx<0){
        error_handling(SIG_IC);
        return;
    }

    if(!indx){
        pop_front(list);
        return;
    }else if(indx==list->size-1){
        pop_back(list);
        return;
    }

    node* er_node=malloc(sizeof(node));
    if(!er_node){
        error_handling(SIG_OS); 
        return;
    }
    er_node=list->head->next;
    node* prev=list->head;

    int count=0;
    while(count++<indx-1){
        prev=er_node;
        er_node=er_node->next;
    }

    prev->next=er_node->next;
    free(er_node);
    --list->size;

    received=SIG_DEF;
}

int front(linked_list* list){
    if(!list->head){
        error_handling(SIG_IC);
        #if !defined TESTS
        #error Returning value can not be used!
        exit(3);
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
        exit(3);
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

    int diff=newsize-(int)list->size;

    if(!diff)
        return;

    if(diff>0){
        node* aux_node=calloc(1,sizeof(node));
        if(!aux_node){
            error_handling(SIG_OS);
            return;
        }

        if(!list->tail){
            list->tail=aux_node;
            list->head=list->tail;
            ++list->size;
            --diff;
        }else
            aux_node=list->tail;

        int count=0;
        while(count<diff){
            aux_node->next=calloc(1,sizeof(node));
            list->tail=aux_node->next;
            if(!aux_node){
                error_handling(SIG_OS);
                return;
            }
            aux_node=aux_node->next;
            ++list->size;
            ++count;
        }
    }else{
        diff=0-diff;
        node* links[diff];

        node* aux_ptr=list->head;

        for(int count=0,i=0;count<list->size;++count){
            if(count==newsize-1)
                list->tail=aux_ptr;
            if(count>=newsize)
                links[i++]=aux_ptr;
            aux_ptr=aux_ptr->next;
        }

        while(diff){
            free(links[--diff]);
            --list->size;
        }

        if(newsize)
            list->tail->next=NULL;
        else{
            list->head=NULL;
            list->tail=NULL;
        }
    }

    received=SIG_DEF;
}

void test_initialize(){
    linked_list example={NULL,NULL,0};

    initialize(&example,0,1);
    assert(received==SIG_TRN);
    assert(example.size==0);
    assert(example.head==NULL);
    assert(example.tail==NULL);

    initialize(&example,1,1);
    assert(received==SIG_DEF);
    assert(example.size==1);
    assert(example.head->data==1);
    assert(example.tail==example.head);
    assert(example.tail->next==NULL);

    free(example.head);
    initialize(&example,3,1);
    assert(received==SIG_DEF);
    assert(example.size==3);
    assert(example.head->data==1);
    assert(example.head->next->data==1);
    assert(example.tail=example.head->next->next);
    assert(example.tail->data==1);
    assert(example.tail->next==NULL);

    free(example.tail);
    free(example.head->next);
    free(example.head);
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
  
    insert(&example,1,1);
    assert(received==SIG_TRN);
    assert(example.head==example.tail);
    assert(example.head->data==0);

    insert(&example,0,1);
    assert(received==SIG_DEF);
    assert(example.tail->data==0);
    assert(example.head->next==example.tail);
    assert(example.size==2);

    insert(&example,1,-2);
    assert(received==SIG_DEF);
    assert(example.tail->data==-2);
    assert(example.head->next->data==0);
    assert(example.head->next->next==example.tail);
    assert(example.size==3);

    insert(&example,1,-1);
    assert(received==SIG_DEF);
    assert(example.head->data==1);
    assert(example.head->next->data==-1);
    assert(example.head->next->next->data==0);
    assert(example.tail->data==-2);
    assert(example.tail->next==NULL);
    assert(example.size==4);

    free(example.tail);
    free(example.head->next->next);
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

    erase(&example,4);
    assert(received==SIG_TRN);
    assert(example.head->next==sec);
    assert(sec->next==thrd);
    assert(example.tail==thrd->next);
    assert(example.size==4);

    erase(&example,-1);
    assert(received==SIG_IC);
    assert(example.head->next==sec);
    assert(sec->next==thrd);
    assert(example.tail==thrd->next);
    assert(example.size==4);

    erase(&example,2);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->next->data==2);
    assert(example.tail->data==0);
    assert(example.tail->next==NULL);
    assert(example.size==3);

    erase(&example,1);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==3);
    assert(example.tail->data==0);
    assert(example.tail->next==NULL);
    assert(example.size==2);

    erase(&example,1);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.head->data==3);
    assert(example.tail->next==NULL);
    assert(example.size==1);

    erase(&example,0);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_front(){
    linked_list example={NULL,NULL,0};

    assert(front(&example)==SIG_IC);
    assert(received==SIG_IC);

    example.head=malloc(sizeof(int));
    assert(example.head);
    example.head->next=NULL;
    example.tail=example.head;
    example.head->data=1;
    example.size=1;
    assert(front(&example)==1);
    assert(received==SIG_DEF);

    example.tail=malloc(sizeof(int));
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

    example.head=malloc(sizeof(int));
    assert(example.head);
    example.head->next=NULL;
    example.tail=example.head;
    example.head->data=1;
    example.size=1;
    assert(back(&example)==1);
    assert(received==SIG_DEF);

    example.tail=malloc(sizeof(int));
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
    assert(example.head->data==0);
    assert(example.tail->next==NULL);

    resize(&example,0);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
    //test newsize=size?
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
    linked_list* ex;
 
    //maybe iterator should be transformed into a separate function
    //init function should be rewritten

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

    return 0;
}
