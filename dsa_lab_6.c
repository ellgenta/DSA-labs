#include <stdlib.h> 
#include <assert.h> 

typedef enum {false,true} bool;

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

void max_heapify(int* ar,ssize_t len,int i){
    if(i>=len || len<=0)
        return;

    int l=left(i);
    int r=right(i);
    int t=i;

    if(l<len && ar[l]>ar[t])
        t=l;
    if(r<len && ar[r]>ar[t])
        t=r;

    if(t!=i){
        swap(&ar[i],&ar[t]);
        max_heapify(ar,len,t);
    }   
}

void heap_sort(int* ar,ssize_t len){
    if(len<=0)
        return;

    int cur_len=len;

    for(int i=len/2-1;i>=0;--i)
        max_heapify(ar,len,i);

    for(int i=0;i<len;++i){
        swap(&ar[cur_len-1],&ar[0]);
        max_heapify(ar,--cur_len,0);
    }
}

void test_heap_sort(){
    int ft[10]={7,4,3,5,8,9,2,6,1,0};
    int exp_ft[10]={0,1,2,3,4,5,6,7,8,9};
    heap_sort(ft,10);
    assert(arcmp(ft,exp_ft,10));

    int sd[1]={1};
    int exp_sd[1]={1};
    heap_sort(sd,1);
    assert(arcmp(sd,exp_sd,1));

    int td[10]={0};
    int exp_td[10]={0};
    heap_sort(td,10);
    assert(arcmp(td,exp_td,10));

    int fh[10]={-10,85,6,-28,31,0,14,44,9,13};
    int exp_fh[10]={-28,-10,0,6,9,13,14,31,44,85};
    heap_sort(fh,10);
    assert(arcmp(fh,exp_fh,10));

    int ff[5]={3,2,1,5,6};
    int exp_ff[5]={3,2,1,5,6};
    heap_sort(ff,-1);
    assert(arcmp(ff,exp_ff,5));

    int sh[2]={2,1};
    int exp_sh[2]={1,2};
    heap_sort(sh,2);
    assert(arcmp(sh,exp_sh,2));
}

int main(void)
{    
    test_heap_sort();
    
    return 0;
}
