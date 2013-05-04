#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "server.h"

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


char line[BUFFER_LENGTH];
char* args[MAX_ARGS];
char tweetBuffer[MAX_TWEET_SIZE];

void inputLoop();
void getInput(char*);
void getArgs(char*, char**);
void flushFlags();
void action();
void inCheck(char**);
void inputTweet();
void readTweets();
void setupInput();