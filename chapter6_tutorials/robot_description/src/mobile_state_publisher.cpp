#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "mobile_state_publisher");
    ros::NodeHandle n;
    tf::TransformBroadcaster broadcaster;
    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    ros::Rate loop_rate(30);

    const double degree = M_PI/180;

    // robot state
    double angle= 0;

    // message declarations
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    while (ros::ok()) {
        //update joint_state
        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(4);
        joint_state.position.resize(4);
        joint_state.name[0] ="base_to_wheel1";
        joint_state.position[0] = 0;
        joint_state.name[1] ="base_to_wheel2";
        joint_state.position[1] = 0;
        joint_state.name[2] ="base_to_wheel3";
        joint_state.position[2] = 0;
        joint_state.name[3] ="base_to_wheel4";
        joint_state.position[3] = 0;

        // update transform
        // (moving in a circle with radius)
        odom_trans.header.stamp = ros::Time::now();
        odom_trans.transform.translation.x = cos(angle);
        odom_trans.transform.translation.y = sin(angle);
        odom_trans.transform.translation.z = 0.0;
        odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle);

        //send the joint state and transform
        joint_pub.publish(joint_state);
        broadcaster.sendTransform(odom_trans);


        // Create new robot state
        angle += degree/4;

        // This will adjust as needed per iteration
        loop_rate.sleep();
    }


    return 0;
}
