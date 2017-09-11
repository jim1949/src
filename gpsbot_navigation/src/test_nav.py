
#!/usr/bin/env python  

import rospy  


from geometry_msgs.msg import Pose, PoseWithCovarianceStamped, Point, Quaternion, Twist  

from gpsbot_navigation.srv import execute_nav_task,nav_flag


def talker():

    rospy.wait_for_service('nav_flag')
    try:
        nav_flag = rospy.ServiceProxy('nav_flag', nav_flag)
        resp1 = nav_flag(x, y)
        return resp1.sum
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    pub = rospy.Publisher('/initialpose',PoseWithCovarianceStamped, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(1) # 10hz

    pose =PoseWithCovarianceStamped()
    pose.header.frame_id = "map"
    pose.pose.pose.position.x=100
    pose.pose.pose.position.y=102
    pose.pose.pose.position.z=0
    pose.pose.covariance=[0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.06853891945200942]
    pose.pose.pose.orientation.z=0.0267568523876
    pose.pose.pose.orientation.w=0.999641971333
    rospy.loginfo(pose)
    pub.publish(pose)
    rospy.spin()

if __name__ == '__main__':  

    try:  

        talker()  

        rospy.spin()  

    except rospy.ROSInterruptException:  

        rospy.loginfo("AMCL navigation test finished.")