#include "client_utils.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        // should I create a basic help and also manual???
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

    char *port = malloc(STR_MAX * (sizeof(char)));
    if (!port)
    {
        malloc_error();
        free(host);
        host = NULL;
        free(path);
        path = NULL;
        exit(1);
    }

    // parse web address
    parse_url(argv[1], host, path, &p_type);

    //printf("host: %s\npath: %s\np_type: %d\n", host, path, p_type);

    // build request
    char *get_req = malloc(STR_MAX * sizeof(char));
    if (!get_req)
    {
        malloc_error();
        exit(1);
    }

    if (p_type == HTTP)
    {
        build_http_request(host, path, port, get_req);
    }

    int sockfd = tcp_connect(host, port);

    //allocating values for receiving and processing response
    size_t bufsize = INITIAL_BUF;
    size_t used = 0;
    char *response = malloc(bufsize * sizeof(char));
    if (!response)
    {
        malloc_error();
        close(sockfd);
        exit(1);
    }

    ssize_t n;

    // sending request packet
    int get_req_len = strlen(get_req);
    if (send(sockfd, get_req, get_req_len, 0) == -1)
    {
        close(sockfd);
        perror("send");
        exit(3);
    }

    // receiving response packet(s)
    while ((n = recv(sockfd, response + used, bufsize - used, 0)) > 0)
    {
        used += n;

        if (used == bufsize)
        {
            expand_buf(response, bufsize, sockfd);
        }
    }

    if (n == -1)
        perror("recv error");

    close(sockfd);

    response[used] = '\0';

    // Print response (debug)
    // printf("=== Full Response ===\n%s\n", response);
    
    char * body = process_body(response);

        //printf("\n=== Body ===\n%s\n", body);
        printf("%s\n", body);

    //free heap data
    free(body);
    free(response);
    free(host);
    free(path);
    free(port);

    body = response = host = path = port = NULL;

    return 0;
}