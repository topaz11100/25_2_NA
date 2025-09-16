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

double secant(int n, double (*f)(double), double p0, double p1)
{
    if      (n == 0) return p0;
    else if (n == 1) return p1;

    double p_n = 0, p_n_1 = p1, p_n_2 = p0;
    
    for(int i = 1; i < n; i += 1)
    {
        p_n = next_p(f, p_n_1, p_n_2);
        p_n_2 = p_n_1; p_n_1 = p_n;
    }

    return p_n;
}

void format(int n, double p_n, double f_p_n)
{
    printf("%d", n);
    
    if (p_n >= 0) printf("     ");
    else          printf("    ");

    printf("%.10f", p_n);

    if (f_p_n >= 0) printf("     ");
    else            printf("    ");

    printf("%.10f", f_p_n);

    printf("\n");
}

int main()
{
    printf("n     p_n             f(p_n)\n");
    for(int i = 2; i < 7; i += 1)
    {
        double p_n = secant(i, f, 1, 2);
        format(i, p_n, f(p_n));
    }

    return 0;
}