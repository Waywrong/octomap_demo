#include<ros/ros.h>  
#include<pcl/point_cloud.h>  
#include<pcl_conversions/pcl_conversions.h>  
#include<sensor_msgs/PointCloud2.h>  
#include<pcl/io/pcd_io.h>

int main (int argc, char **argv)  
{  
  ros::init (argc, argv, "pub_pcl");  
  ros::NodeHandle nh;  
  ros::Publisher pcl_pub = 
      nh.advertise<sensor_msgs::PointCloud2> ("/camera/depth/points", 10);  

  pcl::PointCloud<pcl::PointXYZ> cloud;  
  sensor_msgs::PointCloud2 output;  
  pcl::io::loadPCDFile("/home/peter/code/ira_slam/src/octomap_test/data/office/office1.pcd", cloud);  
  pcl::toROSMsg(cloud,output);

  output.header.stamp=ros::Time::now();
  output.header.frame_id  ="camera_rgb_frame";

  ros::Rate loop_rate(1);  
  while (ros::ok())  
  {  
    pcl_pub.publish(output);  
    ros::spinOnce();  
    loop_rate.sleep();  
  }  
  return 0;  
}  
