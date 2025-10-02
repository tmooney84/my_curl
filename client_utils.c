#include "client_utils.h"

void malloc_error()
{
    perror("Unable to allocate memory.\n");
}

void host_error(char *str)
{
    fprintf(stderr, "my_curl: (6) Could not resolve host: %s\n", str);
    exit(1);
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

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void build_http_request(char *host, char *path, char *port, char *get_req)
{
    strcpy(port, "80");
    port[2] = '\0';
    // build http_request_header()

    snprintf(get_req, MAX_URL_LEN,
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: my_curl/1.0\r\n"
             "Accept: */*\r\n"
             "Connection: close\r\n"
             "\r\n",
             path, host);

    //printf("get request header: \n%s\n", get_req);
}

int tcp_connect(char *host, char *port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    // char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo) != 0))
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        perror("client: failed to connect\n");
        return -1;
    }

    // for testing connection...
    // inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    // printf("client: connecting to %s]n", s);

    freeaddrinfo(servinfo);

    return sockfd;
}

void expand_buf(char *response, size_t bufsize, int sockfd)
{
    bufsize *= 2;
    char *tmp = realloc(response, bufsize);
    if (!tmp)
    {
        perror("realloc error");
        free(response);
        close(sockfd);
        exit(2);
    }
    response = tmp;
    tmp = NULL;
}

char *process_body(char *response)
{
    char *tmp = strstr(response, "\r\n\r\n");
    if (tmp)
    {
        *tmp = '\0';
        tmp += 4;
        // printf("\n=== Headers ===\n%s\n", response);
    }
    size_t body_len = strlen(tmp);
    char *body = malloc((body_len + 1) * sizeof(char));
    if (!body)
    {
        malloc_error();
        exit(1);
    }

    strncpy(body, tmp, body_len);
    body[body_len] = '\0';
    tmp = NULL;

    // Print response (debug)
    // printf("=== Full Response ===\n%s\n", response);

    return body;
}
