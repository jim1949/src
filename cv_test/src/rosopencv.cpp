#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat img = imread("/home/jimmy/api_ws/src/cv_test/src/lena.jpg");  
    Point p1(4, 4);
    Point p2(100, 50);
    line(img, p1, p2, Scalar(33, 33, 133), 2);

    //画第二条线
    line(img, Point(30, 30), Point(75, 300), Scalar(89, 90, 90), 3);

    imshow("画矩形", img);

    waitKey();
//     // waitKey(0);
//   if(image.empty()){
//    printf("open error\n");
//    }
//   sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

//   ros::Rate loop_rate(5);
//   while (nh.ok()) {
//     pub.publish(msg);
//     ros::spinOnce();
//     loop_rate.sleep();
//   }
return 0;
}