#include <iostream>
int main(){
    int N = 32;
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
    
        for (int j = 0 ; j < value; j++){
		asm("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl));
		for (k = 0,i = 0; i < N * K; i++) k = array[k];
		asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
		sub_time = (double)(end.t64 - start.t64);
		if (sub_time < time) time = sub_time;
		}
        std::cout <<"N: "<< i <<"\nTime per 1 elem : " << time / i / OFFSET << " ticks.\n";
        delete array;
    
    }

    return 0;
}