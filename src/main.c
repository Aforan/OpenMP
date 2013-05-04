#include "stdio.h"
#include <stdlib.h>

#include <mpi.h>

#include "input.h"
#include "server.h"
#include "client.h"

#define TWEET_SEND_TAG 			0
#define TWEET_READ_TAG 			1
#define TWEET_ACCEPTED_TAG 		2
#define TWEET_NOT_ACCEPTED_TAG	3

int main(int argc, char **argv) {
	int tid, nthreads;
	char *cpu_name;

	/* add in MPI startup routines */
	/* 1st: launch the MPI processes on each node */
	MPI_Init(&argc,&argv);

	/* 2nd: request a thread id, sometimes called a "rank" from
	the MPI master process, which has rank or tid == 0
	*/
	MPI_Comm_rank(MPI_COMM_WORLD, &tid);

	/* 3rd: this is often useful, get the number of threads
	or processes launched by MPI, this should be NCPUs-1
	*/
	MPI_Comm_size(MPI_COMM_WORLD, &nthreads);

	/* on EVERY process, allocate space for the machine name */
	cpu_name    = (char *)calloc(80,sizeof(char));

	/* get the machine name of this particular host ... well
	at least the first 80 characters of it ... */
	gethostname(cpu_name,80);


	MPI_Status status;
	char buf[MAX_TWEET_SIZE];

	/*	Start the client and the server, don't want to start more than one client*/
	if(tid == 0) {
		startServer();
		prompt();
		MPI_Send(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
	} else if(tid == 1) {
		startClient();
		MPI_Recv(buf, MAX_TWEET_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		fprintf(stderr, "Server got the tweet %s\n", buf);
	}

	MPI_Finalize();
	return(0);
}
