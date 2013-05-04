#include "server.h"

void startServer() {
	num_tweets = 0;
	fprintf(stderr, "starting server\n");
}

void sendTweet(char* tweet) {
	fprintf(stderr, "DEBUG:	tweet size is %d\n", sizeof(tweet));
	
	if(sizeof(tweet) >= MAX_TWEET_SIZE) {
		accepted = 0;
		fprintf(stderr, "Tweet too long, not accepted\n");
		return;
	}

	if(num_tweets >= MAX_TWEETS) {
		accepted = 0;
		fprintf(stderr, "Max Tweets Exceeded, not accepted\n");
		return;
	}

	strcpy(tweets[num_tweets], tweet);
	num_tweets++;
	accepted = 1;
}