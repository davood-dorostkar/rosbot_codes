#pragma once

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <opencv2/tracking.hpp>

class Tracker : public rclcpp::Node
{
public:
    constexpr static float MIN_ANG_VEL = 0.15f;
    constexpr static float MAX_ANG_VEL = 0.5f;
    constexpr static float ANGULAR_GAIN = 1.7f;

    Tracker();

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    void initTracker(cv::Mat frame, cv::Rect obj);
    void designateController(geometry_msgs::msg::Twist &vel_msg, cv::Rect obj, uint32_t img_width);

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subImage;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pubVelocity;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pubVisualization;
    cv::Ptr<cv::Tracker> tracker;
    bool isTrackerInitialized;
};