#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
 
typedef struct _array 
{ 
    int *data; 
    int *size; 
    int *capacity; 
} arraylist; 
 
void push_back(arraylist* ar,int value){ 
    int cap=ar->capacity-ar->data; 
    int length=ar->size-ar->data;

    if(cap<0 || length<0 || length>cap)
        exit(1);

    if(!cap){ 
        if(!ar->data){
            ar->data=malloc(sizeof(int)); 
            if(!ar->data)
                return;
        }
        ar->capacity=ar->data+1; 
        ar->size=ar->data+1;
        ar->data[0]=value; 
    }else{ 
        if(ar->capacity==ar->size){ 
            ar->data=realloc(ar->data,cap*2*sizeof(int)); 
            if(!ar->data)
                return;
            ar->capacity=ar->data+cap*2;
            ar->size=ar->data+length; 
            *(ar->size++)=value; 
        } 
        else 
            *(ar->size++)=value; 
    } 
} 
 
void pop_back(arraylist* ar){ 
    if(ar->size<ar->data || ar->capacity<ar->data || ar->size>ar->capacity)
        exit(1);

    if(ar->size==ar->data) 
        return; 

    --ar->size; 
} 
 
void insert(arraylist* ar,int indx,int value){
    int cap=ar->capacity-ar->data;
    int length=ar->size-ar->data;

    if(cap<0 || length<0 || length>cap)
        exit(1);
    
    if(indx>=length || indx<0) 
        return; 

    if(!ar->data && !indx){
        ar->data=malloc(sizeof(int));
        if(!ar->data)
            return;
        ar->capacity=ar->data+1;
        ar->size=ar->data+1;
        ar->data[0]=value;
    }else if(!length && !indx){
        ar->data[0]=value;
        ++ar->size;
    }else if(ar->size==ar->capacity){
        ar->data=realloc(ar->data,cap*2*sizeof(int));
        if(!ar->data)
            return;
        ar->size=ar->data+length;
        ar->capacity=ar->data+cap*2;
    }

    int cr_indx=length-1; 
    while(cr_indx>=indx){ 
        ar->data[cr_indx+1]=ar->data[cr_indx]; 
        --cr_indx; 
    } 

    ++ar->size;
    ar->data[cr_indx+1]=value; 
} 

int at(arraylist* ar,int indx){
    if(ar->size<ar->data || ar->capacity<ar->data || ar->size>ar->capacity)
        exit(1);

    if(indx<0)
        return -1;
    
    size_t length=ar->size-ar->data;
    if(!length || indx>=length)
        return -2;

    return ar->data[indx];
}

void initialize(arraylist *ar,int cap){
    if(cap<0)
        return;

    ar->data=(cap ? malloc(sizeof(int)*cap) : malloc(sizeof(int)));
    if(!ar->data)
        return;

    ar->capacity=ar->data+cap;
    ar->size=ar->data;   
    int length=ar->capacity-ar->data;
}

void delete(arraylist* ar,int indx){
    int length=ar->size-ar->data;

    if(ar->size<ar->data || ar->capacity<ar->data || ar->size>ar->capacity)
        exit(1);

    if(indx<0 || !length || indx>=length)
        return;
    
    for(int i=indx;i<length-1;++i)
        ar->data[i]=ar->data[i+1];
    --ar->size;
}

void resize(arraylist *ar,int newsize){
    int length=ar->size-ar->data;
    if(newsize==length || newsize<0)
        return;

    int cap=ar->capacity-ar->data;
    
    if(newsize>cap){
        ar->data=realloc(ar->data,newsize);
        if(!ar->data)
            return;
        ar->capacity=ar->data+newsize;
        ar->size=ar->capacity;
        for(int i=length;i<newsize;++i)
            ar->data[i]=0;
    }else if(newsize>length){
        for(int i=length;i<newsize;++i)
            ar->data[i]=0;
        ar->size=ar->data+newsize;
    }else
        ar->size=ar->data+newsize;
}

void clear(arraylist* ar){
    if(ar->size<ar->data || ar->capacity<ar->data || ar->size>ar->capacity)
        exit(1);

    if(!ar->data || ar->data==ar->capacity)
        return;
    
    ar->size=ar->data;
}

int size(arraylist* ar){
    int length=ar->size-ar->data;
    if(length<0)
        exit(1);
    return length;
}

void test_push_back() { 
    arraylist a; 
    a.capacity=NULL; 
    a.size=NULL; 
    a.data=NULL; 

    push_back(&a, 1); 
    assert(a.data!=NULL); 
    assert(a.size-a.data==1); 
    assert(a.capacity-a.data==1); 
    assert(a.data[0]==1); 

    push_back(&a, 2); 
    assert(a.capacity-a.data==2); 
    assert(a.size-a.data==2); 
    assert(a.data!=NULL); 
    assert(a.data[1]==2); 

    a.size=a.data;
    push_back(&a,3);
    assert(a.capacity-a.data==2);
    assert(a.size-a.data==1);
    assert(a.data[0]==3);

    free(a.data);

    //lower operations can be avoided
    //a.capacity=NULL; 
    //a.size=NULL; 
    //a.data=NULL; 
} 
 
void test_pop_back(){ 
    int *tmp=malloc(2*sizeof(int)); 
    arraylist a={tmp,tmp+2,tmp+2}; 
    pop_back(&a); 
    assert(a.capacity==a.size+1); 
    assert(a.data==a.size-1); 

    pop_back(&a);
    assert(a.size==a.data); 
    assert(a.capacity==a.size+2);  
    
    a.size=a.data;
    pop_back(&a);
    assert(a.size==a.data);
    
    free(tmp);
} 

void test_insert(){
    int *tmp=malloc(sizeof(int));
    arraylist a={tmp,tmp,tmp};
    insert(&a,0,0);
    assert(a.capacity==a.data);
    assert(a.size==a.data);

    a.capacity=tmp+1;
    a.size=tmp+1;
    a.data[0]=-1;

    insert(&a,0,0);
    assert(a.data[0]==0);
    assert(a.capacity==a.data+2);
    assert(a.size==a.capacity);
    
    insert(&a,0,1);
    assert(a.capacity==a.data+4);
    assert(a.size==a.capacity-1);
    assert(a.data[0]==1);

    insert(&a,0,2);
    assert(a.capacity==a.size);
    assert(a.capacity-a.data==4);
    assert(a.data[0]==2);
    assert(a.data[1]==1);
    assert(a.data[2]==0);
    assert(a.data[3]==-1);

    insert(&a,4,0);
    assert(a.capacity==a.size);
    assert(a.capacity-a.data==4);

    free(tmp);
}

void test_at(){
    int *tmp=malloc(sizeof(int));
    arraylist a={tmp,tmp+1,tmp+1};
    a.data[0]=0;
    assert(at(&a,0)==0);
    assert(at(&a,1)==-2);
    assert(at(&a,-1)==-1);

    a.data=NULL;
    a.size=NULL;
    a.capacity=NULL;
    assert(at(&a,0)==-2);
    
    free(tmp);
}

void test_initialize(){
    arraylist a;
    initialize(&a,10);

    assert(a.capacity-a.data==10);
    assert(a.size==a.data);

    free(a.data);
    a.data=NULL;
    a.size=NULL;
    a.capacity=NULL;
    
    initialize(&a,-10);
    assert(a.data==NULL);
    assert(a.size==NULL);
    assert(a.capacity==NULL);
}

void test_delete()
{
    int *tmp=malloc(3*sizeof(int));
    arraylist a={tmp,tmp+3,tmp+3};
    a.data[0]=0;
    a.data[1]=1;
    a.data[2]=2;

    delete(&a,1);
    assert(a.data[1]==2);
    assert(a.capacity-a.size==1);

    delete(&a,2);
    assert(a.capacity==a.data+3);
    assert(a.capacity-a.size==1);

    a.size=a.data;
    delete(&a,0);
    assert(a.capacity==a.data+3);
    assert(a.size==a.data);

    free(tmp);
}

void test_resize(){
    int *tmp=malloc(sizeof(int));
    arraylist a={tmp,tmp+1,tmp+1};

    resize(&a,1);
    assert(a.capacity==a.data+1);
    assert(a.size==a.capacity);

    resize(&a,5);
    assert(a.capacity==a.data+5);
    assert(a.size=a.capacity);
    for(int i=1;i<5;++i)
        assert(a.data[i]==0);
    
    resize(&a,2);
    assert(a.capacity==a.data+5);
    assert(a.size==a.data+2);

    free(tmp);
}

void test_clear(){
    int *tmp=malloc(sizeof(int)*2);
    arraylist a={tmp,tmp+2,tmp+2};

    clear(&a);
    assert(a.capacity==a.data+2);
    assert(a.size==a.data);

    a.data=NULL;
    a.capacity=NULL;
    a.size=NULL;

    clear(&a);
    assert(!a.data);
    assert(!a.capacity);
    assert(!a.size);

    free(tmp);
}

void test_size(){
    int *tmp=malloc(5*sizeof(int));
    arraylist a={tmp,tmp+1,tmp+1};

    assert(size(&a)==1);
    
    a.size=tmp+5;
    assert(size(&a)==5);

    a.size=a.data;
    assert(size(&a)==0);

    free(tmp);
}

void show(arraylist* vector){
    printf("showing array (while !cap):\n");
    for(int i=0;i<vector->capacity-vector->data;++i)
        printf("%d ",vector->data[i]);

    printf("\nshowing array (while !size):\n");
    for(int i=0;i<vector->size-vector->data;++i)
        printf("%d ",vector->data[i]);
        
    printf("\n");
}

int main() 
{ 
    arraylist vector;

    test_push_back();
    test_pop_back(); 
    test_insert(); 
    test_at();
    test_initialize();
    test_delete(); 
    test_resize(); 
    test_clear();
    test_size();

    return 0; 
}