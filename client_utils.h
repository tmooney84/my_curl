#ifndef CLIENT_UTILS_H 
#define CLIENT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_URL_LEN 2000
#define STR_MAX 1024
#define INITIAL_BUF 8192
#define FETCH_STR_LEN 64

enum Protocol
{
    NO_PROTO = 0,
    HTTP = 1,
    HTTPS = 2,
    FTP = 3
};

void malloc_error();
void host_error(char *str);
void parse_url(char *url_string, char *host, char *path, enum Protocol *p_type);
void *get_in_addr(struct sockaddr *sa);
int tcp_connect(char *host, char *port);
void build_http_request(char *host, char *path, char *port, char* get_req);
char *process_body(char *response);
void expand_buf(char *response, size_t bufsize, int sockfd);

#endif