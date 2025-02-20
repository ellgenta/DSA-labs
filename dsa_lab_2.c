#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} node;

typedef struct _linked_list{
    node* head;
    size_t length;
} linked_list;

linked_list* initialize(node* head,linked_list* list){
    list->head=head;
    list->length=1;

    return list;
}

void push_front(int value,linked_list* list){
    node* newnode=malloc(sizeof(node));
    if(!newnode)
        return;

    newnode->next=list->head;
    list->head=newnode;
}

void pop_front(linked_list* list){
    node *tmp=list->head->next;
    free(list->head);

    list->head=tmp;
}

int main(void) {

}
