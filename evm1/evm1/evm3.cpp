#include <stdlib.h>
#include <math.h>

int main() {
	int N = 400000000000;
	unsigned long int pi = 0;
	for (int i = 0; i < N; i++) {
		pi += 4 * (pow(-1, i)) / (2 * i + 1);
	}	
	return 0;
}