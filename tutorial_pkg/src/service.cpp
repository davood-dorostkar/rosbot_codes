#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/u_int8.hpp"

using namespace std::placeholders;
using namespace std::chrono_literals;

class ServiceNode : public rclcpp::Node
{
public:
    ServiceNode() : Node("service_node")
    {
        declare_parameter("timer_period", 5);
        auto timer_period = std::chrono::seconds(get_parameter("timer_period").as_int());
        sub = create_subscription<sensor_msgs::msg::Image>("/image", rclcpp::SensorDataQoS(), std::bind(&ServiceNode::image_callback, this, _1));
        pub = create_publisher<std_msgs::msg::UInt8>("/brightness", rclcpp::SensorDataQoS());
        timer = create_wall_timer(timer_period, std::bind(&ServiceNode::timer_callback, this));
        // timer = create_wall_timer(5s, std::bind(&ServiceNode::timer_callback, this));
        RCLCPP_INFO(get_logger(), "Service node started!");
    }
private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub;
    rclcpp::Publisher<std_msgs::msg::UInt8>::SharedPtr pub;
    rclcpp::TimerBase::SharedPtr timer;

    void image_callback(const sensor_msgs::msg::Image::SharedPtr image)
    {
        size_t avg = 0;
        for (auto& item : image->data)
        {
            avg += item;
        }
        avg /= image->data.size();
        std_msgs::msg::UInt8 brightness;
        brightness.data = avg;
        pub->publish(brightness);
    }

    void timer_callback()
    {
        RCLCPP_INFO(get_logger(), "timer activated");
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ServiceNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}