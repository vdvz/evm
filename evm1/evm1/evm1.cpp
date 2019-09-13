#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sys/times.h>
#include <unistd.h>
#include<time.h>

using namespace std;

const unsigned long int N = 20000000;

int function() {
	unsigned long int pi = 0;
	for (int i = 0; i < N; i++) {
		pi += 4 * (pow(-1, i)) / (2 * i + 1);
	}
	return 0;
}

int main() {
	int expected_time = 15;
	string data;
	struct tms start, end;
	struct timespec start_ts, end_ts;
	const int max_buffer = 256;
	long clocks_per_sec = sysconf(_SC_CLK_TCK);
	long clocks;
	char buffer[max_buffer];
	FILE * stream = popen("vmstat", "r");
	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != nullptr) data.append(buffer);
		pclose(stream);
	}
	
	if (stoi(data.substr(data.length() - 6, 2)) <= 40){
		times(&start);
		function();
		times(&end);
		clocks = end.tms_utime - start.tms_utime;
		printf("Time taken: %lf sec.\n", (double)clocks / clocks_per_sec);
	}
	else{
		clock_gettime(CLOCK_MONOTONIC_RAW, &start_ts);
		function();
		clock_gettime(CLOCK_MONOTONIC_RAW, &end_ts);
		long double time = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
		if(true) {
			printf("Time taken: %lf sec.\n", time);
		}
	}
	return 0;
}
