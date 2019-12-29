
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define N 1024
#define NTHR 16
#define RPT N/NTHR

int A[N][N], B[N][N], C[N][N];
int readmat(char *fname, int *mat, int n), 
    writemat(char *fname, int *mat, int n);


void *thrfunc(void *args){

	int i,j,k,myid =(int) args ;
	int sum;


	for(i = myid*RPT; i< (myid+1)*RPT ; i ++){
		for(j = 0; j<N; j++){
			for(k = sum = 0; k<N; k++)
				sum += A[i][k]*B[k][j];
			C[i][j] = sum;
		}
	}
}



int main()
{

	struct timeval tv1 , tv2;
	double t;

	int i;

	pthread_t thr[NTHR];

	/* Read matrices from files: "A_file", "B_file" 
	 */
	if (readmat("Amat1024", (int *) A, N) < 0) 
		exit( 1 + printf("file problem\n") );
	if (readmat("Bmat1024", (int *) B, N) < 0) 
		exit( 1 + printf("file problem\n") );

	/////////////////////////////////////////////////////////////////////
	gettimeofday(&tv1,NULL);

	for(i = 0; i<NTHR; i++)
		pthread_create(&thr[i],NULL,thrfunc,(void*) i);
	for(i =0;i<NTHR;i++)
		pthread_join(thr[i],NULL);
	

	gettimeofday(&tv2,NULL);
	////////////////////////////////////////////////////////////////////

	t = (tv2.tv_sec - tv1.tv_sec) +  /* seconds */
	(tv2.tv_usec - tv1.tv_usec)*1.0E-6; /* convert μsec to sec*/

	

	printf("real time: %lf sec. \n",t );
	


	/* Save result in "Cmat1024"
	 */
	writemat("C2mat1024", (int *) C, N);

	return (0);
}


/* Utilities to read & write matrices from/to files
 * VVD
 */

#define _mat(i,j) (mat[(i)*n + (j)])


int readmat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "r")) == NULL)
		return (-1);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (fscanf(fp, "%d", &_mat(i,j)) == EOF)
			{
				fclose(fp);
				return (-1); 
			};
	fclose(fp);
	return (0);
}


int writemat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "w")) == NULL)
		return (-1);
	for (i = 0; i < n; i++, fprintf(fp, "\n"))
		for (j = 0; j < n; j++)
			fprintf(fp, " %d", _mat(i, j));
	fclose(fp);
	return (0);
}
