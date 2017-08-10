#!/usr/bin/env python

from jimmy_tutorials.srv import *
import rospy


def handle_sub_two_ints(req):
	print "Returning [%s - %s= %s]" %(req.a, req.b)
	rospy.sleep(1)
	return SubTwoIntsResponse()

def add_two_ints_server():
	rospy.init_node('haha')
	s2=rospy.Service('test_for_service',Test,handle_sub_two_ints)
	print "Ready to minus two ints."
	rospy.spin()

if __name__=="__main__":
	add_two_ints_server()