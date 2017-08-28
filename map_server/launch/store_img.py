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
    
    rospy.init_node('convertimg', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    # rospy.sleep(3)
    convert()
    rospy.loginfo("finished!")
    return 0

def convert():
    arg="map_test"
    #need two arg. first is the path(default is /var/www/maps), the second is the unique id of the map. The third is the name of the map.
    if len(sys.argv)>3:
        folder_path=sys.argv[1]#id
        map_id=sys.argv[2]
        map_name=sys.argv[3]
        arg=folder_path+'/'+map_id+'/'+map_id
        rospy.loginfo(arg+".pgm")

    #save pgm to jpg,jpeg.
    im=Image.open(arg+".pgm")
    im.save(arg+".jpg")
    im.save(arg+".jpeg")
    im.save(arg+"_bk.pgm")

    
    #read yaml
    fr_yaml=open(arg+'.yaml','r')
    x=yaml.load(fr_yaml)
    data=x

    fr_yaml.close()

    #rewrite yaml path
    fw_yaml=open(arg+'.pgm','w')
    path=arg+'.yaml'
    rospy.loginfo(path)
    data['image']=path
    yaml.dump(data,fw_yaml)
    fw_yaml.close()


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
    'name': map_name,
    'id': map_id,
    'path': folder_path,   
    'pgmFileName' : arg+'.pgm',
    'jpgFileName' : arg+'.jpg',
    'jpegFileName' : arg+'.jpeg',
    'yamlFileName' : arg+'.yaml',
    }
    with open(arg+'.json','w') as fw:
        fw.write(json.dumps(map_json,sort_keys=True,indent=4, separators=(',', ': ')))


if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass

