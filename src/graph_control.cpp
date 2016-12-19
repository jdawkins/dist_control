#include "ros/ros.h"

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/MultiDOFJointState.h"
#include "trajectory_msgs/MultiDOFJointTrajectory.h"

sensor_msgs::MultiDOFJointState agent_poses;
trajectory_msgs::MultiDOFJointTrajectory config_vector;
geometry_msgs::Twist vel_tgt;


void configVectorCallBack(trajectory_msgs::MultiDOFJointTrajectory msg){
  config_vector = msg;
  
}
void teamStateCallBack(sensor_msgs::MultiDOFJointState msg){
  agent_poses = msg;  
}

void runController(){

  for (int i=0;i<agent_poses.transforms.size();i++){
  //velsx - gain*( (position(1) - configuration_vector(1,1)/configuration_vector(gg+1,1)*neighbor_positions(gg,1)) + (position(1) - configuration_vector(1,1)));
  //  vel_tgt.linear.x = 
    
  }
  
  
}

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "talker");


  ros::NodeHandle nh;


  ros::Subscriber team_state_sub = nh.subscribe("/team_state",100,teamStateCallBack); 
  ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/emaxx/des_vel",10);

  ros::Rate loop_rate(50);


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
