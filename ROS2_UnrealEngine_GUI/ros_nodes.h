#ifndef ROS_NODES_H
#define ROS_NODES_H

#include <QObject>
#include <QDebug>
#include <QImage>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>
#include "ros_image_provider.h"

using twistMsg = geometry_msgs::msg::Twist;
using imageMsg = sensor_msgs::msg::Image;

typedef struct {
    rclcpp::Publisher<twistMsg>::SharedPtr vel;
    rclcpp::Subscription<imageMsg>::SharedPtr cam_sub;
} Pub_t;

class RosNode : public QObject, public rclcpp::Node
{
    Q_OBJECT
public:
    explicit RosNode(QObject *parent = nullptr);
    ~RosNode();
    Q_INVOKABLE void buttonCallback(int number);
    void setImageProvider(RosImageProvider *provider) { image_provider = provider; }

signals:
    void imageReceived(QImage image);

private:
    Pub_t pub;
    void rosSpin();
    void imageCallback(const imageMsg::SharedPtr msg);
    std::thread spin_thread;
    std::string ros_message[4] = {"up", "down", "left", "right"};
    RosImageProvider *image_provider = nullptr;
};

#endif // ROS_NODES_H
