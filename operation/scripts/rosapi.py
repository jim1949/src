#!/usr/bin/env python

from rosapi.srv import *
import rospy


def handle_sub_two_ints(req):
	print "Receive the request"
	rospy.sleep(1)
	return SubTwoIntsResponse("hahahahsdfaadsfds")

def add_two_ints_server():
	rospy.init_node('add')
	s2=rospy.Service('nodes',SubTwoInts,handle_sub_two_ints)
	print "Ready to minus two ints."
	rospy.spin()




if __name__=="__main__":
	add_two_ints_server()
