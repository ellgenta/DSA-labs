#include <stdlib.h>
#include <assert.h>

typedef struct _array
{
    int *data;
    int *size;
    int *capacity;
} arraylist;

#define NAN 0

typedef struct _return_type{
    int error_code;
    int returned_value;
} return_type;

/*
    exit codes description:
    0  if errors does not occur - default exit code
    
    -1 if memory was not allocated 
    -2 if passed argument is transcended
    -3 if the order of the pointers (data, size, capacity) is violated
    -4 if the function call is not opportune
*/

return_type size(arraylist* ar){
    return_type res={0,0};

    if(ar->size-ar->data<0){
        res.returned_value=NAN;
        res.error_code=-3;
    }
    else
        res.returned_value=ar->size-ar->data;

    return res;
}

return_type push_back(arraylist* ar,int value){
    return_type res={0,0};

    int length=size(ar).returned_value;
    if(length==NAN){
        return size(ar);
    }

    int cap=ar->capacity-ar->data;

    if(!cap){
        if(!ar->data){
            ar->data=malloc(sizeof(int));
            if(!ar->data){
                res.error_code=-1;
                return res;
            }
        }
        ar->capacity=ar->data+1;
        ar->size=ar->data+1;
        ar->data[0]=value;
    }else{
        if(ar->capacity==ar->size){
            ar->data=realloc(ar->data,cap*2*sizeof(int));
            if(!ar->data){
                res.error_code=-1;
                return res;
            }
            ar->capacity=ar->data+cap*2;
            ar->size=ar->data+length;
            *(ar->size++)=value;
        }
        else
            *(ar->size++)=value;
    }

    return res;
}

return_type pop_back(arraylist* ar){
    return_type res={0,0};

    int length=size(ar).returned_value;
    if(length==NAN){
        return size(ar);
    }

    int cap=ar->capacity-ar->data;

    if(ar->size==ar->data){
        res.error_code=-4;
        return res;
    }

    --ar->size;

    return res;
}

return_type insert(arraylist* ar,int indx,int value){
    return_type res={0,0};

    int length=size(ar).returned_value;
    if(length==NAN){
        return size(ar);
    }

    int cap=ar->capacity-ar->data;

    if(indx>=length || indx<0){
        res.error_code=-2;
        return res;
    }

    if(!ar->data && !indx){
        ar->data=malloc(sizeof(int));
        if(!ar->data){
            res.error_code=-1;
            return res;
        }
        ar->capacity=ar->data+1;
        ar->size=ar->data+1;
        ar->data[0]=value;
    }else if(!length && !indx){
        ar->data[0]=value;
        ++ar->size;
    }else if(ar->size==ar->capacity){
        ar->data=realloc(ar->data,cap*2*sizeof(int));
        if(!ar->data){
            res.error_code=-1;
            return res;
        }
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

    return res;
}

return_type at(arraylist* ar,int indx){
    return_type res={0,0};

    int length=size(ar).returned_value;
    if(length==NAN){
        return size(ar);
    }

    int cap=ar->capacity-ar->data;

    if(!length){
        res.error_code=-4;
        return res;
    }

    if(indx<0 || indx>=length){
        res.error_code=-2;
        return res;
    }

    res.returned_value=ar->data[indx];
    return res;
}

return_type initialize(arraylist *ar){
    return_type res={0,0};

    ar->data=malloc(sizeof(int));

    if(!ar->data){
        res.error_code=-1;
        return res;
    }

    ar->size=ar->data;
    ar->capacity=ar->data+1;

    return res;
}

return_type delete(arraylist* ar,int indx){
    return_type res={0,0};

    int length=size(ar).returned_value;
    if(length==NAN){
        return size(ar);
    }

    int cap=ar->capacity-ar->data;

    if(indx<0 || indx>=length){
        res.error_code=-2;
        return res;
    }

    if(!length){
        res.error_code=-4;
        return res;
    }

    for(int i=indx;i<length-1;++i)
        ar->data[i]=ar->data[i+1];
    --ar->size;

    return res;
}

return_type resize(arraylist *ar,int newsize){
    return_type res={0,0};

    int length=size(ar).returned_value;
    if(length==NAN){
        return size(ar);
    }

    int cap=ar->capacity-ar->data;

    if(newsize==length)
        return res;

    if(newsize<0){
        res.error_code=-2;
        return res;
    }

    if(newsize>cap){
        ar->data=realloc(ar->data,newsize);
        if(!ar->data){
            res.error_code=-1;
            return res;
        }
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

    return res;
}

return_type clear(arraylist* ar){
    return_type res={0,0};

    ar->data=realloc(ar->data,sizeof(int));
    if(!ar->data){
        res.error_code=-1;
        return res;
    }

    ar->capacity=ar->data ? ar->data+1 : NULL;
    ar->size=ar->data;

    return res;
}

void test_push_back() {
    arraylist a;
    a.capacity=NULL;
    a.size=NULL;
    a.data=NULL;

    assert(push_back(&a, 1).error_code==0);
    assert(a.data!=NULL);
    assert(a.size-a.data==1);
    assert(a.capacity-a.data==1);
    assert(a.data[0]==1);

    assert(push_back(&a, 2).error_code==0);
    assert(a.capacity-a.data==2);
    assert(a.size-a.data==2);
    assert(a.data!=NULL);
    assert(a.data[1]==2);

    a.size=a.data;
    assert(push_back(&a, 3).error_code==0);
    assert(a.capacity-a.data==2);
    assert(a.size-a.data==1);
    assert(a.data[0]==3);

    free(a.data);
}

void test_pop_back(){
    int *tmp=malloc(2*sizeof(int));
    assert(tmp);

    arraylist a={tmp,tmp+2,tmp+2};
    assert(pop_back(&a).error_code==0);
    assert(a.capacity==a.size+1);
    assert(a.data==a.size-1);

    assert(pop_back(&a).error_code==0);
    assert(a.size==a.data);
    assert(a.capacity==a.size+2);

    a.size=a.data;
    assert(pop_back(&a).error_code==-4);
    assert(a.size==a.data);

    free(tmp);
}

void test_insert(){
    int *tmp=malloc(sizeof(int));
    assert(tmp);

    arraylist a={tmp,tmp,tmp};
    assert(insert(&a,0,0).error_code==-2);
    assert(a.capacity==a.data);
    assert(a.size==a.data);

    a.capacity=tmp+1;
    a.size=tmp+1;
    a.data[0]=-1;

    assert(insert(&a,0,0).error_code==0);
    assert(a.data[0]==0);
    assert(a.capacity==a.data+2);
    assert(a.size==a.capacity);

    assert(insert(&a,0,1).error_code==0);
    assert(a.capacity==a.data+4);
    assert(a.size==a.capacity-1);
    assert(a.data[0]==1);

    assert(insert(&a,0,2).error_code==0);
    assert(a.capacity==a.size);
    assert(a.capacity-a.data==4);
    assert(a.data[0]==2);
    assert(a.data[1]==1);
    assert(a.data[2]==0);
    assert(a.data[3]==-1);

    assert(insert(&a,4,0).error_code==-2);
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
    assert(at(&a,-1).error_code==-2);

    a.data=NULL;
    a.size=NULL;
    a.capacity=NULL;
    assert(at(&a,0).error_code==-4);

    free(tmp);
}

void test_initialize(){
    arraylist a;
    assert(initialize(&a).error_code==0);

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

    assert(delete(&a,1).error_code==0);
    assert(a.data[1]==2);
    assert(a.capacity-a.size==1);

    assert(delete(&a,2).error_code==-2);
    assert(a.capacity==a.data+3);
    assert(a.capacity-a.size==1);

    free(tmp);
}

void test_resize(){
    int *tmp=malloc(sizeof(int));
    assert(tmp);

    arraylist a={tmp,tmp+1,tmp+1};

    assert(resize(&a,1).error_code==0);
    assert(a.capacity==a.data+1);
    assert(a.size==a.capacity);

    assert(resize(&a,5).error_code==0);
    assert(a.capacity==a.data+5);
    assert(a.size=a.capacity);
    for(int i=1;i<5;++i)
        assert(a.data[i]==0);

    assert(resize(&a,2).error_code==0);
    assert(a.capacity==a.data+5);
    assert(a.size==a.data+2);

    assert(resize(&a,-1).error_code==-2);
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

    assert(clear(&a).error_code==0);
    assert(a.capacity==a.data+1);
    assert(a.size==a.data);

    assert(clear(&a).error_code==0);
    assert(a.capacity==a.data+1);
    assert(a.size==a.data);

    free(a.data);
}

void test_size(){
    int *tmp=malloc(5*sizeof(int));
    assert(tmp);

    arraylist a={tmp,tmp+1,tmp+1};

    assert(size(&a).returned_value==1);

    a.size=tmp+5;
    assert(size(&a).returned_value==5);

    a.size=a.data;
    assert(size(&a).returned_value==0);

    a.size=a.data-1;
    assert(size(&a).error_code==-3);
    assert(size(&a).returned_value==(int)NAN);

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
