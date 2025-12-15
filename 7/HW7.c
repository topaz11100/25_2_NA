#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// 메모리 관리 공용함수 ======================
double* alloc_vec(int n)
{
    return (double*) malloc(n * sizeof(double));
}

double** alloc_mat(int n)
{
    double** m = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i += 1)
    {
        m[i] = (double*) malloc(n * sizeof(double));
    }
    return m;
}

void free_vec(double* v)
{
    free(v);
}

void free_mat(double** m, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        free(m[i]);
    }
    free(m);
}

// 선형대수 연산 공용 함수 ==========================
void mat_vec_mul(double** A, double* x, double* y, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        y[i] = 0.0;
        for (int j = 0; j < n; j += 1)
        {
            y[i] += A[i][j] * x[j];
        }
    }
}

void mat_mul(double** A, double** B, double** C, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k += 1)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void copy_vec(double* src, double* dst, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        dst[i] = src[i];
    }
}

void zeros_vec(double* v, int n)
{
    for (int i = 0; i < n; i += 1) v[i] = 0.0;
}

// 행렬 분해 및 반복 행렬 생성 =======================================
void A_to_DLU(double** A, double** D, double** L, double** U, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            D[i][j] = 0.0; L[i][j] = 0.0; U[i][j] = 0.0;
            if (i == j)      D[i][j] = A[i][j];
            else if (i > j)  L[i][j] = -A[i][j];
            else             U[i][j] = -A[i][j];
        }
    }
}

void get_jacobi_matrices(double** D, double** L, double** U, double* b, double** T, double* g, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        double d_inv = 1.0 / D[i][i];
        g[i] = b[i] * d_inv;
        for (int j = 0; j < n; j += 1)
        {
            T[i][j] = (L[i][j] + U[i][j]) * d_inv;
        }
    }
}

// 가우스 세이델의 T=(D-L)^{-1}U 중 (D-L)^{-1} 를 구하는 용도
void invert_lower_matrix(double** Low, double** Low_inv, int n)
{
    for (int j = 0; j < n; j += 1)
    {
        for (int i = 0; i < n; i += 1)
        {
            double rhs = (i == j) ? 1.0 : 0.0;
            double sum = 0.0;
            for (int k = 0; k < i; k += 1)
            {
                sum += Low[i][k] * Low_inv[k][j];
            }
            Low_inv[i][j] = (rhs - sum) / Low[i][i];
        }
    }
}

void get_gauss_seidal_matrices(double** D, double** L, double** U, double* b, double** T, double* g, int n)
{
    double** DL = alloc_mat(n);
    double** DL_inv = alloc_mat(n);

    for (int i = 0; i < n; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            DL[i][j] = D[i][j] - L[i][j];
        }
    }

    invert_lower_matrix(DL, DL_inv, n);
    mat_mul(DL_inv, U, T, n);
    mat_vec_mul(DL_inv, b, g, n);

    free_mat(DL, n);
    free_mat(DL_inv, n);
}

// 반복법으로 풀이 ===================================================

// 오차는 무한 norm을 사용한다
double get_inf_norm_error(double* x_new, double* x_old, int n)
{
    double max_diff = 0.0;
    for (int i = 0; i < n; i += 1)
    {
        double diff = fabs(x_new[i] - x_old[i]);
        if (diff > max_diff) max_diff = diff;
    }
    return max_diff;
}

// 풀이 본체 =========
double* solver(double** A, double* b, int n, char method, double TOL, int MAX_ITER)
{
    // 사용할 값의 동적 할당 및 값 할당
    double** D = alloc_mat(n); double** L = alloc_mat(n); double** U = alloc_mat(n);
    A_to_DLU(A, D, L, U, n);

    double** T = alloc_mat(n); double* g = alloc_vec(n);
    if (method == 'J')      get_jacobi_matrices(D, L, U, b, T, g, n);
    else if (method == 'G') get_gauss_seidal_matrices(D, L, U, b, T, g, n);

    double* x_old = alloc_vec(n); double* x_new = alloc_vec(n);
    zeros_vec(x_old, n); // 0으로 초기화

    // 반복횟수, 오차
    int iter = 0;
    double error = 0.0;

    do
    {
        // x_{n+1} = Tx_{n} + g
        mat_vec_mul(T, x_old, x_new, n);
        for (int i = 0; i < n; i += 1) x_new[i] += g[i];
        // 계산된 x_new, 기존 x_old 오차 계산
        error = get_inf_norm_error(x_new, x_old, n);
        // 반복을 위해 x_old <- x_new 갱신
        iter += 1; copy_vec(x_new, x_old, n);
    } while (error >= TOL && iter < MAX_ITER); // 인자로 받은 TOL, MAX_ITER 사용

    free_mat(D, n); free_mat(L, n); free_mat(U, n);
    free_mat(T, n); free_vec(g);
    free_vec(x_new);

    return x_old;
}

// 테스트 케이스 실행 ============================================

void print_result(const char* label, double* x, int n)
{
    printf("[%12s] :", label);
    for (int i = 0; i < n; i += 1)
    {
        printf("%9.5f", x[i]);
    }
    printf("\n");
}

void run_problem(char name, int n, const double* flat_A, const double* src_b, double TOL, int MAX_ITER)
{
    printf("\n=== Problem %c ===\n", name);

    // 할당 및 복사
    double** A = alloc_mat(n);
    double* b = alloc_vec(n);
    for (int i = 0; i < n; i += 1)
    {
        b[i] = src_b[i];
        for (int j = 0; j < n; j += 1)
        {
            A[i][j] = flat_A[i * n + j];
        }
    }

    // Jacobi
    double* sol_j = solver(A, b, n, 'J', TOL, MAX_ITER);
    print_result("Jacobi", sol_j, n);
    free_vec(sol_j);

    // Gauss-Seidel
    double* sol_g = solver(A, b, n, 'G', TOL, MAX_ITER);
    print_result("Gauss-Seidel", sol_g, n);
    free_vec(sol_g);

    // 메모리 해제
    free_mat(A, n);
    free_vec(b);
}

int main(void)
{
    // Problem A
    const int N_a = 3;
    const double MAT_A_a[3][3] = {
        { 3, -1,  1},
        { 3,  6,  2},
        { 3,  3,  7}
    };
    const double VEC_b_a[3] = { 1, 0, 4 };

    // Problem C
    const int N_c = 4;
    const double MAT_A_c[4][4] = {
        {10,  5,  0,  0},
        { 5, 10, -4,  0},
        { 0, -4,  8, -1},
        { 0,  0, -1,  5}
    };
    const double VEC_b_c[4] = { 6, 25, -11, -11 };

    // Problem E
    const int N_e = 5;
    const double MAT_A_e[5][5] = {
        { 4,  1,  1,  0,  1},
        {-1, -3,  1,  1,  0},
        { 2,  1,  5, -1, -1},
        {-1, -1, -1,  4,  0},
        { 0,  2, -1,  1,  4}
    };
    const double VEC_b_e[5] = { 6, 6, 6, 6, 6 };

    // 허용 오차, 반복횟수
    const double TOL = 1e-3;
    const int MAX_ITER = 100000;

    run_problem('a', N_a, (double*) MAT_A_a, VEC_b_a, TOL, MAX_ITER);
    run_problem('c', N_c, (double*) MAT_A_c, VEC_b_c, TOL, MAX_ITER);
    run_problem('e', N_e, (double*) MAT_A_e, VEC_b_e, TOL, MAX_ITER);

    return 0;
}