#!/usr/bin/expect
set timeout -2
spawn rosnode cleanup
expect "Please type y or n to continue:"
send "y\r"
expect "*$"
expect eof
