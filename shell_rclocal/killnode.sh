#!/usr/bin/expect
set timeout -2
spawn rosnode cleanup
expect "(yes/no?)"
send "yes\r"
expect "*$"
expect eof
