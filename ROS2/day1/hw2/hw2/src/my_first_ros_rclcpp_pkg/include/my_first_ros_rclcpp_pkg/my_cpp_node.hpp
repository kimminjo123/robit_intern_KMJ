class MyNode{
    mycpp_publisher_ = this->create_publisher<std_msgs::msg:String>("topicname", 10);
    timer_ = this->create_wall_timer(1s, std::bind(&MyCppNode::timer_callback,this));
    private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mycpp_publisher_;
};

MyNode::MyNode() : Node("mynode"){
    publisher = this->create_publisher<std_msgs::msg:String>("topicname", 10);
}

MyNode::~MyNode(){

}