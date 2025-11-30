#include "exec.h"

void run(solving_method method, double * runtime, void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght){
    clock_t start = clock();
    method(tour, dist, best, distmin, lenght);
    clock_t end = clock();
    (*runtime) = (double)(end - start) / CLOCKS_PER_SEC;
}