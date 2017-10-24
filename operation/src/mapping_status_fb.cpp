#include "ros/ros.h"
#include "std_msgs/Int32.h"

#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "talker");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::Int32>("/mapping_status_feedback", 1,true);

  int count = 0;

  ros::Rate r(1);
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::Int32 msg;
    msg.data=atoi(argv[1]);

    ROS_INFO("mapping_status_feedback: %d", msg.data);

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    
    chatter_pub.publish(msg);
    // ROS_INFO("publish fb finished.");
    ros::spinOnce();
    // r.sleep();
    // return 0;
    

    
}
}