#include "server.h"


void startServer() {
	num_tweets = 0;
}

void sendTweet(char* tweet) {
	if(!sizeof(tweet) < MAX_TWEET_SIZE) {
		fprintf(stderr, "Tweet too long, not accepted\n");
		return;
	}

	if(num_tweets >= MAX_TWEETS) {
		fprintf(stderr, "Max Tweets Exceeded, not accepted\n");
		return;
	}

	strcpy(tweets[num_tweets], tweet);
	num_tweets++;
}