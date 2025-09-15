import rclpy                                                                            #ROS2의 Python 라이브러리
from rclpy.node import Node                                                             #ROS2에서 노드를 만들기위한 기본 클래스
from std_msgs.msg import String                                                         #퍼블리시할 메시지 타입
from std_msgs.msg import Int32
from std_msgs.msg import Float32
                                                                                        #msg는 메시지, msg = String() / msg.data = "Hello World"
class HelloworldPublisher(Node):                                                        #Node를 상속해서 ROS2의 기능을 상속하며 class 정의

    def __init__(self):                                                                 #Python에서 생성자 클래스 설계시 씀
        super().__init__('Helloworld_publisher')                                        #super()은 부모클래스의 매서드나 속성에 접근할 수 있게 해줌. 해당 노드를 helloworld_publisher라는 이름으로 ROS2에 저장
        self.helloworld_publisher = self.create_publisher(String,'/helloworld',10)      #(메시지타입, 토픽이름, 버퍼크기), helloworld_publisher에 저장
        self.helloworld2_publisher = self.create_publisher(Int32, '/helloworld2', 10)
        self.helloworld3_publisher = self.create_publisher(Float32, '/helloworld3', 10)
        self.timer = self.create_timer(1, self.publish_helloworld_msg)                  #(타이머주기, 호출할함수)
        self.count = 0

    def publish_helloworld_msg(self):                                                   #String 메시지 생성 후 
        msg = String()                                                                  
        msg.data = 'Hello World: {0}'.format(self.count)                                #data 필드에 Hello wolrd 입력
        self.helloworld_publisher.publish(msg)                                          #메시지를 /helloworld에 publish
        self.get_logger().info('Published message: {0}'.format(msg.data))               #로그로 퍼블리시한 메시지 출력
        
        int_msg = Int32()
        int_msg.data = self.count
        self.helloworld2_publisher.publish(int_msg)
        self.get_logger().info(f'Published message: {int_msg.data}')

        float_msg = Float32()
        float_msg.data = float(self.count * 0.1)
        self.helloworld3_publisher.publish(float_msg)
        self.get_logger().info(f'Published message: {float_msg.data}')

        self.count += 1



def main(args = None):                                                                  #ROS2 초기화 후 HelloPublisher 객체 생성
    rclpy.init(args=args)                                                               
    node = HelloworldPublisher()                                                       
    try:                                                                                #노드를 계속 실행해 타이머 콜백 기다림
        rclpy.spin(node)
    except KeyboardInterrupt:                                                           #KeyboardInterrupt = 컨트롤c눌렀을때
        node.get_logger().info('Keyboard Interrupt (SIGINT)')                           #get_logger - ROS2의 로그 출력 함수(로그는 기록)
    finally:                                                                            #노드 종료 및 ROS2 shutdown
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()