#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define NET_BUF_SIZE 512
#define sendrecvflag 0
#define nofile "File Not Found!"

#define FILENAME 1
#define FILEDATA 2
#define ENDOFFILE 3
#define EXMESSAGE 4
#define ERRCHRONOLOGY 5
#define SUCCESS 6
#define NONE 7

typedef struct __attribute__((__packed__)) FileTCPPacket {
  uint8_t type;
  char data[NET_BUF_SIZE - 1];
} FileTCPPacket;
