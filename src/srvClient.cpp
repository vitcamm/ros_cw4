#include"ros/ros.h"
#include"ros_cw4/getTransform.h"
#include <tf/transform_broadcaster.h>


int main(int argc, char**argv){

    ros::init(argc,argv, "my_broadcaster");

    ros::NodeHandle nh;
    ros::ServiceClient srvClient= nh.serviceClient<ros_cw4::getTransform>("getTransform");
    std::string frame_a;
    std::string frame_b;
    ros_cw4::getTransform srv;

    if (!nh.getParam("my_frame_a", frame_a)){ 
        ROS_ERROR("No frame_a param: init from src"); 
        frame_a="l1";
    } else {ROS_INFO("Frame a from param");}

    if (!nh.getParam("my_frame_b", frame_b)){ 
        ROS_ERROR("No frame_b param: init from src"); 
        frame_b="l7";
    }
    
    srv.request.frame_a=frame_a;
    srv.request.frame_b=frame_b;
        
    tf::TransformBroadcaster br;
    tf::Transform transform;
    //geometry_msgs::Pose receivedpose;
    ros::Rate r(100);
    
    while(ros::ok()){
        if(srvClient.call(srv)){
            ROS_INFO("risposta ricevuta, broadcasting tf ...");
            //receivedpose=srv.response;
            transform.setOrigin(tf::Vector3 (srv.response.pose.position.x,srv.response.pose.position.y,srv.response.pose.position.z));
            //tf::Quaternion q=srv.response.pose.orientation;
            transform.setRotation(tf::Quaternion(srv.response.pose.orientation.x,srv.response.pose.orientation.y,srv.response.pose.orientation.z,srv.response.pose.orientation.w));
            br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), srv.request.frame_a, srv.request.frame_b));
        }
        else {
            ROS_ERROR("Failed to call service");
            //return 1;
        }
        r.sleep();
    }    
    return 0;





}