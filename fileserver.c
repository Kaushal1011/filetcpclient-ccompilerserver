#include "util.h"

void delay(int number_of_seconds) {
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Storing start time
  clock_t start_time = clock();

  // looping till required time is not achieved
  while (clock() < start_time + milli_seconds)
    ;
}

int main(int argc, char *argv[]) {
  const int BACKLOG = 16;
  // const int NET_BUF_SIZE = 256;
  int port = 8888;
  if (argc >= 2) {
    port = atoi(argv[1]);
  }

  // create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("opening socket");
  }

  // bind socket
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("binding");
  }
  printf("Running server on localhost %d \n", port);

  // listen
  listen(sockfd, BACKLOG);

  // accept
  struct sockaddr_in client_addr;
  socklen_t clientlen = sizeof(client_addr);

  printf("in while");

  int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clientlen);
  if (newsockfd < 0) {
    perror("accept");
  }

  /*
  - receive file name create file
  - (looped)receive file data and put file data
  - receive end of file
  - compile and execute file
  - send message back
  */
  printf("in while");
  // char buffer[NET_BUF_SIZE] = {0};
  FileTCPPacket *send = calloc(1, sizeof(FileTCPPacket));
  FileTCPPacket *rec = calloc(1, sizeof(FileTCPPacket));
  FILE *fp;
  int n = 0;
  if ((n = read(newsockfd, (void *)rec, NET_BUF_SIZE)) < 0) {
    perror("reading the socket");
  }
  printf("%s", rec->data);
  printf("%d", rec->type);
  // delay(1000);
  if (rec->type != FILENAME) {
    send->type = ERRCHRONOLOGY;
    strcpy(send->data, "\0");
    if (write(newsockfd, (void *)send, NET_BUF_SIZE) < 0) {
      perror("Error writing to socket");
    }
    perror("server client chronology error");
    printf("%s", rec->data);
  } else {
    printf("%s", rec->data);
    fp = fopen("cfile.c", "w");
    send->type = SUCCESS;
    strcpy(send->data, "hello");
    if (write(newsockfd, (void *)send, NET_BUF_SIZE) < 0) {
      perror("Error writing to socket");
    }
  }
  rec->type = NONE;
  while (rec->type != ENDOFFILE) {

    if ((n = read(newsockfd, (void *)rec, NET_BUF_SIZE)) < 0) {
      perror("reading the socket");
    }
    // printf("%s", rec->data);
    if (rec->type == FILEDATA)
      fputs((char *)rec->data, fp);
    send->type = SUCCESS;
    strcpy(send->data, "\0");
    if (write(newsockfd, (void *)send, NET_BUF_SIZE) < 0) {
      perror("Error writing to socket");
    }
    if (rec->type == ENDOFFILE) {
      fclose(fp);
      continue;
    }
  }

  int a = system("gcc cfile.c -o compile.out > output1.txt");
  printf("%d\n", a);
  int b = system("./compile.out > output.txt");
  printf("%d\n", b);
  FILE *fp1;
  fp1 = fopen("output.txt", "r");
  char buffer1[NET_BUF_SIZE];
  fgets(buffer1, NET_BUF_SIZE - 1, fp1);

  fclose(fp1);
  send->type = EXMESSAGE;
  strcpy(send->data, buffer1);
  printf("%s", send->data);
  if (write(newsockfd, (void *)send, NET_BUF_SIZE) < 0) {
    perror("Error writing to socket");
  }
  // bzero(buffer, NET_BUF_SIZE);

  free(send);
  free(rec);
  close(newsockfd);
}
