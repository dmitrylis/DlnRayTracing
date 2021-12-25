#ifndef DLNIMAGE_H
#define DLNIMAGE_H

#include <QImage>
#include <QSize>

namespace dln
{

class DlnImage : public QImage
{
public:
    DlnImage();
    DlnImage(const QSize &size, QImage::Format format);

    void setWidePixelColor(int x, int y, float red, float green, float blue);
    void display();

private:
    std::vector<std::vector<float>> m_rChannel;
    std::vector<std::vector<float>> m_gChannel;
    std::vector<std::vector<float>> m_bChannel;

    float m_channelFactor {0.0};
};

}

#endif // DLNIMAGE_H
