
#include <iostream>
#include <cstdlib>
#include <ctime>    

int main() {
	clock_t t1 = clock();
	const int N = 100;
	const int M = 10;
	float matrix_I[N][N];
	float matrix_A[N][N];
	float matrix_B[N][N];
	float matrix_R[N][N];
	/*matrix_A[0][0] = 0;
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

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_I[i][j] = 0;
		}
	}
	/*
		for(int i = 0; i<N; i++){
			for(int j = 0; j<N; j++){
				matrix_A[i][j]=static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/1000));;
			}
		}
	*/
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) {
				matrix_I[i][j] += 1;
			}
		}
	}

	float norm_1 = 0;
	for (int j = 0; j < N; j++) {
		float sum = 0;
		for (int i = 0; i < N; i++) {
			sum += matrix_A[i][j];

		}
		if (sum > norm_1) norm_1 = sum;
	}

	//std::cout << norm_1 << "\n";

	float norm_inf = 0;
	for (int i = 0; i < N; i++) {
		float sum = 0;
		for (int j = 0; j < N; j++) {
			sum += matrix_A[i][j];
		}
		if (sum > norm_inf) norm_inf = sum;
	}

	//std::cout << norm_inf << "\n";

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_B[i][j] = matrix_A[j][i] / (norm_1 * norm_inf);
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
			matrix_R[i][j] = matrix_I[i][j];
			for (int k = 0; k < N; k++) {
				matrix_R[i][j] -= matrix_B[i][k] * matrix_A[k][j];
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
			matrix_I[i][j] += matrix_R[i][j];
		}
	}
	
	float matrix_prev[N][N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_prev[i][j] = matrix_R[i][j];
		}
	}

	/*std::cout << "MATRIX_PREV\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_prev[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	float sub_matr[N][N];
	for (int itter = 0; itter < M-1; itter++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				sub_matr[i][j] = 0;
				for (int k = 0; k < N; k++) {
					sub_matr[i][j] += matrix_R[i][k] * matrix_prev[k][j];
				}
			}
		}
		/*std::cout << "MATRIX_I, ITTER: " << itter << "\n";
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				std::cout << matrix_I[i][j] << " ";
			}
			std::cout << "\n";
		}*/

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrix_I[i][j] += sub_matr[i][j];
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrix_prev[i][j] = sub_matr[i][j];
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			sub_matr[i][j] = 0;
			for (int k = 0; k < N; k++) {
				sub_matr[i][j] += matrix_I[i][k] * matrix_B[k][j];
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

	system("pause");
	return 0;
}
