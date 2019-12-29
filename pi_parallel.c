#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define N 500000000
#define K 100000
#define NTASK N/K
#define NTHR 16

int taskid = 0;
double pi = 0.0 , W = 1.0/N ;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pilock = PTHREAD_MUTEX_INITIALIZER;

void taskexecute(int t){

	int i;
	double mysum = 0.0 ;

	for( i = t*K ; i < (t+1)*K ; i ++)
		mysum += 4*W / (1 +(i + 0.5)*(i + 0.5)*W*W);

	pthread_mutex_lock(&pilock);
	pi += mysum ;
	pthread_mutex_unlock(&pilock);
}

void *thrfunc(void *arg){

	int t;

	while(1){

		pthread_mutex_lock(&lock);
		t = taskid++;
		pthread_mutex_unlock(&lock);
		if(t >= NTASK )
			break;

		taskexecute(t);

	}
}

int main(){

	struct timeval tv1 , tv2;
	double t;

	int i;
	pthread_t thr[NTHR];

	/////////////////////////////////////////////////////////////////////
	gettimeofday(&tv1,NULL);

	for(i = 0; i<NTHR; i++)
		pthread_create(&thr[i],NULL, thrfunc ,(void *)i);
	for(i = 0; i<NTHR; i++)
		pthread_join(thr[i],NULL);

	printf("pi = %.10lf\n",pi);

	gettimeofday(&tv2,NULL);
	////////////////////////////////////////////////////////////////////

	t = (tv2.tv_sec - tv1.tv_sec) +  /* seconds */
	(tv2.tv_usec - tv1.tv_usec)*1.0E-6; /* convert μsec to sec*/

	printf("real time: %lf sec. \n",t );
	return 0;
}




