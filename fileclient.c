#include "util.h"

int main(int argc, char *argv[]) {
  // const int NET_BUF_SIZE = 256;

  if (argc < 3) {
    printf("Usage %s hostname port\n", argv[0]);
    exit(0);
  }

  int port = atoi(argv[2]);
  struct hostent *server = gethostbyname(argv[1]); // DNS kinda utility

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
  }

  // connect
  struct sockaddr_in server_add;
  bzero(&server_add, sizeof(server_add));
  bcopy(server->h_addr_list[0], &server_add.sin_addr.s_addr, server->h_length);
  server_add.sin_port = htons(port); // copy port
  server_add.sin_family = AF_INET;

  if (connect(sockfd, (struct sockaddr *)&server_add, sizeof(server_add)) < 0) {
    perror("Error connecting");
  }
  /*
  - send file name
  - send file data
  - send eof
  - receive executable message
  */
  //   char buffer[NET_BUF_SIZE] = {0};
  char net_buf[NET_BUF_SIZE];
  printf("\nPlease enter file name to send:\n");
  scanf("%s", net_buf);
  FileTCPPacket *send = calloc(1, sizeof(FileTCPPacket));
  FileTCPPacket *rec = calloc(1, sizeof(FileTCPPacket));
  send->type = FILENAME;
  strcpy(send->data, net_buf);
  if (write(sockfd, (void *)send, NET_BUF_SIZE) < 0) {
    perror("Error writing to socket");
  }
  int n = 0;
  if ((n = read(sockfd, (void *)rec, NET_BUF_SIZE)) < 0) {
    perror("reading the socket");
  }
  // printf("%s", net_buf);
  printf("%d\n", rec->type);
  printf("%s\n", rec->data);
  if (rec->type != SUCCESS) {
    perror("client server sync 1");
  } else {
    // start reading and sending file
    FILE *fp;
    fp = fopen(net_buf, "r+");
    printf("fileopened");
    if (fp == NULL) {
      perror("Failed: ");
      return 1;
    }
    char buffer[NET_BUF_SIZE];
    // -1 to allow room for NULL terminator for really long string
    printf("reading start");
    while (fgets(buffer, NET_BUF_SIZE - 1, fp)) {
      // Remove trailing newline
      // buffer[strcspn(buffer, "\n")] = 0;
      printf("%s", buffer);
      send->type = FILEDATA;
      strcpy(send->data, buffer);
      if (write(sockfd, (void *)send, NET_BUF_SIZE) < 0) {
        perror("Error writing to socket");
      }
      if ((n = read(sockfd, (void *)rec, NET_BUF_SIZE)) < 0) {
        perror("reading the socket");
      }
      printf("%d", rec->type);
      if (rec->type != SUCCESS) {
        perror("client server sync 2");
      }
    }
    send->type = ENDOFFILE;
    strcpy(send->data, "\0");
    if (write(sockfd, (void *)send, NET_BUF_SIZE) < 0) {
      perror("Error writing to socket");
    }
    if ((n = read(sockfd, (void *)rec, NET_BUF_SIZE)) < 0) {
      perror("reading the socket");
    }
    // if (rec->type == EXMESSAGE) {
    // printf("Compiled and executed output is : \n");
    // printf("%s", rec->data);
    if ((n = read(sockfd, (void *)rec, NET_BUF_SIZE)) < 0) {
      perror("reading the socket");
    }
    if (rec->type == EXMESSAGE) {
      printf("\nCompiled and executed output is : \n");
      printf("%s", rec->data);
    } else {
      perror("server failed to compile");
    }
    free(rec);
    free(send);
    fclose(fp);
  }
}
