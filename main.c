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

-parse web address

-create struct to snprintf to create a formatted string

-open up a connection using the formatted string as URI

-then listen for returning packets
    -> entering into packet struct? in order to get the partial string parsed

-store partial string chunks in full string until all packets are received

-close connection

-display entire string
*/