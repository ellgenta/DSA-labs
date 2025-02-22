#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _array
{
    int *data;
    int *size;
    int *capacity;
} arraylist;

typedef struct _error_handling{
    int error_code;
    int returned_value;
} error_handling;


int size(arraylist* ar){
    int length=ar->size-ar->data;
    if(length<0)
        exit(1);

    return length;
}

void push_back(arraylist* ar,int value){
    int cap=ar->capacity-ar->data;
    int length=size(ar);

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
    int cap=ar->capacity-ar->data;
    int length=size(ar);

    if(cap<0 || length<0 || length>cap)
        exit(1);

    if(ar->size==ar->data)
        return;

    --ar->size;
}

void insert(arraylist* ar,int indx,int value){
    int cap=ar->capacity-ar->data;
    int length=size(ar);

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

error_handling at(arraylist* ar,int indx){
    error_handling info={0,0};

    int cap=ar->capacity-ar->data;
    int length=size(ar);

    if(cap<0 || length<0 || length>cap)
        exit(1);

    if(indx<0){
        info.error_code=-1;
        return info;
    }

    if(!length || indx>=length){
        info.error_code=-2;
        return info;
    }

    info.returned_value=ar->data[indx];
    return info;
}

void initialize(arraylist *ar){
    ar->data=malloc(sizeof(int));

    if(!ar->data)
        return;

    ar->size=ar->data;
    ar->capacity=ar->data+1;
}

void delete(arraylist* ar,int indx){
    int cap=ar->capacity-ar->data;
    int length=size(ar);

    if(cap<0 || length<0 || length>cap)
        exit(1);

    if(indx<0 || !length || indx>=length)
        return;

    for(int i=indx;i<length-1;++i)
        ar->data[i]=ar->data[i+1];
    --ar->size;
}

void resize(arraylist *ar,int newsize){
    int cap=ar->capacity-ar->data;
    int length=size(ar);

    if(cap<0 || length<0 || length>cap)
        exit(1);

    if(newsize==length || newsize<0)
        return;

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
    ar->data=realloc(ar->data,sizeof(int));

    ar->capacity=ar->data ? ar->data+1 : NULL;
    ar->size=ar->data;
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
}

void test_pop_back(){
    int *tmp=malloc(2*sizeof(int));
    assert(tmp);

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
    assert(tmp);

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

    if(a.data!=tmp)
        free(a.data);
    free(tmp);
}

void test_at(){
    int *tmp=malloc(sizeof(int));
    assert(tmp);

    arraylist a={tmp,tmp+1,tmp+1};
    a.data[0]=0;
    assert(at(&a,0).returned_value==0);
    assert(at(&a,1).error_code==-2);
    assert(at(&a,-1).error_code==-1);

    a.data=NULL;
    a.size=NULL;
    a.capacity=NULL;
    assert(at(&a,0).error_code==-2);

    free(tmp);
}

void test_initialize(){
    arraylist a;
    initialize(&a);

    assert(a.capacity==a.data+1);
    assert(a.size==a.data);

    free(a.data);
}

void test_delete()
{
    int *tmp=malloc(3*sizeof(int));
    assert(tmp);

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
    assert(tmp);

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

    if(a.data!=tmp)
        free(a.data);
    free(tmp);
}

void test_clear(){
    int* tmp=malloc(sizeof(int)*5);
    assert(tmp);

    arraylist a={tmp,tmp+5,tmp+5};

    clear(&a);
    assert(a.capacity==a.data+1);
    assert(a.size==a.data);

    clear(&a);
    assert(a.capacity==a.data+1);
    assert(a.size==a.data);

    free(a.data);
}

void test_size(){
    int *tmp=malloc(5*sizeof(int));
    assert(tmp);

    arraylist a={tmp,tmp+1,tmp+1};

    assert(size(&a)==1);

    a.size=tmp+5;
    assert(size(&a)==5);

    a.size=a.data;
    assert(size(&a)==0);

    free(tmp);
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

