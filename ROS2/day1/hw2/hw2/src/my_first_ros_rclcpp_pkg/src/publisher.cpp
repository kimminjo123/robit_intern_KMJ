/*#include "rclcpp/rclcpp.hpp"

class MyCppNode : public rclcpp::Node{  
    public:
    MyCppNode() : Node("my_cpp_node"){
        RCLCPP_INFO(this->get_logger(), "Hello world2");
    }
};

void MyCppNode::timer_callback(){
    auto msg = std_msgs::msg::String();
    msg.data = "Hello World: " + std::to_string(count++);
    RCLCPP_INFO(this->get_logger(), "Published message: '%s'", msg.data.c_str());
    mycpp_publisher_->publish(msg);
}


int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MyCppNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}*/


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"                                                          //메시지 타입 정의
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;                                                       //1000ms, 1s 같은 시간 리터럴 쓰기 위해 사용

class MyPublisherNode : public rclcpp::Node                                                 //ROS2 노드는 rclcpp::Node를 상속해서 만듬 -> 노드의 기능 사용 가능
{
public:
    MyPublisherNode()
    : Node("my_publisher_node"), count_(0)                                                  //my_publisher_node를 넘기고 count를 0으로 초기화
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>("topicname", 10);        //topicname이라는 topic으로 String타입 퍼블리셔 만듬.
        int_publisher_ = this->create_publisher<std_msgs::msg::Int32>("topicname2", 10);
        float_publisher_ = this->create_publisher<std_msgs::msg::Float32>("topicname3", 10);
        timer_ = this->create_wall_timer(
            1s, std::bind(&MyPublisherNode::timer_callback, this));                         //std::bind는 호출 가능한 객체를 만들어줌. -> 1초마다 timer_callback() 멤버 함수를 호출하는
    }                                                                                       //타이머를 생성하고, 그 타이머를 timer_에 저장

private:
    void timer_callback()
    {
        auto all_count = count_;
        auto message = std_msgs::msg::String();                                             //메시지 객체 만듬
        message.data = "Hello World: " + std::to_string(all_count);                         //message.data에 "Hello World: " + count 문자열 넣음
        RCLCPP_INFO(this->get_logger(), "Published message: '%s'", message.data.c_str());   //정보레벨로그를 출력. c_str: C스타일 문자열로 변환해주는 함수
        publisher_->publish(message);                                                       //퍼블리셔로 메시지 발행

        auto int_message = std_msgs::msg::Int32();                                           
        int_message.data = all_count;                        
        RCLCPP_INFO(this->get_logger(), "Published int_message: '%d'", int_message.data);
        int_publisher_->publish(int_message);

        auto float_message = std_msgs::msg::Float32();                                             
        float_message.data = all_count * 0.1f;                         
        RCLCPP_INFO(this->get_logger(), "Published message: '%f'", float_message.data);
        float_publisher_->publish(float_message);    
        count_++;
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;                         //ROS2퍼블리셔 객체를 가리키는 포인터, 문자열 메시지를 특정 토픽으로 발행
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr int_publisher_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr float_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;                                                    //create_wall_timer()를 호출하면 주기적으로 콜백 함수를 실행해주는 타이머 만들어짐
    size_t count_;                                                                          //(위설명이어서)이 멤버를 저장해 두지 않으면 콜백 실행 X
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);                                                               //ROS2 시스템 초기회
    rclcpp::spin(std::make_shared<MyPublisherNode>());                                      //노드를 생성해 스핀(이벤트루프)을 시작. 스핀은 노드가 살아있는 동안 타이머/subscribe 콜백등을 호출
    rclcpp::shutdown();                                                                     //ROS2 종료
    return 0;
}
