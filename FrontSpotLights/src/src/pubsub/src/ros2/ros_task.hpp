#ifndef __ROS_TASK_HPP__
#define __ROS_TASK_HPP__

#include <rclcpp/rclcpp.hpp>
#include <custom_msg/msg/custom_msg.hpp>


#ifdef __cplusplus
extern "C" {
#endif
/* ------------------ USER CUSTOIMIZE ----------------------------*/
    extern volatile uint8_t payload[8];

/* ------------------ USER CUSTOIMIZE ----------------------------*/
#ifdef __cplusplus
}
#endif

// Create the node class named RosTask which inherits the attributes
// and methods of the rclcpp::Node class.
class RosTask : public rclcpp::Node
{
  private:
    rclcpp::Subscription<custom_msg::msg::CustomMsg>::SharedPtr _subscription;
    rclcpp::TimerBase::SharedPtr timer_;
    void _topic_callback(const custom_msg::msg::CustomMsg::SharedPtr msg);
    void publish_message();
    void subscribe_message();

  public:
    RosTask(
    const std::string& name_space="", 
    const rclcpp::NodeOptions& options = rclcpp::NodeOptions()
    );
};

#endif // __ROS_TASK_HPP__

