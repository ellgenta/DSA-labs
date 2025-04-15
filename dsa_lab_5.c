#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef enum {false,true} bool;
typedef enum {otr,ond} role;

char precedence[]="+-\t*/\t()"; 

/*
considering this to be a returning-type for the shunting_yard(), where res stands for the final result (of calculations), 
and rpn_exp stands for a string, that will store corresponding expression in reverse Polish notation (so the output queue will be transformed into a string)

typedef struct _exp_t{
    int res;
    char* rpn_exp;
} exp_t;

*/

typedef struct _node{
    int data;
    role r;
    struct _node* next;
    struct _node* prev;
} node;

void push(node** top,int value){
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

void enqueue(node** head,int value){
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

bool operator(char* sym){
    strchr(precedence,*sym) ? true : false;
}

bool operand(char* sym){
    if(*sym>='0' && *sym<='9')
        return true;
    else 
        return false;
}

bool compatible(node** top,char sym){
    if(!*top)
        return true;

    if(strchr(precedence,sym)-strchr(precedence,(*top)->data)>1)
        return false;
    else 
        return true;
}

void shunting_yard(char* input){
    node* head=NULL;
    node* top=NULL;

    char* ptr_input=input;
    short ops_count;

    while(*ptr_input){
        if(operator(ptr_input)){
            while(top && compatible(&top,*ptr_input)){
                enqueue(&head,top->data);
                pop(&top);
            }
            push(&top,*ptr_input);
            top->r=otr;
            //continue;
        }
        else if(operand(ptr_input)){
            enqueue(&head,atoi(ptr_input));
            head->r=ond;
        }

        ++ptr_input;
    }

    while(!empty(&top)){
        enqueue(&head,top->data);
        pop(&top);
        head->r=otr;
    }
}

int main(void)
{
    return 0;
}
