import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import Int32
from std_msgs.msg import Float32

class HelloworldSubscriber(Node):                                           #ROS2 노드 정의하는 클래스

    def __init__(self):
        super().__init__('Helloworld_subscriber')                           #부모클래스의 생성자를 호출하며 노드이름 등록
        self.subscription = self.create_subscription(                       #(메시지타입, 토픽이름, 메시지수신시호출할콜백함수, 버퍼크기)
            String,
            'topicname',
            self.subscribe_topic_message,       
            10
        )

        self.int_subscription = self.create_subscription(
            Int32,
            'topicname2',
            self.int_topic_message,
            10
        )

        self.float_subscription = self.create_subscription(
            Float32,
            'topicname3',
            self.float_topic_message,
            10
        )

    def subscribe_topic_message(self,msg):                                  #메시지 받을때마다 자동호출됨, msg.data에 publisher가 보낸 문자열이 들어있음
        self.get_logger().info('Received message: {0}'.format(msg.data))
    
    def int_topic_message(self, int_msg):
        self.get_logger().info(f'Received message: {int_msg.data}')
    
    def float_topic_message(self, float_msg):
        self.get_logger().info(f'Received message: {float_msg.data}')

def main(args = None):
    rclpy.init(args=args)                                                   #ROS2초기화하고 해당노드객체 생성
    node = HelloworldSubscriber()
    try:
        rclpy.spin(node)                                                    #콜백기다림
    except KeyboardInterrupt:                        
        node.get_logger().info('Keyboard Interrupt (SIGINT)')
    finally:
        node.destroy_node()                                                 #노드 정상 종료 및 ROS2 종료
        rclpy.shutdown()

if __name__ == '__main__':
    main()