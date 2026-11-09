#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "patrol_interfaces/msg/patrol_status.hpp"

using namespace std::chrono_literals;

/// 周期性发布巡逻状态，同时订阅同一话题把收到的内容打印出来。
class PatrolNode : public rclcpp::Node
{
public:
  PatrolNode()
  : Node("patrol_node")
  {
    pub_ = this->create_publisher<patrol_interfaces::msg::PatrolStatus>(
      "patrol_status", 10);

    sub_ = this->create_subscription<patrol_interfaces::msg::PatrolStatus>(
      "patrol_status", 10,
      [this](const patrol_interfaces::msg::PatrolStatus::SharedPtr msg) {
        RCLCPP_INFO(
          this->get_logger(), "[patrol] target=%d battery=%.1f arrived=%s",
          msg->target_id, msg->battery, msg->arrived ? "true" : "false");
      });

    timer_ = this->create_wall_timer(500ms, [this]() {
      auto msg = patrol_interfaces::msg::PatrolStatus();
      msg.target_id = 3;
      msg.battery = 88.5f;
      msg.arrived = false;
      pub_->publish(msg);
    });
  }

private:
  rclcpp::Publisher<patrol_interfaces::msg::PatrolStatus>::SharedPtr pub_;
  rclcpp::Subscription<patrol_interfaces::msg::PatrolStatus>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PatrolNode>());
  rclcpp::shutdown();
  return 0;
}
