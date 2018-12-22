#include "pipe_networking.h"


/*=========================
  subserver_handshake
  args: int * to_client, int from_client

  Performs the subserver side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int subserver_handshake(int *to_client, int from_client) {
  int upfd = from_client;

  char private_name[HANDSHAKE_BUFFER_SIZE];
  read(upfd, private_name, HANDSHAKE_BUFFER_SIZE);
  printf("subserver %d received message: \"%s\"\n", getpid(), private_name);

  int downfd = open(private_name, O_WRONLY);
  printf("subserver %d connected to downstream pipe\n", getpid());

  write(downfd, private_name, HANDSHAKE_BUFFER_SIZE);
  printf("subserver %d sent message: \"%s\"\n", getpid(), private_name);

  char message[HANDSHAKE_BUFFER_SIZE];
  read(upfd, message, HANDSHAKE_BUFFER_SIZE);
  printf("subserver %d received message: \"%s\"\n", getpid(), message);

  *to_client = downfd;

  return upfd;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char private_name[HANDSHAKE_BUFFER_SIZE];
  sprintf(private_name, "-%d-", getpid());
  mkfifo(private_name, 0666);
  printf("client made pipe\n");

  int upfd = open("WKP", O_WRONLY);
  printf("client connected to upstream pipe\n");

  write(upfd, private_name, HANDSHAKE_BUFFER_SIZE);
  printf("client sent message: \"%s\"\n", private_name);

  int downfd = open(private_name, O_RDONLY);
  printf("client connected to downstream pipe\n");

  char message[HANDSHAKE_BUFFER_SIZE];
  read(downfd, message, HANDSHAKE_BUFFER_SIZE);
  printf("client received message: \"%s\"\n", message);

  remove(private_name);
  printf("client removed private pipe\n");

  write(upfd, ACK, HANDSHAKE_BUFFER_SIZE);
  printf("client sent message: \"%s\"\n", ACK);

  *to_server = upfd;

  return downfd;
}
