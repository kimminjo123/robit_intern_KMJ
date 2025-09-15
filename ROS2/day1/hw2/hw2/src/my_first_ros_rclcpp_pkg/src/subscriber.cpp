/*#include "rclcpp/rclcpp.hpp"

class MyCppNode : public rclcpp::Node{  
    public:
    MyCppNode() : Node("my_cpp_node"){
        RCLCPP_INFO(this->get_logger(), "Hello world3");
    }
};

void topic_callback(const std_msgs::msg::String::SharedPtr msg){
    RCLCPP_INFO(this->get_logger(), "Received message: '%s'", msg->data.c_str());
}

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MyCppNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}*/

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"

class MySubscriberNode : public rclcpp::Node
{
public:
    MySubscriberNode()
    : Node("my_subscriber_node")
    {
        // subscriber 생성
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "topicname", 10,
            std::bind(&MySubscriberNode::topic_callback, this, std::placeholders::_1));

        int_subscription_ = this->create_subscription<std_msgs::msg::Int32>(
            "topicname2", 10,
            std::bind(&MySubscriberNode::int_callback, this, std::placeholders::_1)
        );

        float_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
            "topicname3", 10,
            std::bind(&MySubscriberNode::float_callback, this, std::placeholders::_1)
        );
    }

private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received message: '%s'", msg->data.c_str());
    }

    void int_callback(const std_msgs::msg::Int32::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(), "Received message: '%d'", msg->data);
    }

    void float_callback(const std_msgs::msg::Float32::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(), "Received message: '%f'", msg->data);
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr int_subscription_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr float_subscription_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MySubscriberNode>());
    rclcpp::shutdown();
    return 0;
}
