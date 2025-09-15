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

#define MAX_URL_LEN 2000

void host_error(char *str){
    fprintf(stderr, "curl: (6) Could not resolve host: %s", str);
    return; 
}

void malloc_error()
{
    perror("Unable to allocate memory.\n");
    return;
}


void parse_host(char *url_string, char *host, char *pos){
    const char *host_start = strstr(url_string, "://");
    if(host_start != NULL){
        if((host_start + 3) != NULL && (host_start + 3) != '\0') 
            host_start += 3;
        else
            host_error(url_string);
    }
    else{
        host_start = url_string;
    }

    

    int i = pos;
    
    while (url_string[i] != 


}

int main(int argc, char**argv){
        if(argc != 2 || (sizeof(argv[1]) > MAX_URL_LEN)){
        host_error(argv[1]);    
    }

    char *url_string = NULL;

    //parse web address
    if(strncmp(argv[1], "http://www.", 11)){
        char *host = malloc(MAX_URL_LEN * (sizeof(char)));
        if(!host){
            malloc_error();
            host = NULL;
            exit(1);
        }
        char *path = malloc(MAX_URL_LEN * (sizeof(char)));
        if(!path){
            malloc_error();
            free(host);
            host = NULL;
            exit(1);
        }

        int pos = 0;

        parse_host(argv[1], host, &pos);

        parse_path(argv[1], path, &pos); 
    }
    else{
        host_error(argv[1]);    
    }




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