#ifndef ROS_IMAGE_PROVIDER_H
#define ROS_IMAGE_PROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QObject>

class RosImageProvider : public QQuickImageProvider
{
    Q_OBJECT

public:
    RosImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void updateImage(const QImage &image);

signals:
    void imageChanged();

private:
    QImage m_image;
};

#endif // ROS_IMAGE_PROVIDER_H
