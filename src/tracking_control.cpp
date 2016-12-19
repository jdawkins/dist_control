#include "ros/ros.h"

#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/MultiDOFJointState.h"
#include "trajectory_msgs/MultiDOFJointTrajectory.h"
#define PI (3.14159)

sensor_msgs::MultiDOFJointState agent_poses;
trajectory_msgs::MultiDOFJointTrajectory config_vector;
geometry_msgs::Twist vel_tgt;
ros::Time start_time;
ros::Publisher cmd_pub;
geometry_msgs::PoseStamped pose;

void configVectorCallBack(trajectory_msgs::MultiDOFJointTrajectory msg){
  config_vector = msg;
  
}
void teamStateCallBack(sensor_msgs::MultiDOFJointState msg){
  agent_poses = msg;  
}
void poseCallBack(geometry_msgs::PoseStamped msg){
  pose = msg;  
}

void runController(){

  double t =ros::Time::now().toSec() - start_time.toSec();
  

  double des_n = 10*cos(2*PI*t/30);
  double des_e = 10*sin(2*PI*t/30);

  geometry_msgs::Twist cmd_vel;
  
  cmd_vel.linear.x = des_n - pose.pose.position.x;
  cmd_vel.linear.y = des_e - pose.pose.position.y;
  
 cmd_pub.publish(cmd_vel); 
  
 // ros::Duration d(0.1);
//  secs = d.toSec();
  
  
   

  for (int i=0;i<agent_poses.transforms.size();i++){
  //velsx - gain*( (position(1) - configuration_vector(1,1)/configuration_vector(gg+1,1)*neighbor_positions(gg,1)) + (position(1) - configuration_vector(1,1)));
  //  vel_tgt.linear.x = 
    
  }
  
 // secs = secs+ secs;
}

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "talker");


  ros::NodeHandle nh;


  ros::Subscriber team_state_sub = nh.subscribe("/team_state",100,teamStateCallBack);
  ros::Subscriber pose_sub = nh.subscribe("/emaxx/pose_ned",100,poseCallBack); 

  cmd_pub = nh.advertise<geometry_msgs::Twist>("/emaxx/des_vel",10);
  ros::Rate loop_rate(50);

  start_time = ros::Time::now();
  int count = 0;
  while (ros::ok())
  {

    runController();

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
