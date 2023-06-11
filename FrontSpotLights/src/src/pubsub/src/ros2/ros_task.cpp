// Include important C++ header files that provide class
// templates for useful operations.
#include <chrono>     // Date and time
#include <functional> // Arithmetic, comparisons, and logical operations
#include <memory>     // Dynamic memory management
#include <string>     // String functions

// ROS Client Library for C++
// Allows use of the most common elements of ROS 2
#include <rclcpp/rclcpp.hpp>

// Custom message type that will be used to publish data
#include <custom_msg/msg/custom_msg.hpp>

#include "mclient.h"
#include "ros_task.hpp"

// chrono_literals handles user-defined time durations (e.g. 500ms) 
using namespace std::chrono_literals;
 

RosTask::RosTask(
  const std::string& name_space, 
  const rclcpp::NodeOptions& options
/* ------------------ USER CUSTOIMIZE ----------------------------*/
): Node("ros_task2",name_space,options)
/* ------------------ USER CUSTOIMIZE ----------------------------*/
{
    // Initializing mclient
    int ret = mclient_init();

    // If mclient initialized normally
    if(!ret)
    {
        _subscription = this->create_subscription<custom_msg::msg::CustomMsg>(
/* ------------------ USER CUSTOIMIZE ----------------------------*/
                            "topic1",
/* ------------------ USER CUSTOIMIZE ----------------------------*/
                                10,
                            std::bind(&RosTask::_topic_callback, this, std::placeholders::_1)
                            );
        timer_ = this->create_wall_timer(
/* ------------------ USER CUSTOIMIZE ----------------------------*/
                                           10ms,
/* ------------------ USER CUSTOIMIZE ----------------------------*/
                                            [this](){
                                                // receive ros2 message
                                                this->subscribe_message();

                                                // Call mclient main function
                                                int ret = mclient_main();

                                                // Send ros2 message
                                                this->publish_message();

                                                // Finish main if ret is to set non-zero
                                                if(ret)
                                                        {
                                                     // Shutdown ros2
                                                     rclcpp::shutdown();
                                                        }
                                                  }
                                                );
    }
    else
    {
        // Shutdown ros2
        rclcpp::shutdown();
    }

}

// subscribe message
void RosTask::_topic_callback(const custom_msg::msg::CustomMsg::SharedPtr msg){
/* ------------------ USER CUSTOIMIZE ----------------------------*/

    for(int i=0; i<8; i++)
    {
        payload[i] = msg->payload[i];
    }

    //RCLCPP_INFO(this->get_logger(), "I heard: %d %d %d", msg->payload[0], msg->payload[1], msg->payload[2]);

/* ------------------ USER CUSTOIMIZE ----------------------------*/
}

// subscribe message
void RosTask::subscribe_message()
{
/* ------------------ USER CUSTOIMIZE ----------------------------*/

/* ------------------ USER CUSTOIMIZE ----------------------------*/
}

// publish message
void RosTask::publish_message()
{
/* ------------------ USER CUSTOIMIZE ----------------------------*/

    //RCLCPP_INFO(this->get_logger(), "call main");

/* ------------------ USER CUSTOIMIZE ----------------------------*/
}

