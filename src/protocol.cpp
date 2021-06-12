#include "headers/protocol.h"
#include <signal.h>
#include <stdlib.h>

int sigpipeReceived;

void sigpipeHandler(int parameter) {
  sigpipeReceived = 1;
  printf("SIGPIPE ERROR\n");
}

void sendMove(FILE *pipe, PlayerMessage m) {
  sigpipeReceived = 0;
  signal(SIGPIPE, sigpipeHandler);
  fwrite(&m, sizeof(PlayerMessage), 1, pipe);
  fflush(pipe);
  if (sigpipeReceived) {
    fprintf(stderr, "Server failure - sendMove!!!\n");
    exit(0);
  }
}

int receiveMove(FILE *pipe, PlayerMessage *m) {
  if (fread(m, sizeof(PlayerMessage), 1, pipe) != 1) {
    return 1;
  } else {
    return 0;
  }
}

int sendGameState(FILE *pipe, ServerMessage m) {
  sigpipeReceived = 0;
  signal(SIGPIPE, sigpipeHandler);
  fwrite(&m, sizeof(ServerMessage), 1, pipe);
  fflush(pipe);
  return sigpipeReceived;
}

void receiveGameState(FILE *pipe, ServerMessage *m) {
  if (fread(m, sizeof(ServerMessage), 1, pipe) != 1) {
    //fprintf(stderr, "Server failure - receiveGameState!!!\n");
    //exit(0);
  }
}


void sendInitPlayer(FILE *pipe, PlayerInitMessage m) {

  sigpipeReceived = 0;
  signal(SIGPIPE, sigpipeHandler);

  fwrite(&m, sizeof(PlayerInitMessage), 1, pipe);
  fflush(pipe);
  if (sigpipeReceived) {
    fprintf(stderr, "Server failure - sendMove!!!\n");
    exit(0);
  }
}

void receiveInitPlayer(FILE *pipe, PlayerInitMessage *m){
  if (fread(m, sizeof(PlayerInitMessage), 1, pipe) != 1) {
    //fprintf(stderr, "Server failure - receiveGameState!!!\n");
    //exit(0);
  }
}
