#include <iostream>
int main(){
    int N = 10;
    int OFFSET = 4194304;
    union ticks{ 
    unsigned long long t64;
    struct s32{ long th, tl; } t32;
	} start, end; 
    
    double time;
    int k, i, j;
	int value = 10;
	double sub_time, time = 1111111111111.0;

    for(i = 1; i <= N; i++){
        int* array = new int[i*OFFSET];
        for(j = 0; j < i*OFFSET; j++){
            if(j >= (i-1)*OFFSET){
                array[j] = (j + OFFSET)%(i*OFFSET)+1;
            } else array[j] = (j + OFFSET)%(i*OFFSET);
        }
        for(j = 0; j < i*OFFSET; j++){
            std::cout<<array[j]<<" ";
        }
        std::cout<<std::endl;

    return 0;
}