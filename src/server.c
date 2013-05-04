#include "server.h"

#define MAX_TWEETS 		512
#define MAX_TWEET_SIZE 	64

char tweets[MAX_TWEETS][MAX_TWEET_SIZE];
int num_tweets;

void startServer() {
	num_tweets = 0;
}

void sendTweet(char* tweet) {
	if(!sizeof(tweet) < MAX_TWEET_SIZE) {
		fprintf(stderr, "Tweet too long, not accepted\n");
		return;
	}

	if(tweets >= MAX_TWEETS) {
		fprintf(stderr, "Max Tweets Exceeded, not accepted\n");
		return;
	}

	strcpy(tweets[num_tweets], tweet);
	num_tweets++;
}

char** getTweets() {
	return tweets;
}