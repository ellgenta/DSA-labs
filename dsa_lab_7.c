#include <stdlib.h>
#include <assert.h>

#define STR_BYTE_SIZE   9

typedef enum {false,true} bool;

bool bit(char c){
    if(c=='0' || c=='1')
        return true;
    else 
        return false;
}

/*
The automaton consists of 9 states (0-8)

The invariant for the first eight (0, 1, 2, ..., 7) states remains the same:
if the current symbol is a bit (sym=='0' or sym=='1'), then we simply increment the state 
(otherwise we return false, because the string in our case should consist only of 'zeros' and 'ones')

As for the last (8th) state, condition is the following:
if the current symbol is the null character (sym=='\0'), then we can terminate verification and return true 
(otherwise we return false, since a byte consists exactly of 8 bits)
*/

bool byte(char* str,size_t str_size){
    int state=0;

    for(size_t i=0;i<str_size;++i){
        if(state==0 && bit(str[i]))
            state=1; 
        else if(state==1 && bit(str[i]))
            state=2; 
        else if(state==2 && bit(str[i]))
            state=3; 
        else if(state==3 && bit(str[i]))
            state=4;
        else if(state==4 && bit(str[i]))
            state=5;
        else if(state==5 && bit(str[i]))
            state=6;
        else if(state==6 && bit(str[i]))
            state=7;
        else if(state==7 && bit(str[i]))
            state=8;
        else if(state==8 && str[i]=='\0')
            return true;
        else
            return false;
    }
}

void test_byte(){
    char first[STR_BYTE_SIZE]="00001111";
    assert(byte(first,STR_BYTE_SIZE));

    char second[STR_BYTE_SIZE-1]="0000111";
    assert(!byte(second,STR_BYTE_SIZE-1));

    char third[STR_BYTE_SIZE+1]="111100001";
    assert(!byte(third,STR_BYTE_SIZE+1));

    char fourth[STR_BYTE_SIZE]="11110002";
    assert(!byte(fourth,STR_BYTE_SIZE));

    char fifth[STR_BYTE_SIZE]="10101010";
    assert(byte(fifth,STR_BYTE_SIZE));
}

int main(void)
{
    test_byte();
    
}
