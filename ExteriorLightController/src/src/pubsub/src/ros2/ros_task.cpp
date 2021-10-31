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
): Node("ros_task1",name_space,options)
/* ------------------ USER CUSTOIMIZE ----------------------------*/
{
    // Initializing mclient
    int ret = mclient_init();

    // If mclient initialized normally
    if(!ret)
    {
/* ------------------ USER CUSTOIMIZE ----------------------------*/
        // Publisher publishes custom messages to a topic named "topic1". 
        // The size of the queue is 10 messages.
        publisher_ = this->create_publisher<custom_msg::msg::CustomMsg>("topic1",10);
/* ------------------ USER CUSTOIMIZE ----------------------------*/

        timer_ = this->create_wall_timer(
/* ------------------ USER CUSTOIMIZE ----------------------------*/
                                           20ms,
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
void RosTask::subscribe_message()
{
/* ------------------ USER CUSTOIMIZE ----------------------------*/


/* ------------------ USER CUSTOIMIZE ----------------------------*/
}

// publish message
void RosTask::publish_message()
{
/* ------------------ USER CUSTOIMIZE ----------------------------*/
    // Create a new message
    auto message1 = custom_msg::msg::CustomMsg();

    // Set our message's data attribute
    for(uint8_t i=0; i<8; i++)
    {
        message1.payload[i] = payload[i];
    }
 
    // Print every message to the terminal window      
    RCLCPP_INFO(this->get_logger(),"Publishing: '%d %d %d'", message1.payload[0], message1.payload[1], message1.payload[2]);


    // Publish the message
    publisher_->publish(message1);

/* ------------------ USER CUSTOIMIZE ----------------------------*/
}

