#include <stdlib.h> 
#include <assert.h> 
#include <string.h>

typedef enum {false,true} bool;

//as an analogue of memcmp() 

bool arcmp(int* first,int* second,int len){
    for(int i=0;i<len;++i){
        if(first[i]!=second[i])
            return false;
    }

    return true;
}

void swap(int* x,int* y){
    int t=*x;

    *x=*y;
    *y=t;
}

int left(int i){
    return i*2+1;
}

int right(int i){
    return i*2+2;
}

bool ascending(int x,int y){
    return (x-y>0) ? true : false;
}

bool descending(int x,int y){
    return (x-y<0) ? true : false;
}

void max_heapify(int* ar,ssize_t len,int i,bool(*opt)(int,int)){
    if(i>=len || len<=0)
        return;

    int l=left(i);
    int r=right(i);
    int t=i;

    if(l<len && opt(ar[l],ar[t]))
        t=l;
    if(r<len && opt(ar[r],ar[t]))
        t=r;

    if(t!=i){
        swap(&ar[i],&ar[t]);
        max_heapify(ar,len,t,opt);
    }   
}

void heap_sort(int* ar,ssize_t len,bool(*opt)(int,int)){
    if(len<=0 || len==1)
        return;

    int cur_len=len;

    for(int i=len/2-1;i>=0;--i)
        max_heapify(ar,len,i,opt);

    for(int i=0;i<len;++i){
        swap(&ar[cur_len-1],&ar[0]);
        max_heapify(ar,--cur_len,0,opt);
    }
}

void test_heap_sort(){
    int first[10]={7,4,3,5,8,9,2,6,1,0};
    int exp_first[10]={0,1,2,3,4,5,6,7,8,9};
    heap_sort(first,10,ascending);
    assert(memcmp(first,exp_first,10*sizeof(int))==0);

    int second[1]={1};
    int exp_second[1]={1};
    heap_sort(second,1,ascending);
    assert(memcmp(second,exp_second,1*sizeof(int))==0);

    int third[10]={0};
    int exp_third[10]={0};
    heap_sort(third,10,ascending);
    assert(memcmp(third,exp_third,10*sizeof(int))==0);

    int fourth[10]={-10,85,6,-28,31,0,14,44,9,13};
    int exp_fourth[10]={-28,-10,0,6,9,13,14,31,44,85};
    heap_sort(fourth,10,ascending);
    assert(memcmp(fourth,exp_fourth,10*sizeof(int))==0);

    int fifth[5]={3,2,1,5,6};
    int exp_fifth[5]={3,2,1,5,6};
    heap_sort(fifth,-1,ascending);
    assert(memcmp(fifth,exp_fifth,5*sizeof(int))==0);

    int sixsth[2]={2,1};
    int exp_sixsth[2]={1,2};
    heap_sort(sixsth,2,ascending);
    assert(memcmp(sixsth,exp_sixsth,2*sizeof(int))==0);

    int seventh[5]={4,2,0,9,7};
    int exp_seventh[5]={9,7,4,2,0};
    heap_sort(seventh,5,descending);
    assert(memcmp(seventh,exp_seventh,5*sizeof(int))==0);

    int eighth[10]={1,-14,28,-35,89,65,0,62,34,34};
    int exp_eighth[10]={89,65,62,34,34,28,1,0,-14,-35};
    heap_sort(eighth,10,descending);
    assert(memcmp(eighth,exp_eighth,10*sizeof(int))==0);

    heap_sort(second,1,descending);
    assert(memcmp(second,exp_second,1*sizeof(int))==0);

    heap_sort(third,10,descending);
    assert(memcmp(third,exp_third,10*sizeof(int))==0);
}

int main(void)
{    
    test_heap_sort();
    
    return 0;
}
