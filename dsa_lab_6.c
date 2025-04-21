#include <stdlib.h> 
#include <stdio.h> 

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

void max_heapify(int* ar,size_t len,int i){
    if(i>=len)
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

void cons_heap(int* ar,int len){
    if(len<0)
        return;
    
    for(int i=len/2-1;i>=0;--i)
        max_heapify(ar,len,i);
}

int main(void)
{
    
    return 0;
}
