#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "cmath"

using namespace std::placeholders;

class Controller : public rclcpp::Node
{
public:
    Controller() : Node("controller")
    {
        declare_parameter("interval", 50);
        auto interval = std::chrono::milliseconds(get_parameter("interval").as_int());
        timer = create_wall_timer(interval, std::bind(&Controller::controlCallback, this));
        pub = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", rclcpp::SensorDataQoS());
        RCLCPP_INFO(get_logger(), "node started");
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub;
    rclcpp::TimerBase::SharedPtr timer;
    double sinIncrement = 0.2f;
    double arc = 0.0f;

    void controlCallback()
    {
        geometry_msgs::msg::Twist command;
        arc += sinIncrement;
        command.angular.z = sin(arc);
        command.linear.x = sin(arc);
        pub->publish(command);
        RCLCPP_INFO(get_logger(), "new command");
    }
};


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto controller = std::make_shared<Controller>();
    rclcpp::spin(controller);
    rclcpp::shutdown();
    return 0;
}