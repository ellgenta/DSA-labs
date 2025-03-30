#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct _node{
    int x;
    struct _node* left;
    struct _node* right;
} node;

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

void insert(node* root,node* subtree_root){
    if(find(root,subtree_root->x))
        return;
    //subtree-root value already makes part of the initial tree values
    
    node* aux=root;
    while(aux){
        if(subtree_root->x<aux->x){
            if(aux->left)
                aux=aux->left;
            else{
                aux->left=subtree_root;
                break;
            }
        } else{
            if(aux->right)
                aux=aux->right;
            else{
                aux->right=subtree_root;
                break;
            }
        }
    }

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

    node* ft_entry=calloc(1,sizeof(node));
    assert(ft_entry);
    ft_entry->x=10; 

    insert(root,ft_entry);
    assert(root->right==ft_entry);

    node* sd_entry_root=calloc(1,sizeof(node));
    assert(sd_entry_root);
    node* sd_entry_left=calloc(1,sizeof(node));
    assert(sd_entry_left);
    node* sd_entry_right=calloc(1,sizeof(node));
    assert(sd_entry_right);

    sd_entry_root->left=sd_entry_left;
    sd_entry_root->right=sd_entry_right;

    sd_entry_root->x=4;
    sd_entry_left->x=2;
    sd_entry_right->x=3;

    insert(root,sd_entry_root);
    assert(root->left==sd_entry_root);
    assert(root->left->left==sd_entry_left);
    assert(root->left->right==sd_entry_right);

    node* td_entry=calloc(1,sizeof(node));
    assert(td_entry);
    td_entry->x=6;

    insert(root,td_entry);
    assert(root->right->left==td_entry);

    clear(&root);
}   

int main(void){

    test_find();
    test_clear();
    test_height();
    test_insert();

    return 0;
}
