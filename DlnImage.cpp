#include "DlnImage.h"

using namespace dln;

DlnImage::DlnImage() : QImage() {}

DlnImage::DlnImage(const QSize &size, QImage::Format format) : QImage(size, format)
{
    m_rChannel.resize(width(), std::vector<float>(height(), 0.0));
    m_gChannel.resize(width(), std::vector<float>(height(), 0.0));
    m_bChannel.resize(width(), std::vector<float>(height(), 0.0));
}

void DlnImage::setWidePixelColor(int x, int y, float red, float green, float blue)
{
    m_rChannel.at(x).at(y) = red;
    m_gChannel.at(x).at(y) = green;
    m_bChannel.at(x).at(y) = blue;

    m_channelFactor = qMax(m_channelFactor, qMax(red, qMax(green, blue)));
}

void DlnImage::display()
{
    for (int x = 0; x < width(); ++x)
    {
        for (int y = 0; y < height(); ++y)
        {
            setPixelColor(x, y, QColor(255 * m_rChannel.at(x).at(y) / m_channelFactor,
                                       255 * m_gChannel.at(x).at(y) / m_channelFactor,
                                       255 * m_bChannel.at(x).at(y) / m_channelFactor));
        }
    }
}
