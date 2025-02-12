#include <memory>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

struct LinearVelocity {
    float x, y, z;
};

struct AngularVelocity {
    float x, y, z;
};

class VelocitySubscriber : public rclcpp::Node
{
public:
    VelocitySubscriber()
    : Node("velocity_subscriber")
    {
        subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel", 10, std::bind(&VelocitySubscriber::velocity_callback, this, std::placeholders::_1));

        socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd_ == -1) {
            RCLCPP_ERROR(this->get_logger(), "Failed to create socket");
            return;
        }

        sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(5000);
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(socket_fd_, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
            RCLCPP_ERROR(this->get_logger(), "Failed to connect to server");
            close(socket_fd_);
            return;
        }

        RCLCPP_INFO(this->get_logger(), "Connected to TCP server on port 5000");
    }

    ~VelocitySubscriber()
    {
        if (socket_fd_ != -1) {
            close(socket_fd_);
        }
    }

private:
    void velocity_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received velocity: linear.x=%.2f, angular.z=%.2f", 
                    msg->linear.x, msg->angular.z);

        LinearVelocity linear_velocity = {msg->linear.x, msg->linear.y, msg->linear.z};
        AngularVelocity angular_velocity = {msg->angular.x, msg->angular.y, msg->angular.z};

        ssize_t bytes_sent = send(socket_fd_, reinterpret_cast<char*>(&linear_velocity), sizeof(linear_velocity), 0);
        if (bytes_sent == -1) {
            RCLCPP_ERROR(this->get_logger(), "Failed to send linear velocity over TCP");
        }

        bytes_sent = send(socket_fd_, reinterpret_cast<char*>(&angular_velocity), sizeof(angular_velocity), 0);
        if (bytes_sent == -1) {
            RCLCPP_ERROR(this->get_logger(), "Failed to send angular velocity over TCP");
        }
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
    int socket_fd_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<VelocitySubscriber>());
    rclcpp::shutdown();
    return 0;
}
