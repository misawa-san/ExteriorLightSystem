#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "ros_task.hpp"

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RosTask>());
    rclcpp::shutdown();

    //printf("\n");
    //printf("finish mclient_main()\n");
    fflush(stdout);

    return 0;
}

