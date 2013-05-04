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
#define SHOULD_CLOSE_TAG		4
#define READY_TO_CLOSE_TAG		5
#define PRINT_DONE_TAG			6

#define CLIENT_DEST 0
#define SERVER_DEST 1

void serverLoop();
void clientLoop();

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

	/*	Start the client and the server, don't want to start more than one client*/
	if(tid == CLIENT_DEST) {
		startClient();
		clientLoop();
	} else if(tid == SERVER_DEST) {
		startServer();
		serverLoop();		
	}
	
	MPI_Finalize();

	return(0);
}


void serverLoop() {
	int shouldClose = 0;
	MPI_Status status;
	char buf[MAX_TWEET_SIZE];

	while(!shouldClose) {

		//	Receive the potential tweet
		MPI_Recv(buf, MAX_TWEET_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		//	If the tag is Tweet Send go ahead and try to send the tweet
		if(status.MPI_TAG == TWEET_SEND_TAG) {
			sendTweet(buf);

			//	Check if accepted and respond accordingly
			if(accepted) {
				MPI_Send(buf, MAX_TWEET_SIZE, MPI_CHAR, CLIENT_DEST, TWEET_ACCEPTED_TAG, MPI_COMM_WORLD);
			} else {
				MPI_Send(buf, MAX_TWEET_SIZE, MPI_CHAR, CLIENT_DEST, TWEET_NOT_ACCEPTED_TAG, MPI_COMM_WORLD);
			}

		//	If the tag is should close, get ready and send ready to close message to client
		} else if (status.MPI_TAG == TWEET_READ_TAG) {
			printTweets();
			MPI_Send(buf, MAX_TWEET_SIZE, MPI_CHAR, CLIENT_DEST, PRINT_DONE_TAG, MPI_COMM_WORLD);
		} else if (status.MPI_TAG == SHOULD_CLOSE_TAG) {
			shouldClose = 1;
			MPI_Send(buf, MAX_TWEET_SIZE, MPI_CHAR, CLIENT_DEST, READY_TO_CLOSE_TAG, MPI_COMM_WORLD);
		}
	}
}

void clientLoop() {
	int shouldClose = 0;
	MPI_Status status;

	while(!shouldClose) {
		
		//	Prompt the user for info
		prompt();

		//	As long as user is not requesting close, send the tweet, and receive the response (can just ignore it for now)
		if(!isCloseRequested) {
			//	If the user requested a read, tell the server
			if(read_flag) {
				MPI_Send(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, SERVER_DEST, TWEET_READ_TAG, MPI_COMM_WORLD);
				MPI_Recv(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			} else {
			//	If they didn't, they must be sending a tweet, tell the server
				MPI_Send(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, SERVER_DEST, TWEET_SEND_TAG, MPI_COMM_WORLD);
				MPI_Recv(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			}
		} else {

			//	If the user has requested close, tell the server and wait for it to be ready to close.
			MPI_Send(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, SERVER_DEST, SHOULD_CLOSE_TAG, MPI_COMM_WORLD);
			MPI_Recv(tweetBuffer, MAX_TWEET_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
			shouldClose = 1;
		}
	}
}