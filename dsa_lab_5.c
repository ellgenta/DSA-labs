#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 100

typedef enum {false,true} bool;
typedef enum {otr,ond} role;

char precedence[]="+-\t*/"; 

typedef struct _exp_t{
    int res;
    char* rpn_exp;
} exp_t;

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

int abs(int x){
    return x<0 ? -x : x;
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

node* tail(node** head){
    node* aux=*head;
    while(aux->next)
        aux=aux->next;
    
    return aux;
}

void copy(node** head,char* str){
    node* aux=*head;
    char* ptr_str=str;

    while(aux && ptr_str){
        if(aux->r==ond)
            sprintf(ptr_str,"%d ",aux->data);
        else
            sprintf(ptr_str,"%c ",aux->data);

        ptr_str=strchr(ptr_str,'\0');
        aux=aux->next;
    }
}

int perform(node** top,node** head){
    int res=0;

    if((*head)->data=='+')
        res=(*top)->prev->data+(*top)->data;
    else if((*head)->data=='-')
        res=(*top)->prev->data-(*top)->data;
    else if((*head)->data=='*')
        res=(*top)->prev->data*(*top)->data;
    else if((*head)->data=='/')
        res=(*top)->prev->data/(*top)->data;

    pop(top);
    pop(top);

    push(top,res);
}

bool valid(node** head){
    node* aux=*head;

    size_t total_otr=0;
    size_t total_ond=0;

    while(aux){
        if(aux->r==otr)
            ++total_otr;
        else
            ++total_ond;
        
        aux=aux->next;
    }

    return total_ond==total_otr+1 ? true : false;
}

bool find(node** top,char key){
    node* aux=*top;

    while(aux){
        if(aux->data==key)
            return true;
        aux=aux->prev;
    }

    return false;
}

bool close_parantheses(node** top,node** head){
    if(!*top)
        return false;

    while(*top){
        if((*top)->data==')')
            return false;
        if((*top)->data!='('){
            enqueue(head,(*top)->data);
            tail(head)->r=otr;
            pop(top);
        }
        else{
            pop(top);
            return true;
        }
    }
    
    return false;
}

int calculate(node** top,node** head){
    int res=0;

    while(!empty(head)){
        if((*head)->r==ond)
            push(top,(*head)->data);
        else
            perform(top,head);
        
        dequeue(head);
    }

    res=(*top)->data;
    pop(top);

    return res;
}

exp_t shunting_yard(char* input){
    node* head=NULL;
    node* top=NULL;

    char* ptr_input=input;

    while(*ptr_input){
        if(*ptr_input==' ' || *ptr_input=='\t'){
            ++ptr_input;
            continue;
        }
        if(operator(ptr_input)){
            while(top && compatible(&top,*ptr_input)){
                enqueue(&head,top->data);
                pop(&top);
            }
            push(&top,*ptr_input);
            top->r=otr;
        }
        else if(operand(ptr_input)){
            enqueue(&head,atoi(ptr_input));
            tail(&head)->r=ond;
            while(operand(ptr_input+1))
                ++ptr_input;
        }
        else if(*ptr_input=='(')
            push(&top,*ptr_input);
        else if(*ptr_input==')'){
            if(close_parantheses(&top,&head)==false)
                return (exp_t){0,""};  
        }
        else 
            return (exp_t){0,""};           

        ++ptr_input;
    }

    if(find(&top,'(') || find(&top,')'))
        return (exp_t){0,""};
        
    while(!empty(&top)){
        enqueue(&head,top->data);
        pop(&top);
        tail(&head)->r=otr;
    }

    if(!valid(&head))
        return (exp_t){0,""};

    char postfix[MAX_LEN]={0};
    copy(&head,postfix);
    int res=calculate(&top,&head);    
    
    return (exp_t){res,postfix};
}

void test_shunting_yard(){
    assert(shunting_yard("1+2*3").res==7);
    assert(!strcmp(shunting_yard("1+2*3").rpn_exp,"1 2 3 * + "));

    assert(shunting_yard("1+2+3").res==6);
    assert(!strcmp(shunting_yard("1+2+3").rpn_exp,"1 2 + 3 + "));

    assert(shunting_yard("1+2-3").res==0);
    assert(!strcmp(shunting_yard("1+2-3").rpn_exp,"1 2 + 3 - "));

    assert(shunting_yard("1+2+a").res==0);
    assert(!strcmp(shunting_yard("1+2+a").rpn_exp,""));

    assert(shunting_yard("(1 + 2 )* 3").res==9);
    assert(!strcmp(shunting_yard("(1+2)*3").rpn_exp,"1 2 + 3 * "));

    assert(shunting_yard("(6+8)/2+3*9").res==34);
    assert(!strcmp(shunting_yard("(6+8)/2+3*9").rpn_exp,"6 8 + 2 / 3 9 * + "));

    assert(shunting_yard("2+2-2*2").res==0);
    assert(!strcmp(shunting_yard("2+2-2*2").rpn_exp,"2 2 + 2 2 * - "));

    assert(shunting_yard("(2").res==0);
    assert(!strcmp(shunting_yard("(2").rpn_exp,""));

    assert(shunting_yard("7/)").res==0);
    assert(!strcmp(shunting_yard("7/)").rpn_exp,""));

    assert(shunting_yard("2").res==2);
    assert(!strcmp(shunting_yard("2").rpn_exp,"2 "));

    assert(shunting_yard("3+").res==0);
    assert(!strcmp(shunting_yard("3+").rpn_exp,""));

    assert(shunting_yard("+ +").res==0);
    assert(!strcmp(shunting_yard("+ +").rpn_exp,""));

    assert(shunting_yard("").res==0);
    assert(!strcmp(shunting_yard("").rpn_exp,""));

    assert(shunting_yard("3+\t4").res==7);
    assert(!strcmp(shunting_yard("3+\t4").rpn_exp,"3 4 + "));

    assert(shunting_yard("\t").res==0);
    assert(!strcmp(shunting_yard("\t'").rpn_exp,""));

    assert(shunting_yard("2*(1+2*(3+6))").res==38);
    assert(!strcmp(shunting_yard("2*(1+2*(3+6))").rpn_exp,"2 1 2 3 6 + * + * "));

    assert(shunting_yard("(1+2))").res==0);
    assert(!strcmp(shunting_yard("(1+2))").rpn_exp,""));

    assert(shunting_yard("(1+2)(").res==0);
    assert(!strcmp(shunting_yard("(1+2)(").rpn_exp,""));

    assert(shunting_yard("(1)+2+3").res==6);
    assert(!strcmp(shunting_yard("1+2+3").rpn_exp,"1 2 + 3 + "));

    assert(shunting_yard("100-200").res==-100);
    assert(!strcmp(shunting_yard("100-200").rpn_exp,"100 200 -"));

    assert(shunting_yard("100*200-300").res==19700);
    assert(!strcmp(shunting_yard("100-200").rpn_exp,"100 200 * 300 -"));
}

int main(void)
{
    test_shunting_yard();

    return 0;
}
