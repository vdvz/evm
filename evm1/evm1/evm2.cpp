#include <stdlib.h>
#include <math.h>
#include <string>


int function(int N) {
	unsigned long int pi = 0;
	for (int i = 0; i < N; i++) {
		pi += 4 * (pow(-1, i)) / (2 * i + 1);
	}
	return 0;
}

int main(int argc, char *argv[]){
    int N = *argv[1];
    function(1);
    
    return 0;
}