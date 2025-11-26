//전체 소스코드
#include <stdio.h>
#include <math.h>
typedef double (*func_t)(double, double);

double f(double t, double y)
{
    return y - t * t + 1;
}
double exact_f(double t)
{
    return (t + 1) * (t + 1) - 0.5 * exp(t);
}

double euler_step(double w, double h, func_t f, double t)
{
    return w + h * f(t, w);
}
double M_euler_step(double w, double h, func_t f, double t)
{
    double k1 = f(t, w);
    double k2 = f(t + h, w + h * k1);
    return w + h / 2.0 * (k1 + k2);
}
double RK4_step(double w, double h, func_t f, double t)
{
    double k1 = h * f(t, w);
    double k2 = h * f(t + h / 2, w + k1 / 2);
    double k3 = h * f(t + h / 2, w + k2 / 2);
    double k4 = h * f(t + h, w + k3);
    return w + 1.0 / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);
}

int main()
{
    double exact = 0.5;
    double w_euler = 0.5, w_M_euler = 0.5, w_rk4 = 0.5;
    double h_euler = 0.025, h_M_euler = 0.05, h_rk4 = 0.1;

    printf("                Euler      M-Euler    RK4\n");
    printf("t    Exact      h=0.025    h=0.05     h=0.1\n");

    for (int step = 0; step < 21; step += 1)
    {
        double t = step * 0.1;
        exact = exact_f(step * 0.1);

        printf("%.1f  %.7f  %.7f  %.7f  %.7f\n",
            t, exact, w_euler, w_M_euler, w_rk4);

        for (int i = 0; i < 4; i += 1)
            w_euler = euler_step(w_euler, h_euler, f, t + i * h_euler);

        for (int i = 0; i < 2; i += 1)
            w_M_euler = M_euler_step(w_M_euler, h_M_euler, f, t + i * h_M_euler);

        w_rk4 = RK4_step(w_rk4, h_rk4, f, t);
    }
    return 0;
}