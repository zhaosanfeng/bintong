#include <ros/ros.h>
#include <std_msgs/String.h>
#include <bintong/Message1.h>
#include <bintong/Service1.h>
enum Robot_State
{
    Ready,
    Executing,
    Pending
};
enum Task
{
    One=1,
    Tow,
    Three,
    Four,
    Five,
    Over
};
Robot_State gcurrent_state  = Ready;
Task        gcurrent_task   = One;
bool Robot1(bintong::Service1::Request& req,
            bintong::Service1::Response& res)
{
    if((gcurrent_task = (Task)req.task) != Over)
    {
        gcurrent_state = Executing;
    }
    else
    {
        gcurrent_state = Pending;
        ROS_INFO("robot1 have finished all task!\n");
    }
    return true;
}


int main(int argc,char** argv)
{
    ros::init(argc,argv,"node_robot_1");
    ros::NodeHandle n;
    ros::ServiceServer service = n.advertiseService("agent_task_1", Robot1);
    ros::Publisher pub_state   = n.advertise<bintong::Message1>("agent_feedback",1);

    ros::Rate loop_sleep(1);
    bintong::Message1 msg;
    while(ros::ok())
    {
        if( Ready == gcurrent_state)
        {
            msg.id         = 1;
            msg.state      = "ready";
            pub_state.publish(msg);
        }
        else if(Executing == gcurrent_state)
        {
            msg.id         = 1;
            msg.state      = "executing";
            pub_state.publish(msg);
            ros::Duration(5.0).sleep();
            ROS_INFO("robot1 has finished task %d!",gcurrent_task);
            if(gcurrent_task!=Five)
            {
                gcurrent_state = Ready;
            }
            else
            {
                gcurrent_state = Pending;
            }
        }
        ros::spinOnce();
        loop_sleep.sleep();
    }
    return 0;
}
