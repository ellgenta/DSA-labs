#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TESTS_MACRO
#define MAX_BUF_SIZE    100

#ifdef TESTS_MACRO

static char test_buf[MAX_BUF_SIZE]={0};
static char* test_buf_ptr=test_buf;

void buf_flush(char* buf_ptr,size_t buf_size){
    for(int i=0;i<buf_size && buf_ptr[i];++i)
        buf_ptr[i]='\0';
}

#endif

typedef struct _node{
    int x;
    struct _node* left;
    struct _node* right;
} node;

typedef enum{
    false,
    true
} bool;

typedef struct _elm{
    node* ptr;
    struct _elm* prev;
    struct _elm* next;
} element;

typedef struct _linked_list{
    element* front;
    element* rear;
} linked_list;

void push(linked_list* queue,node* node_ptr){
    element* new_rear=calloc(1,sizeof(element));
    if(!new_rear)
        return;

    new_rear->ptr=node_ptr;
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

    element* aux_ptr=queue->front->next;
    free(queue->front);
    queue->front=aux_ptr;
    queue->front->prev=NULL;

}

bool empty(linked_list* queue){
    if(!queue->front && !queue->rear)
        return true;
    else
        return false;
}

void clear(node** root){
    if(*root==NULL)
        return;

    if((*root)->left)
        clear(&(*root)->left);
    if((*root)->right)
        clear(&(*root)->right);
    if(!(*root)->right && !(*root)->right){
        node* aux_root=*root;
        *root=NULL;
        free(aux_root);
    }
    
    return;
}

node* find(node* root,int key){
    if(root==NULL)
        return NULL;

    if(root->x>key)
        return find(root->left,key);
    else if(root->x<key)
        return find(root->right,key);

    return root;
}

int height(node* root){
    if(root==NULL)
        return 0;

    int l_height=1;
    int r_height=1;

    l_height+=height(root->left);
    r_height+=height(root->right); 

    return l_height>r_height ? l_height : r_height;
}

void insert(node** root,node* subtree_root){
    if(!*root){
        *root=subtree_root;
        return;
    }

    if(find(*root,subtree_root->x))
        return;

    node* aux=*root;

    while(aux->left!=subtree_root && aux->right!=subtree_root){
        if(subtree_root->x<aux->x && aux->left)
            aux=aux->left;
        else if(subtree_root->x<aux->x)
            aux->left=subtree_root;
        else if(subtree_root->x>aux->x && aux->right)
            aux=aux->right;
        else
            aux->right=subtree_root;
    }
}

void print(node* root){
    if(!root)
        return;
    
    linked_list* q=calloc(1,sizeof(linked_list));
    if(!q)  
        return;

    push(q,root);
    
    while(!empty(q)){
        root=q->front->ptr;

        #ifndef TESTS_MACRO 
            printf("%d ",q->front->ptr->x);
        #else
            sprintf(test_buf_ptr,"%d ",q->front->ptr->x);
            test_buf_ptr=strchr(test_buf,'\0');
        #endif

        pop(q);

        if(root->left)
            push(q,root->left);
        if(root->right)
            push(q,root->right);
    }

    #ifndef TESTS_MACRO
    printf("\n");
    #endif

    free(q);
}

void test_clear(){
    node* root=calloc(1,sizeof(node));
    assert(root);

    root->left=calloc(1,sizeof(node));
    assert(root->left);
    root->right=calloc(1,sizeof(node));
    assert(root->right);

    root->left->left=calloc(1,sizeof(node));
    assert(root->left->left);
    root->left->right=NULL;
    root->right->right=NULL;
    root->right->left=calloc(1,sizeof(node));
    assert(root->right->left);

    clear(&root);
    assert(root==NULL);

    clear(&root);
    assert(root==NULL);
}

void test_find(){
    node* root=malloc(sizeof(node));
    assert(root);
    root->x=15;

    root->left=malloc(sizeof(node));
    assert(root->left);
    root->right=malloc(sizeof(node));
    assert(root->right);

    root->left->x=12;
    root->right->x=17;

    root->left->left=calloc(1,sizeof(node));
    assert(root->left->left);
    root->left->right=calloc(1,sizeof(node));
    assert(root->left->right);
    root->right->left=calloc(1,sizeof(node));
    assert(root->right->left);
    root->right->right=calloc(1,sizeof(node));
    assert(root->right->right);

    root->left->left->x=8;
    root->left->right->x=13;
    root->right->left->x=16;
    root->right->right->x=28;

    assert(find(root,15)==root);
    assert(find(root,12)==root->left);
    assert(find(root,17)==root->right);
    assert(find(root,8)==root->left->left);
    assert(find(root,13)==root->left->right);
    assert(find(root,16)==root->right->left);
    assert(find(root,28)==root->right->right);
    assert(find(root,9)==NULL);

    clear(&root->right->right);
    assert(find(root,28)==NULL);
    assert(find(root,16)==root->right->left);

    clear(&root->left);
    assert(find(root,8)==NULL);
    assert(find(root,13)==NULL);
    assert(find(root,12)==NULL);
    assert(find(root,17)==root->right);
    assert(find(root,16)==root->right->left);

    clear(&root);
}

void test_height(){
    node* root=calloc(1,sizeof(node));
    assert(root);
    
    root->left=calloc(1,sizeof(node));
    assert(root->left);
    root->right=calloc(1,sizeof(node));
    assert(root->right);

    root->left->left=calloc(1,sizeof(node));
    assert(root->left->left);
    root->left->right=NULL;
    root->right->left=NULL;
    root->right->right=calloc(1,sizeof(node));
    assert(root->right->right);

    assert(height(root)==3);

    root->left->left->right=calloc(1,sizeof(node));
    assert(root->left->left->right);
    assert(height(root)==4);

    root->right->right->left=calloc(1,sizeof(node));
    assert(root->right->right->left);
    root->right->right->left->right=calloc(1,sizeof(node));
    assert(root->right->right->left->right);
    assert(height(root)==5);

    clear(&root);
    assert(height(root)==0);
}

void test_insert(){
    node* root=calloc(1,sizeof(node));
    assert(root);
    root->x=5;

    node* first_case=calloc(1,sizeof(node));
    assert(first_case);
    first_case->x=10; 

    insert(&root,first_case);
    assert(root->right==first_case);

    node* second_case=calloc(1,sizeof(node));
    assert(second_case);
    second_case->left=calloc(1,sizeof(node));
    assert(second_case->left);
    second_case->right=calloc(1,sizeof(node));
    assert(second_case->right);

    second_case->x=4;
    second_case->left->x=2;
    second_case->right->x=3;

    insert(&root,second_case);
    assert(root->left==second_case);
    assert(root->left->left==second_case->left);
    assert(root->left->right==second_case->right);

    node* third_case=calloc(1,sizeof(node));
    assert(third_case);
    third_case->x=6;

    insert(&root,third_case);
    assert(root->right->left==third_case);

    clear(&root);

    node* fourth_case=calloc(1,sizeof(node));
    assert(fourth_case);
    
    insert(&root,fourth_case);
    assert(root==fourth_case);

    clear(&root);
}   

void test_print(){
    node* root=malloc(sizeof(node));
    assert(root);
    root->x=15;
    
    root->left=malloc(sizeof(node));
    assert(root->left);
    root->right=malloc(sizeof(node));
    assert(root->right);

    root->left->x=12;
    root->right->x=17;

    root->left->left=calloc(1,sizeof(node));
    assert(root->left->left);
    root->left->right=calloc(1,sizeof(node));
    assert(root->left->right);
    root->right->left=calloc(1,sizeof(node));
    assert(root->right->left);
    root->right->right=calloc(1,sizeof(node));
    assert(root->right->right);

    root->left->left->x=8;
    root->left->right->x=13;
    root->right->left->x=16;
    root->right->right->x=28;

    #ifdef  TESTS_MACRO
    char expected_first[]="15 12 17 8 13 16 28";
    print(root);
    assert(strstr(test_buf,expected_first));
    buf_flush(test_buf,MAX_BUF_SIZE);
    test_buf_ptr=test_buf;

    clear(&root->right->right);
    char expected_second[]="15 12 17 8 13 16";
    print(root);
    assert(strstr(test_buf,expected_second));
    buf_flush(test_buf,MAX_BUF_SIZE);
    test_buf_ptr=test_buf;

    clear(&root->left);
    char expected_third[]="15 17 16";
    print(root);
    assert(strstr(test_buf,expected_third));
    buf_flush(test_buf,MAX_BUF_SIZE);
    test_buf_ptr=test_buf;
    #endif

    clear(&root);
}

int main(void){

    test_find();
    test_clear();
    test_height();
    test_insert();
    test_print();

    return 0;
}
