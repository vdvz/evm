#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include <ctime>
#include <algorithm>


int main() {
	int N_min = 256;
	int N_max = 8388608;

	union ticks{ 
		unsigned long long t64;
		struct s32{ long th, tl; } t32;
	} start, end; 
	//double cpu_Hz = 3000000000ULL;

	int N = N_min;
	int K = 10;
	int i, k;
	while (N <= N_max) {
		int* array = new int[N];
		
		//Straight passage
		for (i = 0; i < N; i++) {
			array[i] = i + 1;
		}
		array[N - 1] = 0;
		//Warming
		for (k = 0, i = 0; i < N; i++) k = array[k];

		//Passage
		asm("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl));
		for (k = 0,i = 0; i < N * K; i++) k = array[k];
		asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
		std::cout <<"N: "<< N <<"\nTime per 1 elem straight passage: " << (end.t64 - start.t64) / K / N << " ticks.\n";

		//Back passage
		for (int i = N - 1; i >= 0; i--) {
			array[i] = i - 1;
		}
		array[0] = N - 1;

		//Warming
		for (k = 0,i = 0; i < N; i++) k = array[k];
		//Passage
		asm("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl));
		for (k = 0,i = 0; i < N * K; i++) k = array[k];
		asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
		std::cout << "Time per 1 elem back passage: " << (end.t64 - start.t64) / K / N << " ticks.\n";

		//Random passage
		std::srand((unsigned)std::time(NULL));
		std::set<int> used;
		int index = 0;
		while (index != N) {
			int val = std::rand() % N;
			if (!used.count(val) && index != val) {
				used.insert(val);
				array[index] = val;
				index++;
			}
		}
		//Warming
		for (k = 0,i = 0; i < N; i++) k = array[k];
		//Passage
		asm("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl));
		for (k = 0,i = 0; i < N * K; i++) k = array[k];
		asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
		std::cout << "Time per 1 elem random passage: " << (end.t64 - start.t64) / K / N << " ticks.\n";
		delete []array;
		N = N * 2;
	}
	return 0;
}