#include <QCoreApplication>
#include <QtQml>
#include <QtCore>
#include <QColor>
#include <QDebug>
#include <QObject>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "ros_nodes.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    rclcpp::init(argc, argv);

    QQmlApplicationEngine engine;
    RosNode node;
    RosImageProvider *imageProvider = new RosImageProvider;
    node.setImageProvider(imageProvider);
    engine.addImageProvider(QLatin1String("rosimage"), imageProvider);

    QQmlContext* context = engine.rootContext();
    context->setContextProperty("_rosNode", &node);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    return app.exec();
}
