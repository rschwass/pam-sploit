#!/bin/bash

gcc -fPIC -fno-stack-protector -c src/mypam.c

#sudo ld -x --shared -o /lib/security/mypam.so mypam.o
#/usr/lib64/security
sudo ld -x --shared -o /usr/lib/x86_64-linux-gnu/security/mypam.so mypam.o
#rm mypam.o


#add the folowing to top of /etc/pam.d/sshd

##custom

#auth optional mypam.so
