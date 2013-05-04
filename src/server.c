#include "server.h"

void startServer() {
	num_tweets = 0;
}

void sendTweet(char* tweet) {
	fprintf(stderr, "DEBUG:	tweet size is %d\n", strlen(tweet));

	if(strlen(tweet) >= MAX_TWEET_SIZE) {
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

void printTweets() {
	fprintf(stdout, "No Tweets: %d\n", num_tweets);
	int i;
	
	for(i = 0; i < num_tweets; i++) {
		fprintf(stdout, "Tweet %d: %s", i, tweets[i]);
	} 
}