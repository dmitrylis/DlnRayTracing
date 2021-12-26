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
    //for (int x = 0; x < width(); ++x)
    //{
    //    for (int y = 0; y < height(); ++y)
    //    {
    //        QRgb pixelData = pixel(i,j);
    //
    //        setPixelColor(x, y, QColor(255 * m_rChannel[x][y] / m_channelFactor,
    //                                   255 * m_gChannel[x][y] / m_channelFactor,
    //                                   255 * m_bChannel[x][y] / m_channelFactor));
    //    }
    //}

    for (int y = 0; y < height(); ++y)
    {
        QRgb *line = reinterpret_cast<QRgb*>(scanLine(y));
        for (int x = 0; x < width(); ++x) {
            QColor pixelColor(255 * m_rChannel[x][y] / m_channelFactor,
                              255 * m_gChannel[x][y] / m_channelFactor,
                              255 * m_bChannel[x][y] / m_channelFactor);
            line[x] = pixelColor.rgb();
        }
    }
}
