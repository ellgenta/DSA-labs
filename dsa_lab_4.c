#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct _node{
    int x;
    struct _node* left;
    struct _node* right;
} node;

//an error-handling system will be implemented a bit later

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
    //root==NULL case should be catched

    if(root->x>key)
        return find(root->left,key);
    else if(root->x<key)
        return find(root->right,key);

    return root;
}

void test_clear(){
    node* root=malloc(sizeof(node));
    root->x=10;
    
    root->left=malloc(sizeof(node));
    root->right=malloc(sizeof(node));

    root->left->x=5;
    root->right->x=13;

    root->left->left=calloc(1,sizeof(node));
    root->left->right=calloc(1,sizeof(node));
    root->right->left=calloc(1,sizeof(node));
    root->right->right=calloc(1,sizeof(node));

    root->left->left->x=4;
    root->left->right->x=6;
    root->right->left->x=11;
    root->right->right->x=15;

    clear(&root);
    assert(root==NULL);

    clear(&root);
    assert(root==NULL);
}

void test_find(){
    node* root=malloc(sizeof(node));
    root->x=10;
    
    root->left=malloc(sizeof(node));
    root->right=malloc(sizeof(node));

    root->left->x=5;
    root->right->x=13;

    root->left->left=calloc(1,sizeof(node));
    root->left->right=calloc(1,sizeof(node));
    root->right->left=calloc(1,sizeof(node));
    root->right->right=calloc(1,sizeof(node));

    root->left->left->x=4;
    root->left->right->x=6;
    root->right->left->x=11;
    root->right->right->x=15;

    assert(find(root,15)->x==15);
    assert(find(root,6)->x==6);
    assert(find(root,4)->x==4);
    assert(find(root,5)->x==5);
    assert(find(root,10)->x==10);
    assert(find(root,8)==NULL);

    clear(&root);
}

int main(void){

    test_find();
    test_clear();

    return 0;
}
