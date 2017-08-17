#!/usr/bin/env python

# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

# input: nav and wall positions (vector) service called from android.
# functions:
# 1.get the positions, and store. give response.
# 2.transform the nav positions to map frame, and store in json file or SQL file.
# 3.draw wall_positions

# nav_pose_set.srv:

from tf import *
from basic_msgs.srv import *
import rospy

def handle_nav_pose(req):
    rospy.loginfo("%s",req.nav_pose.name)
    # x= "{'data':1,'errormsg':'errormsg','successed':0}"
    x=nav_pose_setResponse()
    x.errormsg="None"
    # print x
    return x


def handle_wall_pose(req):
    rospy.loginfo("%s",req.wall_pose.name)

    # return wall_pose_setResponse()


def start():
    rospy.init_node('nav_and_wall_positions_node')
    nav_pose_server = rospy.Service('/nav_pose_set', nav_pose_set, handle_nav_pose)
    wall_pose_server = rospy.Service('/wall_pose_set', wall_pose_set,handle_wall_pose)
    print "Ready to get nav and wall pose set."
    rospy.spin()

if __name__ == "__main__":
    start()