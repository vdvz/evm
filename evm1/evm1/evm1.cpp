#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

const unsigned long int N = 20000000;

int main() {
	unsigned long int pi = 0;

	for (int i = 0; i < N; i++) {
		pi += 4 * (pow(-1, i)) / (2 * i + 1);
	}
	string data;
	const int max_buffer = 256;
	char buffer[max_buffer];


	FILE * stream = popen("vmstat", "r");
	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != nullptr) data.append(buffer);
		pclose(stream);
	}
	
	cout << data;

	return 0;
}
