#include <cstdlib>
#include <cmath>
#include <iostream>
#include <ctime>
#include <time.h>
#include <xmmintrin.h>

int main(){
	clock_t t1 = clock(); 
	const int N = 1024;
	const int M = 10;
	//float *matrix_I = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_A = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_B = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_R = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_E = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_prev = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	//float *matrix_sub = (float*)_mm_malloc(sizeof(float)*N*N, 16);
	float *matrix_I = new float [N*N];
	float *matrix_A  = new float [N*N];
	float *matrix_B = new float [N*N];
	float *matrix_R = new float [N*N];
	float *matrix_E = new float [N*N];
	float *matrix_prev = new float [N*N];
	float *matrix_sub  = new float[N*N];
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_E[i*N+j] = 0;
			matrix_R[i*N + j] = 0;
			matrix_I[i*N + j] = 0;
			if (i == j) {
				matrix_R[i*N + j] += 1;
				matrix_I[i*N + j] += 1;
			}
		}
	}

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

	float norm_1 = 0;
	float sub_buffer[4];
	for (int j = 0; j < N; j++) {
		float sum = 0;
		auto mm_vect = _mm_setzero_ps();
		for (int i = 0; i < ceil(N / 4); i++) {
			auto row0 = _mm_set_ps(matrix_A[4 * i*N+j], 
									matrix_A[N*(4 * i + 1)+j], 
									matrix_A[N*(4 * i + 2)+j], 
									matrix_A[N*(4 * i + 3)+j]);
			mm_vect = _mm_add_ps(mm_vect, row0);
		}
		_mm_store_ps(sub_buffer, mm_vect);
		sum = sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3];
		if (sum > norm_1) norm_1 = sum;
	}

	float norm_inf = 0;
	for (int i = 0; i < N; i++) {
		float sum = 0;
		auto mm_vect = _mm_setzero_ps();
		for (int j = 0; j < ceil(N / 4); j++) {
			auto mm_sub_vect = _mm_load_ps(&matrix_A[N*i + 4 * j]);
			//mm_vect = _mm_shuffle_ps(mm_vect, mm_vect, _MM_SHUFFLE(0, 1, 2, 3));
			mm_vect = _mm_add_ps(mm_vect, mm_sub_vect);
		}
		_mm_store_ps(sub_buffer, mm_vect);
		sum = sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3];
		if (sum > norm_inf) norm_inf = sum;
	}

	auto row_norms = _mm_set_ps1(norm_1*norm_inf);
	for (int i = 0; i < N / 4; i++) {
		for (int j = 0; j < N / 4; j++) {
			auto row0 = _mm_load_ps(&matrix_A[(4 * i)*N+4 * j]);
			auto row1 = _mm_load_ps(&matrix_A[(4 * i+1)*N+4 * j]);
			auto row2 = _mm_load_ps(&matrix_A[(4 * i+2)*N+4 * j]);
			auto row3 = _mm_load_ps(&matrix_A[(4 * i+3)*N+4 * j]);
			_MM_TRANSPOSE4_PS(row0, row1, row2, row3);
			_mm_store_ps(&matrix_B[(4 * i) * N + 4 * j], _mm_div_ps(row0, row_norms));
			_mm_store_ps(&matrix_B[(4 * i + 1) * N + 4 * j], _mm_div_ps(row1, row_norms));
			_mm_store_ps(&matrix_B[(4 * i + 2) * N + 4 * j], _mm_div_ps(row2, row_norms));
			_mm_store_ps(&matrix_B[(4 * i + 3) * N + 4 * j], _mm_div_ps(row3, row_norms));
		}
	}
	//std::cout<<"DONE!\n";
/*
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N ; j++) {
			auto vect_R = _mm_setzero_ps();
			for (int k = 0; k < ceil(N/4); k++) {
				auto colm_A = _mm_set_ps(matrix_A[4 * k * N+j], 
										matrix_A[(4 * k + 1) * N+j], 
										matrix_A[(4 * k + 2) * N+j], 
										matrix_A[(4 * k + 3) * N+j]);
				auto row_B = _mm_load_ps(&matrix_B[N*i + 4 * k]);
				colm_A = _mm_shuffle_ps(colm_A, colm_A, _MM_SHUFFLE(0, 1, 2, 3));
				vect_R = _mm_add_ps(vect_R, _mm_mul_ps(row_B, colm_A));
			}
			_mm_store_ps(sub_buffer, vect_R);
			matrix_R[N*i+j] = matrix_I[N*i+j] - (sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3]);
		}
	}*/

	//multiplay R=I-A*B
	//Check if matrix_R is not 0;
	for(int i = 0;i<N;i++){
		for(int k = 0;k<N;k++){
			auto scalar = _mm_set_ps1(matrix_B[i*N+k]);
			for(int j = 0; j<ceil(N/4);j++){
				auto row_A = _mm_load_ps(&matrix_A[N*k + 4 * j]);
				auto row_R = _mm_load_ps(&matrix_R[N*k + 4 * j]);
				row_R = _mm_sub_ps(row_R, _mm_mul_ps(row_A, scalar));
				_mm_store_ps(&matrix_R[N*k + 4 * j], row_R);			}
		}
	}
	//std::cout<<"DONE!\n";
	
	for (int i = 0; i < ceil(N / 4); i++) {
		for (int j = 0; j < ceil(N / 4); j++) {
			auto row_prev0 = _mm_load_ps(&matrix_R[(4 * i)*N+4 * j]);
			auto row_prev1 = _mm_load_ps(&matrix_R[(4 * i + 1)*N+4 * j]);
			auto row_prev2 = _mm_load_ps(&matrix_R[(4 * i + 2)*N+4 * j]);
			auto row_prev3 = _mm_load_ps(&matrix_R[(4 * i + 3)*N+4 * j]);
			auto row_I0 = _mm_load_ps(&matrix_I[N*4 * i + 4 * j]);
			auto row_I1 = _mm_load_ps(&matrix_I[N*(4 * i + 1)+4 * j]);
			auto row_I2 = _mm_load_ps(&matrix_I[N*(4 * i + 2)+4 * j]);
			auto row_I3 = _mm_load_ps(&matrix_I[N*(4 * i + 3)+4 * j]);
			_mm_store_ps(&matrix_I[4 * i * N + 4 * j], _mm_add_ps(row_prev0, row_I0));
			_mm_store_ps(&matrix_I[N*(4 * i + 1)+4 * j], _mm_add_ps(row_prev1, row_I1));
			_mm_store_ps(&matrix_I[N*(4 * i + 2)+4 * j], _mm_add_ps(row_prev2, row_I2));
			_mm_store_ps(&matrix_I[N*(4 * i + 3)+4 * j], _mm_add_ps(row_prev3, row_I3));
		}
	}
	
	//std::cout<<"DONE!\n";

	//Duplicate matrix_R to matrix_prev
	for (int i = 0; i < ceil(N/4); i++) {
		for (int j = 0; j < ceil(N/4); j++) {
			auto row0 = _mm_load_ps(&matrix_R[4 * i * N + 4 * j]);
			auto row1 = _mm_load_ps(&matrix_R[(4 * i + 1)*N + 4 * j]);
			auto row2 = _mm_load_ps(&matrix_R[(4 * i + 2)*N + 4 * j]);
			auto row3 = _mm_load_ps(&matrix_R[(4 * i + 3)*N + 4 * j]);
			_mm_store_ps(&matrix_prev[4 * i * N + 4 * j], row0);
			_mm_store_ps(&matrix_prev[N*(4 * i + 1)+4 * j], row1);
			_mm_store_ps(&matrix_prev[N*(4 * i + 2)+4 * j], row2);
			_mm_store_ps(&matrix_prev[N*(4 * i + 3)+4 * j], row3);
		}
	}
	//std::cout<<"DONE!\n";
	for (int itter = 0; itter < M - 1; itter++) {
		/*for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				auto vect_sum = _mm_setzero_ps();
				for (int k = 0; k < ceil(N / 4); k++) {
					auto colm_R = _mm_set_ps(matrix_R[4 * k * N + j], 
											 matrix_R[(4 * k + 1) * N + j], 
											 matrix_R[(4 * k + 2) * N + j], 
											 matrix_R[(4 * k + 3) * N + j]);
					auto row_prev = _mm_load_ps(&matrix_prev[N * i + 4 * k]);
					colm_R = _mm_shuffle_ps(colm_R, colm_R, _MM_SHUFFLE(0, 1, 2, 3));
					vect_sum = _mm_add_ps(vect_sum, _mm_mul_ps(row_prev, colm_R));
				}
				_mm_store_ps(sub_buffer, vect_sum);
				matrix_sub[N*i + j] = (sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3]);
			}
		}*/
		
		//sub=prev*R
		for(int i = 0;i<N;i++){
			for(int k = 0;k<N;k++){
				auto scalar = _mm_set_ps1(matrix_prev[i*N+k]);
				for(int j = 0; j<ceil(N/4);j++){
					auto row_R = _mm_load_ps(&matrix_R[N*k + 4 * j]);
					auto row_sub = _mm_load_ps(&matrix_sub[N*k + 4 * j]);
					row_sub = _mm_sub_ps(row_sub, _mm_mul_ps(row_R, scalar));
					_mm_store_ps(&matrix_sub[N*k + 4 * j], row_sub);			}
			}
		}


	
	//Accumulate in matrix_I 
		for (int i = 0; i < ceil(N / 4); i++) {
			for (int j = 0; j < ceil(N / 4); j++) {
				auto row_prev0 = _mm_load_ps(&matrix_sub[4 * i * N + 4 * j]);
				auto row_prev1 = _mm_load_ps(&matrix_sub[(4 * i + 1)*N + 4 * j]);
				auto row_prev2 = _mm_load_ps(&matrix_sub[(4 * i + 2)*N + 4 * j]);
				auto row_prev3 = _mm_load_ps(&matrix_sub[(4 * i + 3)*N + 4 * j]);
				auto row_I0 = _mm_load_ps(&matrix_I[4 * i * N + 4 * j]);
				auto row_I1 = _mm_load_ps(&matrix_I[(4 * i + 1)*N + 4 * j]);
				auto row_I2 = _mm_load_ps(&matrix_I[(4 * i + 2)*N + 4 * j]);
				auto row_I3 = _mm_load_ps(&matrix_I[(4 * i + 3)*N + 4 * j]);
				_mm_store_ps(&matrix_I[4 * i * N + 4 * j], _mm_add_ps(row_prev0, row_I0));
				_mm_store_ps(&matrix_I[(4 * i + 1)*N + 4 * j], _mm_add_ps(row_prev1, row_I1));
				_mm_store_ps(&matrix_I[(4 * i + 2)*N + 4 * j], _mm_add_ps(row_prev2, row_I2));
				_mm_store_ps(&matrix_I[(4 * i + 3)*N + 4 * j], _mm_add_ps(row_prev3, row_I3));
			}
		}
		//Duplicate matrix_sub to matrix_prev
		for (int i = 0; i < ceil(N / 4); i++) {
			for (int j = 0; j < ceil(N / 4); j++) {
				auto row0 = _mm_load_ps(&matrix_sub[4 * i * N + 4 * j]);
				auto row1 = _mm_load_ps(&matrix_sub[(4 * i + 1)*N+4 * j]);
				auto row2 = _mm_load_ps(&matrix_sub[(4 * i + 2)*N+4 * j]);
				auto row3 = _mm_load_ps(&matrix_sub[(4 * i + 3)*N+4 * j]);
				_mm_store_ps(&matrix_prev[4 * i*N + 4 * j], row0);
				_mm_store_ps(&matrix_prev[(4 * i + 1)*N + 4 * j], row1);
				_mm_store_ps(&matrix_prev[(4 * i + 2)*N + 4 * j], row2);
				_mm_store_ps(&matrix_prev[(4 * i + 3)*N + 4 * j], row3);
			}
		}
	//std::cout<<"DONE!\n";
	}

	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			auto vect_sum = _mm_setzero_ps();
			for (int k = 0; k < ceil(N / 4); k++) {
				auto colm_B = _mm_set_ps(matrix_B[4 * k * N + j], 
										matrix_B[(4 * k + 1)*N+j], 
										matrix_B[(4 * k + 2)*N+j], 
										matrix_B[(4 * k + 3)*N+j]);
				auto row_I = _mm_load_ps(&matrix_I[i*N+4 * k]);
				colm_B = _mm_shuffle_ps(colm_B, colm_B, _MM_SHUFFLE(0, 1, 2, 3));
				vect_sum = _mm_add_ps(vect_sum, _mm_mul_ps(row_I, colm_B));
			}
			_mm_store_ps(sub_buffer, vect_sum);
			matrix_sub[i*N+j] = (sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3]);
		}
	}*/
	
	//E=I*B
		for(int i = 0;i<N;i++){
			for(int k = 0;k<N;k++){
				auto scalar = _mm_set_ps1(matrix_I[i*N+k]);
				for(int j = 0; j<ceil(N/4);j++){
					auto row_B = _mm_load_ps(&matrix_B[N*k + 4 * j]);
					auto row_E = _mm_load_ps(&matrix_E[N*k + 4 * j]);
					row_E = _mm_sub_ps(row_E, _mm_mul_ps(row_B, scalar));
					_mm_store_ps(&matrix_E[N*k + 4 * j], row_E);			}
			}
		}
	//std::cout<<"DONE!\n";
	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_sub[N*i + j] << " ";
		}
		std::cout << "\n";
	}*/

	clock_t t2 = clock();
	std::cout<< 1000.0 * (t2 - t1) / CLOCKS_PER_SEC<<"\n";

	delete matrix_I;
	delete matrix_A;
	delete matrix_B;
	delete matrix_R;
	delete matrix_E;
	delete matrix_prev;
	delete matrix_sub; 

	return 0;
}
