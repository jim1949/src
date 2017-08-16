#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-07-12 11:43:18
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : $Id$


import rospy
import actionlib
from actionlib_msgs.msg import *
from geometry_msgs.msg import Pose, Point, Quaternion, Twist
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from tf.transformations import quaternion_from_euler
from visualization_msgs.msg import Marker
from math import radians, pi
from std_msgs.msg import Int32
from std_msgs.msg import String
from PIL import Image
import sys

def listener():
    
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    sub = rospy.Subscriber("map_status", Int32, convert)
    rospy.spin()

def convert(data):
    arg="map_test1"
    if len(sys.argv)>1:
        arg=sys.argv[1]
    print(arg)
    im=Image.open("/var/www/"+arg+".pgm")
    print("test") 
    im.save("/var/www/"+arg+".jpg")
    im.save("/var/www/"+arg+".jpeg")


if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
