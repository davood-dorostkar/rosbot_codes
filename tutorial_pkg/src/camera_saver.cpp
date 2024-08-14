#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_srvs/srv/empty.hpp"

using namespace std::placeholders;
using namespace std::chrono_literals;

class CameraSaver : public rclcpp::Node
{
public:
    CameraSaver() : Node("camera_saver_node")
    {
        declare_parameter("period", 5);
        auto period = std::chrono::seconds(get_parameter("period").as_int());
        sub = create_subscription<sensor_msgs::msg::Image>("/image", rclcpp::SensorDataQoS(), std::bind(&CameraSaver::imageCallback, this, _1));
        pub = create_publisher<std_msgs::msg::UInt8>("/brightness", rclcpp::SensorDataQoS());
        timer = create_wall_timer(period, std::bind(&CameraSaver::timerCallback, this));
        client = create_client<std_srvs::srv::Empty>("/save");

        RCLCPP_INFO(get_logger(), "Node started!");
    }
private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub;
    rclcpp::Publisher<std_msgs::msg::UInt8>::SharedPtr pub;
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr client;

    void imageCallback(const sensor_msgs::msg::Image::SharedPtr image)
    {
        size_t avg = 0;
        for (auto& pixel : image->data)
        {
            avg += pixel;
        }
        std_msgs::msg::UInt8 brightness;
        brightness.data = avg / image->data.size();
        pub->publish(brightness);
    }

    void timerCallback()
    {
        RCLCPP_INFO(get_logger(), "timer activated");
        if(!client->wait_for_service(1s))
        {
            RCLCPP_ERROR(get_logger(), "cannot connect to camera");
            return;
        }
        auto request = std::make_shared<std_srvs::srv::Empty::Request>();
        auto future = client->async_send_request(request);
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CameraSaver>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}