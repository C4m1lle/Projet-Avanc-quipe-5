#ifndef __EXEC_H__
#define __EXEC_H__

#include "../tsp/types.h"
#include <time.h>

void run(solving_method method, double * runtime, void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght);
#endif