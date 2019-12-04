#include<stdio.h>
#include <ctime>
#include <time.h>
#ifdef __INTEL_COMPILER
    #include<mkl_cblas.h>// C-интерфейсBLAS(заголовочныйфайлизMKL)
#else
    #include <cblas.h>  // C-интерфейсBLAS(стандартный заголовочный файл)
#endif
 
int main(){
	
	clock_t t1 = clock();
	const int N = 4;
	const int M = 10;
	float matrix_I[N][N];
	float matrix_A[N][N];
	float matrix_B[N][N];
	float matrix_R[N][N];
	float matrix_E[N][N];

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
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_E[i][j] = 0;
			matrix_R[i][j] = 0;
			if (i == j) {
				matrix_E[i][j] += 1;
				matrix_R[i][j] += 1;
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

	float norm_inf = 0;
	for (int i = 0; i < N; i++) {
		float sum = 0;
		auto mm_vect = _mm_setzero_ps();
		for (int j = 0; j < ceil(N / 4); j++) {
			auto mm_sub_vect = _mm_load_ps(&matrix_A[i][4 * j]);
			mm_vect = _mm_add_ps(mm_vect, mm_sub_vect);
		}
		_mm_store_ps(sub_buffer, mm_vect);
		sum = sub_buffer[0] + sub_buffer[1] + sub_buffer[2] + sub_buffer[3];
		if (sum > norm_inf) norm_inf = sum;
	}


	//B=trans(A)/const
	cblas_sgemm(CblasRowMajor,CblasTrans,CblasNoTrans,N,N,N,1/(norn_1*norm_inf),*matrix_A,N,*matrix_E,N,0.0,*matrix_B,N);

	//R=-AB+I
	cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,-1.0,*matrix_B,N,*matrix_A,N,1.0,*matrix_R,N);

	//I = R + E
	cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,*matrix_R,N,*matrix_E,N,1.0,*matrix_I,N);
	//Accumulate powers of R in I 
	for (int itter = 0; itter < M-1; itter++) {
		//mb it would not work
		cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,*matrix_R,N,*matrix_prev,N,0.0,*matrix_prev,N);
		cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,*matrix_E,N,*matrix_prev,N,1.0,*matrix_I,N);
	}
	//A^-1=I*B
	cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,*matrix_I,N,*matrix_B,N,0.0,*matrix_A,N);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << matrix_A[i][j] << " ";
		}
		std::cout << "\n";
	}

	clock_t t2 = clock();
	std::cout << 1000.0 * (t2 - t1) / CLOCKS_PER_SEC << "\n";

	system("pause");

    return 0;
}
