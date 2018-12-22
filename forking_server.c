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
	while(1) {
	  from_client = server_handshake( &to_client );
	  while(1) {
		  r = read(from_client, data, BUFFER_SIZE);
		  if(r) {
		  	printf("server received data: \"%s\"\n", data);
		  	for(i=0;2*i<strlen(data);i++) {
		  		processed[i]=data[2*i];
		  	}
		  	processed[i]=0;
		  	write(to_client, processed, BUFFER_SIZE);
		  	printf("server sent processed data: \"%s\"\n", processed);
			}
			else {
				break;
			}
		}
  }
}