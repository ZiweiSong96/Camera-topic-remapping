#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"

class ImageRemap_30 : public rclcpp::Node{
  public:
    ImageRemap_30():Node("image_remap_30")
    {
      //rclcpp::QoS sub_qos_1(rclcpp::KeepLast(10));
      //qos.reliability(RMW_QOS_POLICY_RELIABILITY_RELIABLE);
      //sub_qos_1.reliability(RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT);
      image_subscription_ = create_subscription<sensor_msgs::msg::Image>(
        "/sensing/camera/traffic_light/image_raw",rclcpp::QoS(rclcpp::KeepLast(10)).best_effort(),
        std::bind(&ImageRemap_30::imageCallback,this,std::placeholders::_1)
      );
      camera_info_subscription_ = create_subscription<sensor_msgs::msg::CameraInfo>(
        "/sensing/camera/traffic_light/camera_info",rclcpp::QoS(rclcpp::KeepLast(10)).best_effort(),
        std::bind(&ImageRemap_30::cameraInfoCallback,this,std::placeholders::_1)
      );      
      // rclcpp::QoS sub_qos_2(rclcpp::KeepLast(10));
      // //qos.reliability(RMW_QOS_POLICY_RELIABILITY_RELIABLE);
      // sub_qos_2.reliability(RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT);
      // camera_info_subscription_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
      //   "/sensing/camera/traffic_light/camera_info",sub_qos_2,
      //   std::bind(&ImageRemap::cameraInfoCallback,this,std::placeholders::_1)
      // );

      //rclcpp::QoS pub_qos_1(rclcpp::KeepLast(10));
      //pub_qos_1.reliability(RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT);
      image_publisher_=create_publisher<sensor_msgs::msg::Image>(
        "/sensing/camera/camera0/image_rect_color",
        rclcpp::QoS(rclcpp::KeepLast(10)).best_effort());
      camera_info_publisher_=create_publisher<sensor_msgs::msg::CameraInfo>(
        "/sensing/camera/camera0/camera_info",
        rclcpp::QoS(rclcpp::KeepLast(10)).best_effort());      
      // rclcpp::QoS pub_qos_2(rclcpp::KeepLast(10));
      // pub_qos_2.reliability(RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT);
      // camera_info_publisher_=this->create_publisher<sensor_msgs::msg::CameraInfo>(
      //   "/sensing/camera0/image_rect/camera_info0",pub_qos_2);
      timer_=create_wall_timer(std::chrono::milliseconds(33),
      std::bind(&ImageRemap_30::timer_callback, this));        
    }
  private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
      //image_publisher_->publish(*msg);
      latest_image_msg_ = msg;
    }  
    void cameraInfoCallback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
    {
      //camera_info_publisher_->publish(*msg);
      latest_camera_info_msg_ = msg;
    }
    void timer_callback()
    {
      if(latest_image_msg_ && latest_camera_info_msg_)
      {
        image_publisher_->publish(*latest_image_msg_);
        camera_info_publisher_->publish(*latest_camera_info_msg_);
      }
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_subscription_;
    rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    sensor_msgs::msg::Image::SharedPtr latest_image_msg_;
    sensor_msgs::msg::CameraInfo::SharedPtr latest_camera_info_msg_;
};

int main(int argc, char ** argv)
{
//  (void) argc;
//  (void) argv;
  rclcpp::init(argc,argv);
  auto node = std::make_shared<ImageRemap_30>();
  rclcpp::spin(node);
  //printf("hello world image_remap package\n");
  return 0;
}
