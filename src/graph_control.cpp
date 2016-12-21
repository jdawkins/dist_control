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

std::string ugv_name;

void orderAgentIndex(std::vector<int> ids,std::vector<int> *ordered_ids){
  int n = ids.size();
   std::vector <int> spots;
   for (int i=0;i<n;i++){    
     int spot = 0;
        for (int j=0;j<n;j++){
	  if(ids[i] < ids[j]){
	    spot++;
	  }	       
      }
      ordered_ids->push_back((n-1)-spot);
    }
}

void configVectorCallBack(emaxx_ctrl_msgs::team_state msg){

// iterates over all data points in the array. Assigns to desired configuration array in x and y
  int count = 0;
  for (int i=0;i<=msg.length;i++){
    if (msg.poses[i].position.x==pose.pose.position.x){ // if me
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
    if (msg.poses[i].position.x== pose.pose.position.x){ // if me
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
  
  // initialize variables
  double Kp = 0.1;
  double velsx = 0.0;
  double velsy = 0.0;

  // query configuration quantities and neighbor positions and calculate comms part of control
  for (int i=0;i<agent_poses.poses.size();i++){
    for (int j=0; j<config_vector.poses.size();j++){
      if (agent_poses.id[i]==config_vector.id[j]){
	velsx  = velsx - Kp*( (pose.pose.position.x - config_vector.poses[j].position.x/myDesired.poses[0].position.x*agent_poses.poses[i].position.x));
	velsy  = velsy - Kp*( (pose.pose.position.y - config_vector.poses[j].position.y/myDesired.poses[0].position.y*agent_poses.poses[i].position.y));
      }
    }
  }
 if (myDesired.leader.front()){
  // append controller with absolute control to position if configuration vector is known NOTE: need to add if statement and some handling of "knowledge"
    velsx = velsx - Kp*(pose.pose.position.x - myDesired.poses[0].position.x);
    velsy = velsy - Kp*(pose.pose.position.y - myDesired.poses[0].position.y);
  }
  
  // commanded velocity variable
  geometry_msgs::Twist cmd_vel;
  // set linear velocities according to controller
  cmd_vel.linear.x = velsx;
  cmd_vel.linear.y = velsy;
  
  // publish control signal to mbed
  cmd_pub.publish(cmd_vel);
}


int main(int argc, char **argv)
{
 
/*  std::vector<int> test;
  test.push_back(1);
  test.push_back(3);
  test.push_back(9);
  test.push_back(7);
  test.push_back(5);
  std::vector<int> out;
  orderAgentIndex(test,&out);
  
        for (int j=0;j<out.size();j++){
      
	  std::cout << out[j] << std::endl; 
      }
  */

 ros::init(argc, argv, "talker");
  

  ros::NodeHandle nh;
  nh.param("ugv_name",ugv_name,std::string("emaxx"));

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
