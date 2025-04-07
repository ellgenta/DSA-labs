#include <stdlib.h>
#include <assert.h>

typedef enum {false,true} bool;

typedef struct _node{
    char data;
    struct _node* next;
    struct _node* prev;
} node;

void push(node** top,char value){
    node* new_top=calloc(1,sizeof(node));
    if(!new_top)
        return;
    new_top->data=value;

    if(*top){
        new_top->prev=*top;
        (*top)->next=new_top;
        *top=new_top;
    }else
        *top=new_top;
}

void pop(node** top){
    if(!*top)
        return;
    
    node* aux=*top;
    *top=(*top)->prev;
    if(*top)
        (*top)->next=NULL;
    
    free(aux);
}

void enqueue(node** head,char value){
    node* new_rear=calloc(1,sizeof(node));
    if(!new_rear)
        return;
    new_rear->data=value;

    if(!*head){
        *head=new_rear;
        return;
    }

    node* aux=*head;
    while(aux->next)
        aux=aux->next;

    aux->next=new_rear;
    new_rear->prev=aux;
}

void dequeue(node** head){
    if(!*head)
        return;

    node* aux=*head;
    *head=(*head)->next;
    if(*head)
        (*head)->prev=NULL;

    free(aux);
}

bool empty(node** head){
    return *head ? false : true;
}

int main(void)
{
    return 0;
}
