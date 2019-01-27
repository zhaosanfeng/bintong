#include <ros/ros.h>
#include <std_msgs/String.h>
#include "bintong/Message1.h"
#include "bintong/Service1.h"
enum Task
{
    One=1,
    Tow,
    Three,
    Four,
    Five,
    Over
};
enum Robot_State
{
    Ready,
    Executing,
    Pending
};
Task grobot1_task = One;
Task grobot2_task = One;
Robot_State grobot1_state = Pending;
Robot_State grobot2_state = Pending;

void MiniCallback(const bintong::Message1::ConstPtr& msg)
{
    switch (msg->id) {
    case 1:
        if("ready" == msg->state)
        {
            grobot1_state = Ready;
        }
        else if("executing" == msg->state)
        {
            grobot1_state = Executing;
        }
        break;
    case 2:
        if("ready" == msg->state)
        {
            grobot2_state = Ready;
        }
        else if("executing" == msg->state)
        {
            grobot2_state = Executing;
        }

        break;
    default:
        break;
    }

}



int main(int argc,char** argv)
{
    ros::init(argc,argv,"node_mini_factory_server");
    ros::NodeHandle n;
    ros::Subscriber sub        = n.subscribe("agent_feedback",1,MiniCallback);
    ros::ServiceClient client1 = n.serviceClient<bintong::Service1>("agent_task_1");
    ros::ServiceClient client2 = n.serviceClient<bintong::Service1>("agent_task_2");

    ros::Rate loop_sleep(2);
    bintong::Service1 srv1;
    bintong::Service1 srv2;
    while(ros::ok())
    {
        if(Ready == grobot1_state)
        {
            srv1.request.task = (uint8_t)grobot1_task;
            if(grobot1_task < Over)
            {
                if(client1.call(srv1))
                {
                    grobot1_state = Executing;
                    ROS_INFO("Server has published task %d to robot1.",grobot1_task);
                    grobot1_task = (Task)(grobot1_task+1);
                }
            }
            else
            {
                if(client1.call(srv1))
                {
                    grobot1_state = Pending;
                }
            }

        }
        if(Ready == grobot2_state)
        {
            srv2.request.task = (uint8_t)grobot2_task;
            if(grobot2_task < Over)
            {
                if(client2.call(srv2))
                {
                    grobot2_state = Executing;
                    ROS_INFO("Server has published task %d to robot2.",grobot2_task);
                    grobot2_task = (Task)(grobot2_task+1);
                }
            }
            else
            {
                if(client2.call(srv2))
                {
                    grobot2_state = Pending;
                }
            }

        }
        ros::spinOnce();
        loop_sleep.sleep();
    }

    return 0;
}
