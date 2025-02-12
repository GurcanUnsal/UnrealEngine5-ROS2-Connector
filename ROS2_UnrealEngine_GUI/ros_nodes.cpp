#include "ros_nodes.h"

RosNode::RosNode(QObject *parent): QObject{parent}, Node{"velocity_publisher"}
{
    spin_thread = std::thread{ std::bind(&RosNode::rosSpin, this) };
    pub.vel = this->create_publisher<twistMsg>("cmd_vel", 10);

    pub.cam_sub = this->create_subscription<imageMsg>(
        "/cam", 10, std::bind(&RosNode::imageCallback, this, std::placeholders::_1)
        );
}

RosNode::~RosNode() {
    rclcpp::shutdown();
}

void RosNode::rosSpin() {
    rclcpp::Rate rate(30);
    while (rclcpp::ok()) {
        rclcpp::spin_some(this->get_node_base_interface());
        rate.sleep();
    }
    rclcpp::shutdown();
}

void RosNode::buttonCallback(int number){
    if (number > 4) {
        return;
    }

    geometry_msgs::msg::Twist velocity;

    if (number == 0) {
        velocity.linear.x = 100.0;
    } else if (number == 1) {
        velocity.linear.x = -100.0;
    } else if (number == 2) {
        velocity.angular.z = -5.0;
    } else if (number == 3) {
        velocity.angular.z = 5.0;
    }

    pub.vel->publish(velocity);
}

void RosNode::imageCallback(const imageMsg::SharedPtr msg) {
    cv::Mat cv_image = cv_bridge::toCvCopy(msg, "bgr8")->image;
    QImage qimage(cv_image.data, cv_image.cols, cv_image.rows, cv_image.step, QImage::Format_RGB888);

    emit imageReceived(qimage.rgbSwapped());

    if (image_provider) {
        image_provider->updateImage(qimage.rgbSwapped());
    }
}
