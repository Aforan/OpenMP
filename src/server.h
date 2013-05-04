#include <stdio.h>
#include <string.h>

#define MAX_TWEETS 		512
#define MAX_TWEET_SIZE 	64

char tweets[MAX_TWEETS][MAX_TWEET_SIZE];
int num_tweets;
int accepted;

void startServer();
void sendTweet(char*);