#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 	64
#define DELIMITERS 		" \n\t"
#define MAX_ARGS 		64
#define MAX_FLAGS		3
#define QUIT_FLAG		0
#define SEND_FLAG		1
#define READ_FLAG		2
#define SEND "send"
#define READ "read"
#define QUIT "quit"

int isCloseRequested;

int no_input_flag;

int flags[MAX_FLAGS];

void inputLoop();
void getInput(char*);
void getArgs(char*, char**);
void flushFlags();
void action();
void inCheck(char**);
void sendTweet();
void readTweets();

void inputLoop() {
	isCloseRequested = 0;
	char line[BUFFER_LENGTH];
	char* args[MAX_ARGS];

	while(!isCloseRequested) {
		fprintf(stderr, "DEBUG: Start of Input Loop\n");
		getInput(line);
		getArgs(line, args);
		action();
	}
}

void getInput(char* line) {
	memset(line, '\0', sizeof(line));

	char* t = fgets(line, BUFFER_LENGTH, stdin);

	if(t == NULL) {
		no_input_flag = 1;
	} else {
		no_input_flag = 0;
	}
}

/*	
	in: 	Pointer to the line of input
	args: 	String array to place arguments
*/
void getArgs(char* in, char** args) {
	//Working pointer to args
	char** arg;
	arg = args;

	//Get the first token and continue through string placing 
	//tokens in args, last token will be NULL
	*arg++ = strtok(in, DELIMITERS);
	while((*arg++ = strtok(NULL, DELIMITERS)));

	inCheck(args);
}

/*
	args: 	String array to check for quit command
*/
void inCheck(char** args) {
	//Working pointer to args
	char** arg;	
	arg = args;

	//Reset flags
	flushFlags();

	//If there was an argument, check for the command
	if(*arg) {
		if(!strcmp(*arg, QUIT)) {
			flags[QUIT_FLAG] = 1;
		} else if(!strcmp(*arg, SEND)) {
			flags[SEND_FLAG] = 1;
		} else {
			fprintf(stdout, "Invalid input use help for commands\n");
		}
	} 
}

void flushFlags() {
	memset(flags, 0, sizeof(flags));
}

void action() {
	if(flags[QUIT_FLAG]) {
		isCloseRequested = 1;
	} if(flags[SEND_FLAG]) {
		sendTweet();
	} if(flags[READ_FLAG]) {
		readTweets();
	}
}

void sendTweet() {
	fprintf(stdout, "sending\n");
}

void readTweets() {
	fprintf(stdout, "reading\n");
}

int main(int argc, char** argv) {
	fprintf(stderr, "WTF man\n");
	inputLoop();
	return 0;
}


export PATH=`echo ${PATH} | awk -v RS=: -v ORS=: '/Haskell\ Platform/ {next} {print}'`