#include<stdio.h>
#ifdef __INTEL_COMPILER
    #include<mkl_cblas.h>// C-интерфейсBLAS(заголовочныйфайлизMKL)
#else
    #include <cblas.h>  // C-интерфейсBLAS(стандартный заголовочный файл)
#endif
 
int main(){
	clock_t t1 = clock();
	const int N = 100;
	const int M = 10;
	float matrix_I[N][N];
	float matrix_A[N][N];
	float matrix_B[N][N];
	float matrix_R[N][N];
	float matrix_E[N][N];
	
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
	//B=trans(A)/const
	cblas_sgemm(CblasRowMajor,CblasTrans,CblasNoTrans,N,N,N,1/(norn_1*norm_inf),matrix_A,N,matrix_E,N,0.0,matrix_B,N);

	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_R[i][j] = matrix_I[i][j];
			for (int k = 0; k < N; k++) {
				matrix_R[i][j] -= matrix_B[i][k] * matrix_A[k][j];
			}
		}
	}

	//R=-AB+I
	cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,-1.0,matrix_B,N,matrix_A,N,1.0,matrix_R,N);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_I[i][j] += matrix_R[i][j];
		}
	}
	//I = R + E
	cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,matrix_R,N,matrix_E,N,1.0,matrix_I,N);

	float matrix_prev[N][N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_prev[i][j] = matrix_R[i][j];
		}
	}

	//Accumulate powers of R in I 
	for (int itter = 0; itter < M-1; itter++) {
		//mb it would not work
		cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,matrix_R,N,matrix_prev,N,0.0,matrix_prev,N);
		cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,matrix_E,N,matrix_prev,N,1.0,matrix_I,N);
	}
	//A^-1=I*B
	cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,matrix_I,N,matrix_B,N,0.0,matrix_A,N);

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
