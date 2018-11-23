#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>

double *initial_x();
double *initial_u();
double *flux(double *u);
double *Lax(double *u, double t);

int main(int argc, char *argv)
{
    #pragma omp parallel
    {
      int thread_id = omp_get_thread_num();
      double x[];
      double u[];
      double u_final[];
      double t_max_values[] = {0.0, 0.5, 1.0, 2.0};

      int i;
      for(i = 0; i < sizeof(t_max_values); i++)
      {
          x = initial_x();
          u = initial_u();
    
          u_final = Lax(u, t_max_values[i]);

          for(p = 0 ; p < sizeof(x); p++){
              fprintf(stdout, "%f,%f,%f,%d\n", x[i], u_final[i], t_max_values[i],thread_id);
          }
      }
    }
    return 0;
}

double *initial_x()
{
    double delta_x = 0.05;
    double L = 4.0;
    double N = L/delta_x + 1;
    double x[N];
    
    int i;
    for(i = 1; i < N; i++)
    {
        x[i] = x[i-1] + delta_x;
    }

    return x;
}

double *initial_u()
{
    double delta_x = 0.05;
    double L = 4.0;
    double N = L/delta_x + 1;
    double u[N];
    
    int i;
    for(i = 0; i < int(N/2); i++)
    {
        u[i] = 1.0;
    }

    return u;
}

double *flux(double *u)
{
    double flux[sizeof(u)];

    int i;
    for(i = 0; i < sizeof(u); i++)
    {
        flux[i] = 0.5*u[i]*u[i];
    }

    return flux;
}

double *Lax(double *u, double t)
{
    double t_max = t;
    double delta_t = 0.025;
    double delta_x = 5.0;

    int N_t = int(t_max/delta_t);
    double u_final[sizeof(u)];
    memcpy(u_final, u, sizeof(u_final));

    int i;
    for(i = 0; i < N_t; i++)
    {
        double *F = flux(u);
        int j;
        for(j = 1; j < sizeof(u_final)-1; j++)
        {
            u_final[j] = 0.5*(u[j+1] + u[j-1]);
            u_final[j] -= (0.5*delta_t/delta_x)*(F[j+1] - F[j-1])
        }
        memcpy(u, u_final, sizeof(u));
    }

    return u_final;
}