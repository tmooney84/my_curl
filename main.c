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
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_URL_LEN 2000
#define MAX_STR_LEN 1024
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

    // parse web address
    parse_url(argv[1], host, path, &p_type);

    printf("host: %s\npath: %s\np_type: %d\n", host, path, p_type);

    //!!!need to account for if www.google.com >>> is the DNS figuring out this is https???
    //!!!may need to call a subfunction to identify the protocol type
    //!!!need to somewhere handle non-existant web addresses

    //!!!likely call subfunction for build HTTP Header
    if (p_type == HTTP)
    {
        // build request
        char *get_req = malloc(MAX_STR_LEN * sizeof(char));
        if (!get_req)
        {
            malloc_error();
        }

        // -2 capatures the '/' that has been disgarded and '\0'
        snprintf(get_req, MAX_URL_LEN, "GET %s HTTP/1.1\r\n\
        Host: %s\r\n\
        User-Agent: my_curl/1.0 \r\n\
        */*\r\n\
        \r\n",
                 path, host);

        printf("get request header: \n%s\n", get_req);
    }

    

   

    // free request, host, path

    return 0;
}

/*
-create struct to snprintf to create a formatted string

-open up a connection using the formatted string as URI

-then listen for returning packets
    -> entering into packet struct? in order to get the partial string parsed

-store partial string chunks in full string until all packets are received

-close connection

-display entire string
*/