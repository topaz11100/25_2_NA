#include <stdio.h>
#include <math.h>

double f(double x)
{
    return pow(x, 3) + 4 * pow(x, 2) - 10;
}

double next_p(double (*f)(double), double p_n, double p_n_1)
{
    return p_n - f(p_n) * (p_n - p_n_1) / (f(p_n) - f(p_n_1));
}

void format(int n, double p_n, double f_p_n)
{
    printf("%d", n);

    if (p_n >= 0)
        printf("     ");
    else
        printf("    ");

    printf("%.10f", p_n);

    if (f_p_n >= 0)
        printf("     ");
    else
        printf("    ");

    printf("%.10f", f_p_n);

    printf("\n");
}

double secant(double TOL, int TRIAL, double (*f)(double), double p0, double p1)
{
    double p_n = 0, p_n_1 = p1, p_n_2 = p0;

    double tol = 0.005;
    int trial = 1;

    while (tol >= TOL && trial <= TRIAL)
    {
        double p_n = next_p(f, p_n_1, p_n_2);
        p_n_2 = p_n_1; p_n_1 = p_n;

        tol = fabs(p_n_1 - p_n_2); trial += 1;

        format(trial, p_n, f(p_n));
    }

    return p_n;
}

int main()
{
    printf("n     p_n             f(p_n)\n");
    double p_n = secant(0.0005, 20, f, 1, 2);

    return 0;
}