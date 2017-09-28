#!/usr/bin/expect
spawn ssh relaybot@192.168.1.3
expect "(yes/no)?"
send "yes\r"

expect "password:"
send "cslg\r"
expect "*$"
interact
#spawn roslaunch rosbridge_server rosbridge_websocket.launch
#	set spid_roswebsocket $spawn_id
#	expect " Rosbridge WebSocket server started on port 9090"
#	send "rostopic list"
#	exp_continue
#	send "rostopic list"
#	set spid_rostopic $spawn_id
#spawn rostopic list

#expect "password:"
#send "cslg\r"
#expect "*$"
#interact

#spawn ftp connect
#    set spid_ftp $spawn_id
#    spawn telnet connect
#    set spid_telnet $spawn_id
#    exp_send –i $spid_ftp “ftp command”
#    exp_send –i $spid_telnet “telnet command”
