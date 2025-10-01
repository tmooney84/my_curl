/*
Description
HTTP Protocol is used everywhere and pretty easy to read, this project will lead you to its world.

You will create a my_curl command (very similar to the UNIX command curl) my_curl is a tool to get data from a server, using HTTP. The command is designed to work without user interaction.

How does it work? You send an url as parameter and it will print the html content of a web page.

my_curl only supports HTTP.

$>./my_curl http://www.columbia.edu/~fdc/sample.html
<!DOCTYPE HTML>
<html lang="en">
<head>
...
<p>
<i>(End)</i>

<hr>
</body>  <!-- close the <body> begun above -->
</html>  <!-- close the <html> begun above -->


How would you be able to do this magic? Using sockets. They are file descriptors.
Authorized functions
bind
connect
socket
write / read
close
select (optional)

*/

/*
MY_CURL OUTLINE

$>./my_curl http://www.columbia.edu/~fdc/sample.html

*/
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

// typedef struct http_request_header
// {
//     char fetch_req[MAX_URL_LEN];
//     char host[MAX_URL_LEN];
//     char user_agent[FETCH_STR_LEN];
//     char accept[FETCH_STR_LEN];
//     // char content_type[FETCH_STR_LEN];
//     // char content_length[FETCH_STR_LEN];
// } http_request_header;

typedef struct http_response_header
{
    char fetch_resp[MAX_URL_LEN];
    char server[MAX_URL_LEN];
    char date[FETCH_STR_LEN];
    char content_length[FETCH_STR_LEN];
    char content_type[FETCH_STR_LEN];
    char cache_control[FETCH_STR_LEN];
} http_response_header;

enum Protocol
{
    NO_PROTO = 0,
    HTTP = 1,
    HTTPS = 2,
    FTP = 3
};

void host_error(char *str)
{
    fprintf(stderr, "my_curl: (6) Could not resolve host: %s\n", str);
    exit(1);
}

void malloc_error()
{
    perror("Unable to allocate memory.\n");
}

void parse_url(char *url_string, char *host, char *path, enum Protocol *p_type)
{
    if (strncmp("http", url_string, 4) == 0 && url_string[4] != 's')
    {
        *p_type = HTTP;
    }
    else if (strncmp("https", url_string, 5) == 0)
    {
        *p_type = HTTPS;
    }

    char *host_start = strstr(url_string, "://");
    char *path_start = NULL;
    if (host_start != NULL && *(host_start + 3) != ' ' && *(host_start + 3) != '\0')
    {
        host_start += 3;
    }
    else
        host_error(url_string);

    int i = 0;
    int fs_found = 0;

    int hs_len = strlen(host_start);

    while (i < hs_len)
    {
        if (host_start[i] == '/')
        {
            fs_found = 1;
            break;
        }

        i++;
    }

    if (fs_found && hs_len > i + 1)
    {
        path_start = host_start + i;
        snprintf(path, strlen(path_start) + 1, "%s", path_start);
        strncpy(host, host_start, i);
        host[i] = '\0'; // writes over '/' mark
    }
    else if (fs_found)
    {
        strncpy(host, host_start, i);
        // host[i - 1] = '\0';             //writes over '/' mark
        path[0] = '/';
        path[1] = '\0';
    }
    else
    {
        strncpy(host, host_start, i);
        path[0] = '/';
        path[1] = '\0';
    }

    host_start = NULL;
    path_start = NULL;

    return;
}

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        // should I create a basic help and also manual!!! ???
        fprintf(stderr, "my_curl: try 'my_curl --help' or 'my_curl --manual' for more information\n");
        exit(1);
    }
    if (argc != 2 || (sizeof(argv[1]) > MAX_URL_LEN))
    {
        host_error(argv[1]);
    }

    enum Protocol p_type = NO_PROTO;

    char *host = malloc(MAX_URL_LEN * (sizeof(char)));
    if (!host)
    {
        malloc_error();
        host = NULL;
        exit(1);
    }
    char *path = malloc(MAX_URL_LEN * (sizeof(char)));
    if (!path)
    {
        malloc_error();
        free(host);
        host = NULL;
        exit(1);
    }

    char *port = malloc(STR_MAX *(sizeof(char)));
    if(!port){
        malloc_error();
        free(host);
        host = NULL;
        free(path);
        path = NULL;
        exit(1);
    }

    // parse web address
    parse_url(argv[1], host, path, &p_type);

    printf("host: %s\npath: %s\np_type: %d\n", host, path, p_type);

    //!!!need to account for if www.google.com >>> is the DNS figuring out this is https???
    //!!!may need to call a subfunction to identify the protocol type
    //!!!need to somewhere handle non-existant web addresses

    //!!!likely call subfunction for build HTTP Header
    
    // build request
        char *get_req = malloc(STR_MAX * sizeof(char));
        if (!get_req)
        {
            malloc_error();
            exit(1);
        }
    
    if (p_type == HTTP)
    {
        strcpy(port, "80");
        port[2] = '\0';
        //build http_request_header()

        snprintf(get_req, MAX_URL_LEN,
                 "GET %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "User-Agent: my_curl/1.0\r\n"
                 "Accept: */*\r\n"
                 "Connection: close\r\n"
                 "\r\n",
                 path, host);

        printf("get request header: \n%s\n", get_req);
    }

    //int sockfd = tcp_connect(host, "443");
    


    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    //char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(host, port, &hints, &servinfo)!= 0)){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next){
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("client: socket");
            continue;
        }
        
        if(connect(sockfd, p->ai_addr, p->ai_addrlen)== -1){
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL){
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    //for testing connection...
    //inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    //printf("client: connecting to %s]n", s);

    freeaddrinfo(servinfo);
    
    //sending request packet
    int get_req_len = strlen(get_req);
    if(send(sockfd, get_req, get_req_len, 0)== -1){
        close(sockfd);
        perror("send");
        exit(3);
    }

    size_t bufsize = INITIAL_BUF; 
    size_t used = 0; 
    char *response = malloc(bufsize * sizeof(char));
    if(!response){
        malloc_error();
        close(sockfd);
        exit(1);
    }

    ssize_t n;

    //receiving response packet(s)
    while((n = recv(sockfd, response + used, bufsize - used, 0)) > 0){
        used += n;

        if(used == bufsize){
            bufsize *= 2;
            char *tmp = realloc(response, bufsize);
            if(!tmp){
                perror("realloc error");
                free(response);
                close(sockfd);
                exit(2);
            }
            response = tmp;
            tmp = NULL;
        }

    }
        if(n == -1) perror("recv error");

        close(sockfd);

        response[used] = '\0';


    // Print response (debug)
    //printf("=== Full Response ===\n%s\n", response);


    char *body = strstr(response, "\r\n\r\n");
    if(body){
        *body = '\0';
        body += 4;
        //printf("\n=== Headers ===\n%s\n", response);
        printf("\n=== Body ===\n%s\n", body);
    }

    free(response);

// -then listen for returning packets
//     -> entering into packet struct? in order to get the partial string parsed

// -store partial string chunks in full string until all packets are received

// -close connection

// -display entire string





    


    // free request, host, path

    return 0;
}