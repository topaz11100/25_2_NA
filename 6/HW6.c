#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const int N = 3;
const int len_vec = N * sizeof(double);

// 테스트용 계수행렬, 우변 벡터
const double A_test[4][3][3] = { { { 1, -5,  1},
                                   {10,  0, 20},
                                   { 5,  0, -1} },
                                 { { 1,  1, -1},
                                   { 1,  1,  4},
                                   { 2, -1,  2} },
                                 { { 2, -3,  2},
                                   {-4,  2, -6},
                                   { 2,  2,  4} },
                                 { { 0,  1,  1},
                                   { 1, -2, -1},
                                   { 1, -1,  1} } };

const double b_test[4][3] = { { 7,  6,  4 },
                              { 1,  2,  3 },
                              { 5, 14,  8 },
                              { 6,  4,  5 } };

// 행 상수배 후 더하기: dst_row <- dst_row + scale * src_row
void add_scaled_row(int src, double scale, int dst, double** A, double* b, int N)
{
    for (int col = 0; col < N; col += 1)
        A[dst][col] += scale * A[src][col];
    b[dst] += scale * b[src];
}

// 교환: row src <-> row dst (A와 b 둘 다)
void swap_row(int src, int dst, double** A, double* b, int N)
{
    if (src == dst) return;

    for (int col = 0; col < N; col += 1)
    {
        double tmp = A[dst][col];
        A[dst][col] = A[src][col];
        A[src][col] = tmp;
    }

    double tmp_b = b[dst];
    b[dst] = b[src];
    b[src] = tmp_b;
}

// 피봇이 0이면 아래에서 첫 번째 non-zero 행을 찾아 교환
void seek_nonzero(int d, double** A, double* b, int N)
{
    if (A[d][d] != 0.0) return;

    for (int row = d + 1; row < N; row += 1)
    {
        if (A[row][d] != 0.0)
        {
            swap_row(d, row, A, b, N);
            return;
        }
    }
}

// 표준 가우스 소거
void normal_gauss(double** A, double* b, int N)
{
    for (int col = 0; col < N; col += 1)
    {
        seek_nonzero(col, A, b, N);

        for (int row = col + 1; row < N; row += 1)
        {
            double factor = -A[row][col] / A[col][col];
            add_scaled_row(col, factor, row, A, b, N);
        }
    }
}

// |A[i][d]|가 최대인 행을 d행과 교환
void seek_max(int d, double** A, double* b, int N)
{
    int max_idx = d;
    double max_val = fabs(A[d][d]);

    for (int i = d + 1; i < N; i += 1)
    {
        double v = fabs(A[i][d]);
        if (v > max_val)
        {
            max_val = v;
            max_idx = i;
        }
    }

    swap_row(d, max_idx, A, b, N);
}

// partial pivoting
void pivoting_gauss(double** A, double* b, int N)
{
    for (int col = 0; col < N; col += 1)
    {
        seek_max(col, A, b, N);

        for (int row = col + 1; row < N; row += 1)
        {
            double factor = -A[row][col] / A[col][col];
            add_scaled_row(col, factor, row, A, b, N);
        }
    }
}

// 스케일 팩터 배열 s 계산: s[i] = max_j |A[i][j]|
double* find_s(double** A, int N)
{
    double* s = (double*) malloc(len_vec);

    for (int i = 0; i < N; i += 1)
    {
        double max_v = fabs(A[i][0]);
        for (int j = 1; j < N; j += 1)
        {
            double v = fabs(A[i][j]);
            if (v > max_v) max_v = v;
        }
        s[i] = max_v;
    }

    return s;
}

// 스케일드 부분 피봇팅: |A[i][d]| / s[i] 가 최대인 행을 선택해서 d행과 교환
void seek_scaled_max(int d, double** A, double* b, double* s, int N)
{
    int max_idx = d;
    double max_ratio = fabs(A[d][d]) / s[d];

    for (int i = d + 1; i < N; i += 1)
    {
        double ratio = fabs(A[i][d]) / s[i];
        if (ratio > max_ratio)
        {
            max_ratio = ratio;
            max_idx = i;
        }
    }

    if (max_idx != d)
    {
        swap_row(d, max_idx, A, b, N);

        // 행과 함께 스케일 팩터도 같이 교환
        double tmp = s[d];
        s[d] = s[max_idx];
        s[max_idx] = tmp;
    }
}

// 스케일드 부분 피봇팅
void scaled_pivoting_gauss(double** A, double* b, int N)
{
    double* s = find_s(A, N);  // 초기 스케일 팩터 한 번 계산

    for (int col = 0; col < N; col += 1)
    {
        seek_scaled_max(col, A, b, s, N);

        for (int row = col + 1; row < N; row += 1)
        {
            double factor = -A[row][col] / A[col][col];
            add_scaled_row(col, factor, row, A, b, N);
        }
    }

    free(s);
}

// backward substitution): Ux = b (상 삼각행렬 U)
void backward_substitution(double** A, double* b, int N)
{
    for (int i = N - 1; i >= 0; i -= 1)
    {
        double temp = 0.0;
        for (int j = i + 1; j < N; j += 1)
            temp += A[i][j] * b[j];

        b[i] = (b[i] - temp) / A[i][i];
    }
}

// 해를 표 형태로 출력, 행 a,b,c,d가 각각 4개의 시스템에 대응
void print_root(double* b, int n)
{
    const char row_name[4] = { 'a', 'b', 'c', 'd' };
    printf("%c  ", row_name[n]);
    for (int j = 0; j < N; j += 1)
    {
        printf("%14.10f", b[j]); printf("  ");
    }
    printf("\n");
}

// 테스트용 시스템 로딩: idx번째 시스템을 A,b에 복사
void load_system(int idx, double** A, double* b)
{
    for (int r = 0; r < N; r += 1)
        memcpy(A[r], A_test[idx][r], len_vec);
    memcpy(b, b_test[idx], len_vec);
}

int main(void)
{
    // 사용할 배열 동적할당
    double** A = (double**) malloc(N * sizeof(double*));
    for (int i = 0; i < N; i += 1)
        A[i] = (double*) malloc(len_vec);
    double* b = (double*) malloc(len_vec);

    printf("=== Normal ===\n");
    printf("     x1              x2              x3\n");
    for (int i = 0; i < 4; i += 1)
    {
        load_system(i, A, b);
        normal_gauss(A, b, N);
        backward_substitution(A, b, N);
        print_root(b, i);
    }

    printf("=== Partial Pivoting ===\n");
    printf("     x1              x2              x3\n");
    for (int i = 0; i < 4; i += 1)
    {
        load_system(i, A, b);
        pivoting_gauss(A, b, N);
        backward_substitution(A, b, N);
        print_root(b, i);
    }

    printf("=== Scaled Partial Pivoting ===\n");
    printf("     x1              x2              x3\n");
    for (int i = 0; i < 4; i += 1)
    {
        load_system(i, A, b);
        scaled_pivoting_gauss(A, b, N);
        backward_substitution(A, b, N);
        print_root(b, i);
    }

    // 해제
    free(b);
    for (int i = 0; i < N; i += 1)
        free(A[i]);
    free(A);

    return 0;
}
