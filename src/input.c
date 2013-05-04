#include "input.h"

void prompt() {
	fprintf(stderr, "Enter Command\n");
	getInput(line);
	getArgs(line, args);
	action();
}

void setupInput() {
	isCloseRequested = 0;
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
		inputTweet();
	} if(flags[READ_FLAG]) {
		readTweets();
	}
}

void inputTweet() {
	fprintf(stdout, "Enter your tweet\n");

	memset(tweetBuffer, '\0', sizeof(tweetBuffer));
	char* t = fgets(tweetBuffer, MAX_TWEET_SIZE, stdin);
}

void readTweets() {
	fprintf(stdout, "reading\n");
}