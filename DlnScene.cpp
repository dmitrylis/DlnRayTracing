#include "DlnScene.h"

#include "DlnSphere.h"
#include "DlnPointLight.h"

using namespace dln;

DlnScene::DlnScene() {
    // setup camera
    m_camera.setPosition(QVector3D(0.0, -10.0, 0.0));
    m_camera.setLookAt(QVector3D(0.0, 0.0, 0.0));
    m_camera.setUp(QVector3D(0.0, 0.0, 1.0));
    m_camera.setProjHSize(0.25);
    //m_camera.setAspectRatio(viewSize.width() / viewSize.height());
    m_camera.updateCameraGeometry();

    // create test objects
    QSharedPointer<DlnSphere> sphere1(new DlnSphere());
    sphere1->setBaseColor(Qt::red);
    m_geometryObjects.push_back(sphere1);

    QSharedPointer<DlnSphere> sphere2(new DlnSphere());
    DlnTransform testTransform2;
    testTransform2.setTransform(QVector3D(-1.5, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.5, 0.5, 1.0));
    sphere2->setTransform(testTransform2);
    sphere2->setBaseColor(Qt::green);
    m_geometryObjects.push_back(sphere2);

    // create test lights
    QSharedPointer<DlnLightObject> light1(new DlnPointLight());
    light1->setColor(Qt::yellow);
    light1->setPosition(QVector3D(5.0, -10.0, -5.0));
    m_lightObjects.push_back(light1);

    //QSharedPointer<DlnLightObject> light2(new DlnPointLight());
    //light2->setColor(Qt::blue);
    //light2->setPosition(QVector3D(5.0, -10.0, 5.0));
    //m_lightObjects.push_back(light2);
}

void DlnScene::initialize(const QSizeF &viewSize)
{
    // setup camera
    m_camera.setAspectRatio(viewSize.width() / viewSize.height());
    m_camera.updateCameraGeometry();
}

bool DlnScene::render(QImage &outputImg)
{
    const int width = outputImg.width();
    const int height = outputImg.height();

    DlnRay currentRay;
    QVector3D intersectionPoint;
    QVector3D localNormal;
    QColor localColor;

    const float xFactor = 1.0 / (static_cast<float>(width) / 2.0);
    const float yFactor = 1.0 / (static_cast<float>(height) / 2.0);
    float minDistance = 1e6;
    float maxDistance = 0.0;

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            // normalizw x and y
            const float normX = static_cast<float>(x) * xFactor - 1.0;
            const float normY = static_cast<float>(y) * yFactor - 1.0;

            // generate ray
            m_camera.generateRay(normX, normY, currentRay);

            for (auto object : m_geometryObjects)
            {
                if (object->testIntersection(currentRay, intersectionPoint, localNormal, localColor))
                {
                    // compute intensity of illumination
                    float intensity = 0.0;
                    QColor color;
                    bool illuminated = false;

                    for (auto light : m_lightObjects)
                    {
                        illuminated |= light->computeIllumination(intersectionPoint, localNormal, m_geometryObjects, object, color, intensity);
                    }

                    // compute the distance between camera and intersection point
                    const float distance = (intersectionPoint - currentRay.startPoint()).length();
                    if (distance > maxDistance)
                    {
                        maxDistance = distance;
                    }

                    if (distance < minDistance)
                    {
                        minDistance = distance;
                    }

                    if (illuminated)
                    {
                        outputImg.setPixelColor(x, y, QColor(localColor.red() * intensity,
                                                             localColor.green() * intensity,
                                                             localColor.blue() * intensity));
                    }
                    else
                    {
                        //outputImg.setPixelColor(x, y, Qt::black);
                    }
                }
                else {
                    //outputImg.setPixelColor(x, y, Qt::black);
                }
            }
        }
    }

    return true;
}
