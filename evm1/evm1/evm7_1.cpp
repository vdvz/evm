#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>    

int main() {
	clock_t t1 = clock();
	const int N = 1024;
	const int M = 10;
	float *matrix_I = new float [N*N];
	float *matrix_A  = new float [N*N];
	float *matrix_B = new float [N*N];
	float *matrix_R = new float [N*N];
	float *matrix_E = new float [N*N];
	float *matrix_prev = new float [N*N];
	float *matrix_sub  = new float[N*N];
/*	matrix_A[0][0] = 0;
	matrix_A[0][1] = 1;
	matrix_A[0][2] = 1;
	matrix_A[0][3] = 1;
	matrix_A[1][0] = 1;
	matrix_A[1][1] = 0;
	matrix_A[1][2] = 1;
	matrix_A[1][3] = 1;
	matrix_A[2][0] = 1;
	matrix_A[2][1] = 1;
	matrix_A[2][2] = 0;
	matrix_A[2][3] = 1;
	matrix_A[3][0] = 1;
	matrix_A[3][1] = 1;
	matrix_A[3][2] = 1;
	matrix_A[3][3] = 0;
*/	
	/*for(int i = 0; i<N; i++){
			for(int j = 0; j<N; j++){
				matrix_A[i][j]=static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/1000));;
			}
		}
	*/
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_E[i*N+j] = 0;
			matrix_R[i*N + j] = 0;
			matrix_I[i*N + j] = 0;
			if (i == j) {
				matrix_E[i*N + j] += 1;
				matrix_R[i*N + j] += 1;
				matrix_I[i*N + j] += 1;
			}
		}
	}

	float norm_1 = 0;
	for (int j = 0; j < N; j++) {
		float sum = 0;
		for (int i = 0; i < N; i++) {
			sum += matrix_A[i*N+j];

		}
		if (sum > norm_1) norm_1 = sum;
	}

	float norm_inf = 0;
	for (int i = 0; i < N; i++) {
		float sum = 0;
		for (int j = 0; j < N; j++) {
			sum += matrix_A[i*N+j];
		}
		if (sum > norm_inf) norm_inf = sum;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_B[N*i+j] = matrix_A[N*j+i] / (norm_1 * norm_inf);
		}
	}

	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_B[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_R[N*i+j] = matrix_I[N*i+j];
			for (int k = 0; k < N; k++) {
				matrix_R[N*i+j] -= matrix_B[N*i+k] * matrix_A[N*k+j];
			}
		}
	}

	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_R[i][j] << " ";
		}
		std::cout << "\n";
	}*/


	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_I[N*i+j] += matrix_R[N*i+j];
		}
	}
	

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_prev[N*i+j] = matrix_R[N*i+j];
		}
	}

	for (int itter = 0; itter < M-1; itter++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrix_sub[i*N+j] = 0;
				for (int k = 0; k < N; k++) {
					matrix_sub[i*N+j] += matrix_R[i*N+k] * matrix_prev[N*k+j];
				}
			}
		}
		
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrix_I[i*N + j] += matrix_sub[N*i+j];
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrix_prev[N*i+j] = matrix_sub[N*i+j];
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_sub[i*N+j] = 0;
			for (int k = 0; k < N; k++) {
				matrix_sub[N*i+j] += matrix_I[N*i+k] * matrix_B[N*k+j];
			}
		}
	}
	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << sub_matr[i][j] << " ";
		}
		std::cout << "\n";
	}*/
	
	clock_t t2 = clock();
	std::cout << 1000.0 * (t2 - t1) / CLOCKS_PER_SEC << "\n";

	delete matrix_I;
	delete matrix_A;
	delete matrix_B;
	delete matrix_R;
	delete matrix_E;
	delete matrix_prev;
	delete matrix_sub; 

	return 0;
}
