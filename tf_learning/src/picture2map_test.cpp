
/**
# @Date    : 2017-08-10 09:24:31
# @Author  : Jian Jiao (jim1949@163.com)
# @Link    : http://github.com/jim1949
# @Version : 1

input: pose from the picture frame(frame 1), target frame(frame 2).
output:pose in the target frame.

*/
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <tf_learning/pose_tf.h>
using namespace std; 


int main(int argc,char **argv){
ros::init(argc, argv, "pose_transform_node");
ros::NodeHandle n;
// ros::ServiceServer nav_service=n.advertiseService("/nav_pose_set",nav_server);
geometry_msgs::Pose pose_in_frame1;


const string frame1("/picture_frame");
const string frame2("/map");
pose_in_frame1.position.x=0.0;
pose_in_frame1.position.y=0.0;
pose_in_frame1.position.z=0.0;
pose_in_frame1.orientation.x=0.0;
pose_in_frame1.orientation.y=0.0;
pose_in_frame1.orientation.z=0.0;
pose_in_frame1.orientation.w=1.0;
transform_point(pose_in_frame1,frame1,frame2);
transform_point(0.0,0.0,3.14,frame1,frame2);

return 0;
}