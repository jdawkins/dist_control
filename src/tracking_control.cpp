#include "ros/ros.h"

#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Transform.h"
#include "emaxx_ctrl_msgs/team_state.h"
#include "sensor_msgs/MultiDOFJointState.h"
#include "trajectory_msgs/MultiDOFJointTrajectory.h"

#define PI (3.14159)



emaxx_ctrl_msgs::team_state agent_poses, config_vector, myDesired;
geometry_msgs::Twist vel_tgt;
ros::Time start_time;
ros::Publisher cmd_pub;
geometry_msgs::PoseStamped pose;

void configVectorCallBack(emaxx_ctrl_msgs::team_state msg){

// iterates over all data points in the array. Assigns to desired configuration array in x and y
  int count = 0;
  for (int i=0;i<=msg.length;i++){
    if (msg.poses[i].position.x == pose.pose.position.x){ // if me
      myDesired.poses.front() = msg.poses[i];
      myDesired.id.front() = msg.id[i];
      myDesired.leader.front() = msg.leader[i];
    }
    else{
      config_vector.poses[count] = msg.poses[i]; // if a neighbor (not me)
      config_vector.id[count] = msg.id[i];
      count++;
    }
  }  
}

void teamStateCallBack(emaxx_ctrl_msgs::team_state msg){
  
  // iterates over all data points in the array. Assigns to desired configuration array in x and y
  int count = 0;
  for (int i=0;i<=msg.length;i++){
    if (msg.poses[i].position.x == pose.pose.position.x){ // if me
    }
    else{
      agent_poses.poses[count] = msg.poses[i]; // if a neighbor (not me)
      agent_poses.id[count] = msg.id[i];
      count++;
    }
  }
}

void poseCallBack(geometry_msgs::PoseStamped msg){
  pose = msg;  
}


void runController(){

  double t =ros::Time::now().toSec() - start_time.toSec();
  

  double des_n = 5*cos(2*PI*t/30);
  double des_e = 5*sin(2*PI*t/30);

  geometry_msgs::Twist cmd_vel;
  
  cmd_vel.linear.x = des_n - pose.pose.position.x - PI/3*sin(2*PI*t/30);
  cmd_vel.linear.y = des_e - pose.pose.position.y + PI/3*cos(2*PI*t/30);
  
 cmd_pub.publish(cmd_vel); 

}

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "talker");


  ros::NodeHandle nh;


  ros::Subscriber team_state_sub = nh.subscribe("/team_state",100,teamStateCallBack);
  ros::Subscriber pose_sub = nh.subscribe("pose_ned",100,poseCallBack); 

  cmd_pub = nh.advertise<geometry_msgs::Twist>("des_vel",10);
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
