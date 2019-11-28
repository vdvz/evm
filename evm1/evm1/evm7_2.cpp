#include <cstdlib>
#include <iostream>
#include <ctime>
#include <time.h>
#include <xmmintrin.h>

int main_2(){
	struct timespec start, end; 

	clock_t t1 = clock(); 
	const int N = 100;
	const int M = 10;
	//float *matrix_I = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_A = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_B = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_R = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	float matrix_I[N][N];
	float matrix_A[N][N];
	float matrix_B[N][N];
	float matrix_R[N][N];

	/*
	_mm_store_ps(sub_buffer, row_norms);
			std::cout<< sub_buffer[0] <<"\n";
			std::cout << sub_buffer[1] << "\n";
			std::cout << sub_buffer[2] << "\n";
			std::cout << sub_buffer[3] << "\n";
	*/
	/*
	matrix_A[0][0] = 0;
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

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) {
				matrix_I[i][j] += 1;
			}
		}
	}

	float norm_1 = 0;
	float sub_buffer[4];
	for (int j = 0; j < N; j++) {
		float sum = 0;
		auto mm_vect = _mm_setzero_ps();
		for (int i = 0; i < ceil(N / 4); i++) {
			auto row0 = _mm_set_ps(matrix_A[4 * i][j], matrix_A[4 * i + 1][j], matrix_A[4 * i + 2][j], matrix_A[4 * i + 3][j]);
			mm_vect = _mm_add_ps(mm_vect, row0);
		}
		_mm_store_ps(sub_buffer, mm_vect);
		sum = sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3];
		if (sum > norm_1) norm_1 = sum;
	}

	//std::cout << "NORM_1: " << norm_1 << "\n";



	float norm_inf = 0;
	for (int i = 0; i < N; i++) {
		float sum = 0;
		auto mm_vect = _mm_setzero_ps();
		for (int j = 0; j < ceil(N / 4); j++) {
			auto mm_sub_vect = _mm_load_ps(&matrix_A[i][4 * j]);
			//mm_vect = _mm_shuffle_ps(mm_vect, mm_vect, _MM_SHUFFLE(0, 1, 2, 3));
			mm_vect = _mm_add_ps(mm_vect, mm_sub_vect);
		}
		_mm_store_ps(sub_buffer, mm_vect);
		sum = sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3];
		if (sum > norm_inf) norm_inf = sum;
	}

	//std::cout << "NORM_INT: " << norm_inf << "\n";

	auto row_norms = _mm_set_ps1(norm_1*norm_inf);
	for (int i = 0; i < N / 4; i++) {
		for (int j = 0; j < N / 4; j++) {
			auto row0 = _mm_load_ps(&matrix_A[4 * i][4 * j]);
			auto row1 = _mm_load_ps(&matrix_A[4 * i+1][4 * j]);
			auto row2 = _mm_load_ps(&matrix_A[4 * i+2][4 * j]);
			auto row3 = _mm_load_ps(&matrix_A[4 * i+3][4 * j]);
			_MM_TRANSPOSE4_PS(row0, row1, row2, row3);
			_mm_store_ps(&matrix_B[4 * i][4 * j], _mm_div_ps(row0, row_norms));
			_mm_store_ps(&matrix_B[4 * i + 1][4 * j], _mm_div_ps(row1, row_norms));
			_mm_store_ps(&matrix_B[4 * i + 2][4 * j], _mm_div_ps(row2, row_norms));
			_mm_store_ps(&matrix_B[4 * i + 3][4 * j], _mm_div_ps(row3, row_norms));
		}
	}

	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_B[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N ; j++) {
			auto vect_R = _mm_setzero_ps();
			for (int k = 0; k < ceil(N/4); k++) {
				auto colm_A = _mm_set_ps(matrix_A[4 * k][j], matrix_A[4 * k + 1][j], matrix_A[4 * k + 2][j], matrix_A[4 * k + 3][j]);
				auto row_B = _mm_load_ps(&matrix_B[i][4 * k]);
				colm_A = _mm_shuffle_ps(colm_A, colm_A, _MM_SHUFFLE(0, 1, 2, 3));
				vect_R = _mm_add_ps(vect_R, _mm_mul_ps(row_B, colm_A));
			}
			_mm_store_ps(sub_buffer, vect_R);
			matrix_R[i][j] = matrix_I[i][j] - (sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3]);
		}
	}
	/*
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_R[i][j] << " ";
		}
		std::cout << "\n";
	}*/


	for (int i = 0; i < ceil(N / 4); i++) {
		for (int j = 0; j < ceil(N / 4); j++) {
			//matrix_I[i][j] += matrix_R[i][j];
			auto row_prev0 = _mm_load_ps(&matrix_R[4 * i][4 * j]);
			auto row_prev1 = _mm_load_ps(&matrix_R[4 * i + 1][4 * j]);
			auto row_prev2 = _mm_load_ps(&matrix_R[4 * i + 2][4 * j]);
			auto row_prev3 = _mm_load_ps(&matrix_R[4 * i + 3][4 * j]);
			auto row_I0 = _mm_load_ps(&matrix_I[4 * i][4 * j]);
			auto row_I1 = _mm_load_ps(&matrix_I[4 * i + 1][4 * j]);
			auto row_I2 = _mm_load_ps(&matrix_I[4 * i + 2][4 * j]);
			auto row_I3 = _mm_load_ps(&matrix_I[4 * i + 3][4 * j]);
			_mm_store_ps(&matrix_I[4 * i][4 * j], _mm_add_ps(row_prev0, row_I0));
			_mm_store_ps(&matrix_I[4 * i + 1][4 * j], _mm_add_ps(row_prev1, row_I1));
			_mm_store_ps(&matrix_I[4 * i + 2][4 * j], _mm_add_ps(row_prev2, row_I2));
			_mm_store_ps(&matrix_I[4 * i + 3][4 * j], _mm_add_ps(row_prev3, row_I3));
		}
	}


	float matrix_prev[N][N];
	for (int i = 0; i < ceil(N/4); i++) {
		for (int j = 0; j < ceil(N/4); j++) {
			//auto row_R = _mm_load_ps(&matrix_R[i][4 * j]);
			//_mm_store_ps(&matrix_prev[i][4 * j], row_R);
			auto row0 = _mm_load_ps(&matrix_R[4 * i][4 * j]);
			auto row1 = _mm_load_ps(&matrix_R[4 * i + 1][4 * j]);
			auto row2 = _mm_load_ps(&matrix_R[4 * i + 2][4 * j]);
			auto row3 = _mm_load_ps(&matrix_R[4 * i + 3][4 * j]);
			_mm_store_ps(&matrix_prev[4 * i][4 * j], row0);
			_mm_store_ps(&matrix_prev[4 * i + 1][4 * j], row1);
			_mm_store_ps(&matrix_prev[4 * i + 2][4 * j], row2);
			_mm_store_ps(&matrix_prev[4 * i + 3][4 * j], row3);
		}
	}
	/*
	std::cout << "MATRIX_PREV\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_prev[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	float matrix_matr[N][N];
	for (int itter = 0; itter < M - 1; itter++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				auto vect_sum = _mm_setzero_ps();
				for (int k = 0; k < ceil(N / 4); k++) {
					auto colm_R = _mm_set_ps(matrix_R[4 * k][j], matrix_R[4 * k + 1][j], matrix_R[4 * k + 2][j], matrix_R[4 * k + 3][j]);
					auto row_prev = _mm_load_ps(&matrix_prev[i][4 * k]);
					colm_R = _mm_shuffle_ps(colm_R, colm_R, _MM_SHUFFLE(0, 1, 2, 3));
					vect_sum = _mm_add_ps(vect_sum, _mm_mul_ps(row_prev, colm_R));
				}
				_mm_store_ps(sub_buffer, vect_sum);
				matrix_matr[i][j] = (sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3]);
			}
		}
		
		/*std::cout << "MATRIX_I, ITTER: " << itter << "\n";
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				std::cout << matrix_I[i][j] << " ";
			}
			std::cout << "\n";
		}*/

		for (int i = 0; i < ceil(N / 4); i++) {
			for (int j = 0; j < ceil(N / 4); j++) {
				//matrix_I[i][j] += matrix_matr[i][j];
				auto row_prev0 = _mm_load_ps(&matrix_matr[4 * i][4 * j]);
				auto row_prev1 = _mm_load_ps(&matrix_matr[4 * i + 1][4 * j]);
				auto row_prev2 = _mm_load_ps(&matrix_matr[4 * i + 2][4 * j]);
				auto row_prev3 = _mm_load_ps(&matrix_matr[4 * i + 3][4 * j]);
				auto row_I0 = _mm_load_ps(&matrix_I[4 * i][4 * j]);
				auto row_I1 = _mm_load_ps(&matrix_I[4 * i + 1][4 * j]);
				auto row_I2 = _mm_load_ps(&matrix_I[4 * i + 2][4 * j]);
				auto row_I3 = _mm_load_ps(&matrix_I[4 * i + 3][4 * j]);
				_mm_store_ps(&matrix_I[4 * i][4 * j], _mm_add_ps(row_prev0, row_I0));
				_mm_store_ps(&matrix_I[4 * i + 1][4 * j], _mm_add_ps(row_prev1, row_I1));
				_mm_store_ps(&matrix_I[4 * i + 2][4 * j], _mm_add_ps(row_prev2, row_I2));
				_mm_store_ps(&matrix_I[4 * i + 3][4 * j], _mm_add_ps(row_prev3, row_I3));
			}
		}



		for (int i = 0; i < ceil(N / 4); i++) {
			for (int j = 0; j < ceil(N / 4); j++) {
				auto row0 = _mm_load_ps(&matrix_matr[4 * i][4 * j]);
				auto row1 = _mm_load_ps(&matrix_matr[4 * i + 1][4 * j]);
				auto row2 = _mm_load_ps(&matrix_matr[4 * i + 2][4 * j]);
				auto row3 = _mm_load_ps(&matrix_matr[4 * i + 3][4 * j]);
				_mm_store_ps(&matrix_prev[4 * i][4 * j], row0);
				_mm_store_ps(&matrix_prev[4 * i + 1][4 * j], row1);
				_mm_store_ps(&matrix_prev[4 * i + 2][4 * j], row2);
				_mm_store_ps(&matrix_prev[4 * i + 3][4 * j], row3);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			auto vect_sum = _mm_setzero_ps();
			for (int k = 0; k < ceil(N / 4); k++) {
				auto colm_B = _mm_set_ps(matrix_B[4 * k][j], matrix_B[4 * k + 1][j], matrix_B[4 * k + 2][j], matrix_B[4 * k + 3][j]);
				auto row_I = _mm_load_ps(&matrix_I[i][4 * k]);
				colm_B = _mm_shuffle_ps(colm_B, colm_B, _MM_SHUFFLE(0, 1, 2, 3));
				vect_sum = _mm_add_ps(vect_sum, _mm_mul_ps(row_I, colm_B));
			}
			_mm_store_ps(sub_buffer, vect_sum);
			matrix_matr[i][j] = (sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3]);
		}
	}

	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_matr[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	clock_t t2 = clock();
	std::cout<< 1000.0 * (t2 - t1) / CLOCKS_PER_SEC<<"\n";

	system("pause");

	return 0;
}
