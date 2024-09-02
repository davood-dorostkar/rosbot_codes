#include "tutorial_pkg/tracker.hpp"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

using namespace std::placeholders;

Tracker::Tracker() : Node("tracker"), isTrackerInitialized(false)
{
    subImage = create_subscription<sensor_msgs::msg::Image>("/image", rclcpp::SensorDataQoS(), std::bind(&Tracker::imageCallback, this, _1));
    pubVisualization = create_publisher<sensor_msgs::msg::Image>("/visualization", rclcpp::SensorDataQoS());
    pubVelocity = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", rclcpp::SensorDataQoS());
    RCLCPP_INFO(get_logger(), "node started.");
}

void Tracker::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
  cv_bridge::CvImagePtr cv_image = cv_bridge::toCvCopy(msg, "bgr8");
  cv::Mat frame = cv_image->image;
  cv::Rect obj;
  geometry_msgs::msg::Twist vel_msg;

  if (!isTrackerInitialized)
  {
    initTracker(frame, obj);
  }

  bool ok = tracker->update(frame, obj);

  if (ok) {
    // Calculate angular speed based on the position of the object
    designateControl(vel_msg, obj, msg->width);
    RCLCPP_INFO(get_logger(), "Angular velocity: %0.2f", vel_msg.angular.z);
  }
  else {
    RCLCPP_WARN(get_logger(), "Tracking failure detected. Stop vehicle!");
    putText(frame, "Tracking failure detected", cv::Point(100, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);
  }

  _vel_pub->publish(vel_msg);

  // Publish visualization with rectangle around the tracked object
  rectangle(frame, obj, cv::Scalar(255, 0, 0), 2, 1);

  cv_image->image = frame;
  auto img_msg = cv_image->toImageMsg();
  _visualization_pub->publish(*img_msg);
}

void Tracker::initTracker(cv::Mat frame, cv::Rect obj)
{
  obj = selectROI("ROI selector", frame, false);
  tracker = cv::TrackerKCF::create();
  tracker->init(frame, obj);
  isTrackerInitialized = true;
  cv::destroyWindow("ROI selector");
  cv::waitKey(1);
}

void Tracker::designateControl(geometry_msgs::msg::Twist &vel_msg, cv::Rect obj, uint32_t img_width)
{
    int obj_x_center = obj.x + obj.width / 2;
    int px_to_center = img_width / 2 - obj_x_center;
    float ang_vel = ANGULAR_GAIN * px_to_center / static_cast<float>(img_width);

    // Ensure angular velocity is within bounds
    if ((ang_vel >= -MAX_ANG_VEL && ang_vel <= -MIN_ANG_VEL) || (ang_vel >= MIN_ANG_VEL && ang_vel <= MAX_ANG_VEL)) {
      vel_msg.angular.z = ang_vel;
    }
}

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Tracker>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}