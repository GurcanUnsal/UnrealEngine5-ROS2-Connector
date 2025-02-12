#include "ros_image_provider.h"

RosImageProvider::RosImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage RosImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    if (size)
        *size = m_image.size();

    if (m_image.isNull()) {
        return QImage(requestedSize.width() > 0 ? requestedSize.width() : 640,
                      requestedSize.height() > 0 ? requestedSize.height() : 480,
                      QImage::Format_RGB32);
    }

    if (requestedSize.width() > 0 && requestedSize.height() > 0)
        return m_image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);

    return m_image;
}

void RosImageProvider::updateImage(const QImage &image)
{
    m_image = image;
    emit imageChanged();
}
