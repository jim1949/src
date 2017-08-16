#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-07-12 11:43:18
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
#This is the tool which can convert the picture of .pgm file to jpg and jpeg file.
# @Version : １


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
import json
import yaml
import time

def listener():
    
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    x0=341.47684
    y0=348.20615
    #x
    grid_length=800.0
    #y
    grid_width=544.0
    android_length=3200
    android_width=2176
    resolution=0.05

    factor=grid_width/android_width
    factor=1
    print factor
    init_y=-grid_width*resolution+y0*factor*resolution
    init_x=-x0*factor*resolution
    print "init_x:",init_x, "init_y: ", init_y
    # with open(arg+'map.json','w') as fw:
    #     fw.write(json.dumps(map_json,sort_keys=True,indent=4, separators=(',', ': ')))

    # sub = rospy.Subscriber("map_status", Int32, convert)
    rospy.spin()

def convert(data):
    arg="map_test"
    if len(sys.argv)>1:
        arg=sys.argv[1]
    print(arg)
    im=Image.open(arg+".pgm")
    print("test") 
    im.save(arg+".jpg")
    im.save(arg+".jpeg")

    fr_yaml=open(arg+'.yaml','r')
    x=yaml.load(fr_yaml)

    rospy.loginfo(x['origin'][1])

    pic_size=im.size
    x_size=pic_size[0]
    y_size=pic_size[1]
    measurements={'x_size': x_size,  
                                    'y_size': y_size }
    map_json={
    'createdAt' : time.strftime("%Y-%m-%d %X", time.localtime()),
    'mapInfo': {
        'gridHeight': x_size,
        'gridWidth': y_size,
        'originX': x['origin'][1],
        'originY': x['origin'][2],
        'resolution': x['resolution']},
    'obstacleFileName' : "unknown.json",
    'name': arg,
   
    'pgmFileName' : arg+'.pgm',
    'pngFileName' : arg+'.png',
    'yamlFileName' : arg+'.yaml',
    }
    with open(arg+'map.json','w') as fw:
        fw.write(json.dumps(map_json,sort_keys=True,indent=4, separators=(',', ': ')))


if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass

