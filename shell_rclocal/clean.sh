#!/usr/bin/expect
set timeout -30
spawn rosnode cleanup
expect "(yes/no?)"
send "yes\r"
expect "*$"
expect eof
