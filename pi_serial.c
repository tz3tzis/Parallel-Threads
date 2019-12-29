/* pi.c
 * ----
 *   Serial pi calculation.
 *
 * VVD
 */
#include <stdio.h>
#include <sys/time.h>

#define N     500000000      /* Intervals */


main()
{
  int    i;
  double W = 1.0 / ((double) N), W2, pi = 0.0;


	struct timeval tv1 , tv2;
	double t;
	
  W2 = W*W;   /* Save some calculations */
  W = 4*W;
  
/////////////////////////////////////////////////////////////////////
gettimeofday(&tv1,NULL);

  for (i = 0; i < N; i ++)
    pi += W / (1.0 + (0.5 + (double) i)*(0.5 + (double) i)*W2);
gettimeofday(&tv2,NULL);
////////////////////////////////////////////////////////////////////

t = (tv2.tv_sec - tv1.tv_sec) +  /* seconds */
	(tv2.tv_usec - tv1.tv_usec)*1.0E-6; /* convert μsec to sec*/

printf("real time: %lf sec. \n",t );
  printf("pi = %.10lf\n", pi);
}


