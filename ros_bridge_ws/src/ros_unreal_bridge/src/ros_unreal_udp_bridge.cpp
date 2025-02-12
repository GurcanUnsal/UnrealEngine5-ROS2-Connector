#include <opencv2/opencv.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>

#define PORT 12345
#define IMAGE_WIDTH 200
#define IMAGE_HEIGHT 100

class UdpToRosNode : public rclcpp::Node
{
public:
    UdpToRosNode() : Node("udp_to_ros")
    {
        image_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/cam", 10);

        UdpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (UdpSocket < 0)
        {
            RCLCPP_ERROR(this->get_logger(), "Socket creation failed");
            return;
        }

        sockaddr_in ServerAddr{};
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_port = htons(PORT);
        ServerAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(UdpSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0)
        {
            RCLCPP_ERROR(this->get_logger(), "Binding failed");
            return;
        }

        buffer.resize(IMAGE_WIDTH * IMAGE_HEIGHT * 3);
    }

    ~UdpToRosNode()
    {
        close(UdpSocket);
    }

    void receive_and_publish()
    {
        cv::Mat ImageRGB(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
        sockaddr_in ClientAddr{};
        socklen_t ClientAddrLen = sizeof(ClientAddr);

        int ReceivedBytes = recvfrom(UdpSocket, buffer.data(), buffer.size(), 0, (struct sockaddr*)&ClientAddr, &ClientAddrLen);
        if (ReceivedBytes > 0)
        {
            memcpy(ImageRGB.data, buffer.data(), buffer.size());

            cv::Mat ImageBGR;
            cv::cvtColor(ImageRGB, ImageBGR, cv::COLOR_RGB2BGR);

            cv::Mat ResizedImage;
            cv::resize(ImageBGR, ResizedImage, cv::Size(416, 416));

            auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", ResizedImage).toImageMsg();
            image_pub_->publish(*msg);
        }
    }

private:
    int UdpSocket;
    std::vector<uint8_t> buffer;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<UdpToRosNode>();

    rclcpp::Rate rate(30);
    while (rclcpp::ok())
    {
        node->receive_and_publish();
        rclcpp::spin_some(node);
        rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
