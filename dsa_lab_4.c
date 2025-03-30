#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

//#define TESTS

typedef struct _node{
    int x;
    struct _node* left;
    struct _node* right;
} node;

//an error-handling systel will be implemented here if such a need occurs

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

void test_clear(){
    node* root=calloc(1,sizeof(node));
    
    root->left=calloc(1,sizeof(node));
    root->right=calloc(1,sizeof(node));

    root->left->left=calloc(1,sizeof(node));
    root->left->right=NULL;
    root->right->left=NULL;
    root->right->right=calloc(1,sizeof(node));

    clear(&root);
    assert(root==NULL);

    clear(&root);
    assert(root==NULL);
}

void test_find(){
    node* root=malloc(sizeof(node));
    root->x=15;
    
    root->left=malloc(sizeof(node));
    root->right=malloc(sizeof(node));

    root->left->x=12;
    root->right->x=17;

    root->left->left=calloc(1,sizeof(node));
    root->left->right=calloc(1,sizeof(node));
    root->right->left=calloc(1,sizeof(node));
    root->right->right=calloc(1,sizeof(node));

    root->left->left->x=8;
    root->left->right->x=13;
    root->right->left->x=16;
    root->right->right->x=28;

    assert(find(root,15)->x==15);
    assert(find(root,12)->x==12);
    assert(find(root,17)->x==17);
    assert(find(root,8)->x==8);
    assert(find(root,13)->x==13);
    assert(find(root,16)->x==16);
    assert(find(root,28)->x==28);
    assert(find(root,9)==NULL);

    free(root->right->right);
    root->right->right=NULL;

    assert(find(root,28)==NULL);
    assert(find(root,16)->x==16);

    free(root->left->left);
    free(root->left->right);
    free(root->left);
    root->left=NULL;

    assert(find(root,8)==NULL);
    assert(find(root,13)==NULL);
    assert(find(root,12)==NULL);
    assert(find(root,17)->x==17);
    assert(find(root,16)->x==16);

    clear(&root);
}

void test_height(){
    node* root=calloc(1,sizeof(node));
    
    root->left=calloc(1,sizeof(node));
    root->right=calloc(1,sizeof(node));

    root->left->left=calloc(1,sizeof(node));
    root->left->right=NULL;
    root->right->left=NULL;
    root->right->right=calloc(1,sizeof(node));

    assert(height(root)==3);

    root->left->left->right=calloc(1,sizeof(node));
    assert(height(root)==4);

    root->right->right->left=calloc(1,sizeof(node));
    root->right->right->left->right=calloc(1,sizeof(node));
    assert(height(root)==5);

    clear(&root);
    assert(height(root)==0);
}

int main(void){

    test_find();
    test_clear();
    test_height();

    return 0;
}
