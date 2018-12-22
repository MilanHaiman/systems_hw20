#include "pipe_networking.h"

static void sighandler(int signo) {
	if (signo == SIGINT) {
		remove("WKP");
		exit(0);
	}
}

int main() {
	signal(SIGINT, sighandler);
	int to_client;
	int from_client;
	char data[BUFFER_SIZE];
	char processed[BUFFER_SIZE];
	int r;
	int i;
	int f;
	while(1) {
	  	// start handshake
		mkfifo("WKP", 0666);
  		printf("server made WKP\n");

  		from_client = open("WKP", O_RDONLY);
  		printf("server connected to upstream pipe\n");

  		f = fork();
		if (f) {
			close(from_client);
			remove("WKP");
		 	printf("server removed WKP\n");
		}
		else {
			// finish handshake
			subserver_handshake(&to_client, from_client);
			while(1) {
			  	r = read(from_client, data, BUFFER_SIZE);
			  	// printf("subserver %d has r = %d\n", getpid(), r);
			  	if(r) {
				  	printf("subserver %d received data: \"%s\"\n", getpid(), data);
				  	for(i=0;2*i<strlen(data);i++) {
				  		processed[i]=data[2*i];
				  	}
				  	processed[i]=0;
				  	write(to_client, processed, BUFFER_SIZE);
				  	printf("subserver %d sent processed data: \"%s\"\n", getpid(), processed);
				}
				else {
					printf("subserver %d exiting\n", getpid());
					return 0;
				}
			}
		}

	  	
  }
}