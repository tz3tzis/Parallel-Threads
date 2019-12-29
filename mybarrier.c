#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


#define NTHR 10

typedef int bool;

#define true 1
#define false 0

struct mybar {

	int remain ;
	int count;
	bool flag;
	pthread_mutex_t lock ;
	pthread_cond_t  cv;

};

struct mybar bar1;
static const struct mybar EmptyStruct;

////////////////////////////////////////////////////////////////////////////////////////////////////
int barrier_init(struct mybar *bar1 ,int number){

	bar1->flag = false;
	bar1->count = number;                     	
	bar1->remain = number;                     	//arxikopoihsh tou arithmou twn nhmatwn
	pthread_mutex_init(&bar1->lock,NULL); 	  	//arxikopoihsh tou mutex
	pthread_cond_init(&bar1->cv,NULL);			//arxikopoihsh ths metavlhths sunthikis

	return 0;

	

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int barrier_wait(struct mybar *bar1 ) {


	


	pthread_mutex_lock(&bar1->lock);



		bar1->remain--;
		//printf("remain2 = %d\n",bar1->remain );

		printf("remain = %d flag = %d \n" ,bar1->remain ,bar1->flag );

		if (bar1->remain == 0) {

			pthread_cond_broadcast(&bar1->cv); 
			//printf("esteila broadcast...ksupnhste!! \n" );
			//bar1->flag=true;
			//f(bar1->flag == true){

			bar1->remain = bar1->count;
			//bar1->flag = false;

			//pthread_mutex_lock(&bar1->lock); 


		}else {
			
			
	  		while(bar1->remain != 0) {
	  			//printf("perimenw broadcast ...\n");
	  			printf("remain = %d waiting for broadcast  , flag = %d \n",bar1->remain ,bar1->flag);
	  			pthread_cond_wait(&bar1->cv, &bar1->lock);
	  		 }

	  	}
		
	pthread_mutex_unlock(&bar1->lock); 

	printf("ksekleidwsa to mutex... \n" );


	

	return 0;
	

}
///////////////////////////////////////////////////////////////////////////////////
int barrier_destroy(){

		bar1 = EmptyStruct ;
		return 0;

} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sum = 0;
int multi = 0;
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

/* Initialized in main() */
void *thrfunc(void *arg) {

	int i, me = (int) arg;



	pthread_mutex_lock(&mx);
	sum ++;
	pthread_mutex_unlock(&mx);

	barrier_wait(&bar1);

	printf("sum =  %d \n" ,sum);
	printf("teleiwsa ..eimai to nhma :  %d \n",me);


	pthread_mutex_lock(&mx);
	multi = sum*2;
	pthread_mutex_unlock(&mx);

	barrier_wait(&bar1);

	printf("multi =  %d \n" ,multi);

	printf("twra teleiwsa ..egw eimai to nhma :  %d \n",me); 


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){


	int i;
	pthread_t thr[NTHR];
	
	
	barrier_init(&bar1,NTHR);

	for(i = 0; i<NTHR; i++)
		pthread_create(&thr[i],NULL, thrfunc ,(void *)i);
	
	for(i = 0; i<NTHR; i++)
		pthread_join(thr[i],NULL);
	


	return 0;
}