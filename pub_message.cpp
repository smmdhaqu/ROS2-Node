#include<chrono>
#include<functional>
#include<memory>
#include<string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. And for demo, just Publishing Data instead of Data from NatNetSDK*/

class PosePublisher : public rclcpp::Node
{
  public:
   PosePublisher()
    : Node("pose_publisher"), count_(0)
    { 
      publisher_ = this->create_publisher<std_msgs::msg::String>("pose_msg", 10); //Testing: The Messsage will be string type
      timer_ = this->create_wall_timer(                                           //Timer will call the class after certain Time and create timer
      500ms, std::bind(&PosePublisher::timer_callback, this));                    //Every 500ms the timer will call the function timer_callback
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();                                     //ROS2 message type
      message.data = "Pose Data: " + std::to_string(count_++);                    //Later we will take Data from NatNetSDK insted of "Pose Data:" and then will get values from message.x,message.y etc
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());  //RCLCPP_INFO is for Debugging to see publishing message or not
      publisher_->publish(message);                                               //publish Node will Publish Message
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PosePublisher>());  //The call PosePublisher will spin and calling timer_callback in every 500ms
  rclcpp::shutdown();
  return 0;
}

