#include"ros/ros.h"
#include"ros_cw4/getTransform.h"
#include"tf/transform_listener.h"
#include<iostream>



bool srvCallback(ros_cw4::getTransform::Request &req, ros_cw4::getTransform::Response &res){
    std::cout<<"Richiesta ricevuta: primo frame "<<req.frame_a<<", secondo frame "<<req.frame_b<<std::endl;
    tf::StampedTransform transform;
    tf::TransformListener listener;
    try{
        listener.waitForTransform(req.frame_a, req.frame_b, ros::Time(0), ros::Duration(1.0));
        listener.lookupTransform(req.frame_b, req.frame_a, ros::Time(0), transform);
    } catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
        return false;
    }
    res.pose.position.x=transform.getOrigin().x();
    res.pose.position.y=transform.getOrigin().y();
    res.pose.position.z=transform.getOrigin().z();
    res.pose.orientation.x=transform.getRotation().x();
    res.pose.orientation.y=transform.getRotation().y();
    res.pose.orientation.z=transform.getRotation().z();
    res.pose.orientation.w=transform.getRotation().w();
    ROS_INFO_STREAM(" Pos: " << 
        "x: "<<transform.getOrigin().x() << ", " << 
        "y: "<<transform.getOrigin().y() << ", " <<
        "z: "<<transform.getOrigin().z() << ". Or: " << 
        "x: "<<transform.getRotation().x() << ", " << 
        "y: "<<transform.getRotation().y() << ", " << 
        "z: "<<transform.getRotation().z() << ", " <<
        "w: "<<transform.getRotation().w()
    );
    
    return true;
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "my_listener_server");
    ros::NodeHandle nh;
    

    ros::ServiceServer server;
    server = nh.advertiseService("getTransform", srvCallback);
    ros::spin();

}


