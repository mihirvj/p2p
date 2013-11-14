#include<stdio.h>
#include<string.h>
#include<sys/utsname.h>
#include<time.h>
#include<malloc.h>

#define VERSION "P2P-CI/1.0"
#define SPACE " "
#define CRLF "\r\n"
#define ERROR "error"

#define GETRFC "GET RFC %d %s\r\nHost: %s\r\nPort: %s\r\nTitle: %s"
#define ADDRFC "ADD RFC %d %s\r\nHost: %s\r\nPort: %s\r\nTitle: %s"
#define LOOKUPRFC "LOOKUP RFC %d %s\r\nHost: %s\r\nPort: %s\r\nTitle: %s"
#define LISTALLRFC "LIST ALL %s\r\nHost: %s\r\nPort: %s"

#define SIZE 100

enum{
  OK = 200,
  BadRequest = 400,
  NotFound = 404,
  VersionNotFound = 505
};

enum{
  ADD = 0,
  GET = 1,
  LOOKUP = 2,
  LISTALL = 3
};

void generate_request(char req[1000], int req_type, int rfc, char host[50], char port[50], char title[50]);
int parse_request(char req[1000], char rfc[50], char host[50], char port[50], char title[100], int *method);
void generate_response(char buf[1000], int errcode, int method);
void append_response(char buf[1000], int rfc, char host[50], char port[50], char title[50]);
int parse_response(char buf[1000], int index, char rfc[50], char title[50], char host[50], char port[50]);
