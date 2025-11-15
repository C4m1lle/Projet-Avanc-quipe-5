#ifndef __OPT_H__
#define __OPT_H__





int opt2(tTournee tour, DistanceFunc dist, int *bestTour, double *bestDist);
int is_crossed(tInstance A,tInstance B,tInstance C,tInstance D);
#endif