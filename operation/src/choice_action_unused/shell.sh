#!/bin/bash


/usr/bin/expect <<\EOF

spawn ssh relaybot@192.168.31.47
expect -- "yes/no"
send "yes\r"
#interact 
expect eof
#EOF
