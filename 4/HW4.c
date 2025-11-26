#include <stdio.h>
#include <math.h>
typedef double (*func_t)(double);

double f_a(double x)
{
    return exp(2 * x) * sin(3 * x);
}

double f_b(double x)
{
    return exp(3 * x) * sin(2 * x);
}

double f_c(double x)
{
    return 2 * x * cos(2 * x) - (x - 2) * (x - 2);
}

double f_d(double x)
{
    return 4 * x * cos(2 * x) - (x - 2) * (x - 2);
}

double S(func_t f, double a, double b)
{
    return (f(a) + 4 * f((a + b) / 2) + f(b)) * (b - a) / 6;
}

double AQ(func_t f, double a, double b, double TOL)
{
    double o = S(f, a, b);
    double l = S(f, a, (a + b) / 2);
    double r = S(f, (a + b) / 2, b);
    if (fabs(o - l - r) <= 15 * TOL)
        return l + r;

    l = AQ(f, a, (a + b) / 2, TOL / 2);
    r = AQ(f, (a + b) / 2, b, TOL / 2);
    return l + r;
}

int main()
{
    double TOL = 1e-5;
    printf("TOL : %16.10f\n", TOL);

    printf("  a : %16.10f\n", AQ(f_a, 1, 3, TOL));
    printf("  b : %16.10f\n", AQ(f_b, 1, 3, TOL));
    printf("  c : %16.10f\n", AQ(f_c, 0, 5, TOL));
    printf("  d : %16.10f\n", AQ(f_d, 0, 5, TOL));

    return 0;
}