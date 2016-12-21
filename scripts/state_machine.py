#!/usr/bin/env python
import rospy
import math
import tf
from geometry_msgs.msg import PoseStamped, Pose
from nav_msgs.msg import Path
from std_msgs.msg import String,Header
from keyboard.msg import Key
from emaxx_ctrl_msgs.msg import emaxx_status

LOITER_STATE=0
TELEOP_STATE=1
AUTO_STATE=2
HOME_STATE=3

TELEOPERATION=0
WAYPOINT=1
PATH=2
VELOCITY=3
TRAJECTORY=4

curr_status = emaxx_status()

def setAuto():
    rospy.loginfo("Setting Auto")
    curr_status.nav_state = AUTO_STATE
    set_stat_pub.publish(curr_status)

def setHome():
    rospy.loginfo("Setting Home")
    curr_status.nav_state = HOME_STATE
    set_stat_pub.publish(curr_status)

def setLoiter():
    rospy.loginfo("Setting Loiter")
    curr_status.armed = True
    curr_status.nav_state = LOITER_STATE
    set_stat_pub.publish(curr_status)

def setArm():
    rospy.loginfo("Setting Arm")
    curr_status.armed = True
    set_stat_pub.publish(curr_status)

def setDisarm():
    rospy.loginfo("Setting Disarm")
    curr_status.armed = False
    set_stat_pub.publish(curr_status)

def setRun():
    rospy.loginfo("Setting Run")
    curr_status.armed = True
    curr_status.nav_state = AUTO_STATE
    set_stat_pub.publish(curr_status)

def keyBoardCallback(data):

    c = chr(data.code)		  

    if c == 'a':
        setAuto()

    if c == 'h':
        setHome()

    if c == 'l':
        setLoiter()

    if c == 'x':
        setArm()

    if c == 'd':
        setDisarm()

    if c == 'r':
        setRun()
   

if __name__ == '__main__':

    rospy.init_node('Emaxx_State_Machine', anonymous=True)

    rospy.Subscriber('/keyboard/keydown',Key,keyBoardCallback)
    set_stat_pub = rospy.Publisher('/emaxx/set_status',emaxx_status,queue_size=10,latch=True)
    rate = rospy.Rate(10.0)
    rospy.spin()
    #while not rospy.is_shutdown():
 	
