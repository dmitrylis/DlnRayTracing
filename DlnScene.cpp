#include "DlnScene.h"

#include "primitives/DlnSphere.h"
#include "primitives/DlnPlane.h"
#include "lights/DlnPointLight.h"

#include <limits>

using namespace dln;

DlnScene::DlnScene() {
    // setup camera
    m_camera.setPosition(QVector3D(0.0, -11.0, -1.0));
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
    DlnTransform sphere2Transform;
    sphere2Transform.setTransform(QVector3D(-1.5, 0.0, 1.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.5, 0.5, 1.0));
    sphere2->setTransform(sphere2Transform);
    sphere2->setBaseColor(Qt::green);
    m_geometryObjects.push_back(sphere2);

    QSharedPointer<DlnSphere> sphere3(new DlnSphere());
    DlnTransform sphere3Transform;
    sphere3Transform.setTransform(QVector3D(1.5, -1.0, 0.2), QVector3D(0.0, 0.0, 0.0), QVector3D(0.5, 0.5, 1.0));
    sphere3->setTransform(sphere3Transform);
    sphere3->setBaseColor(Qt::blue);
    m_geometryObjects.push_back(sphere3);

    // create white box
    QSharedPointer<DlnPlane> plane1(new DlnPlane());
    DlnTransform plane1Transform;
    plane1Transform.setTransform(QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 0.0, 0.0), QVector3D(2.5, 3.5, 2.5));
    plane1->setTransform(plane1Transform);
    plane1->setBaseColor(Qt::white);
    m_geometryObjects.push_back(plane1);

    //QSharedPointer<DlnPlane> plane2(new DlnPlane());
    //DlnTransform plane2Transform;
    //plane2Transform.setTransform(QVector3D(2.5, 0.0, 0.0), QVector3D(0.0, 1.57, 0.0), QVector3D(2.5, 3.5, 2.5));
    //plane2->setTransform(plane2Transform);
    //plane2->setBaseColor(Qt::white);
    //m_geometryObjects.push_back(plane2);
    //
    //QSharedPointer<DlnPlane> plane3(new DlnPlane());
    //DlnTransform plane3Transform;
    //plane3Transform.setTransform(QVector3D(-2.5, 0.0, 0.0), QVector3D(0.0, -1.57, 0.0), QVector3D(2.5, 3.5, 2.5));
    //plane3->setTransform(plane3Transform);
    //plane3->setBaseColor(Qt::white);
    //m_geometryObjects.push_back(plane3);
    //
    //QSharedPointer<DlnPlane> plane4(new DlnPlane());
    //DlnTransform plane4Transform;
    //plane4Transform.setTransform(QVector3D(0.0, 3.0, 0.0), QVector3D(-1.57, 0.0, 0.0), QVector3D(2.5, 3.5, 2.5));
    //plane4->setTransform(plane4Transform);
    //plane4->setBaseColor(Qt::white);
    //m_geometryObjects.push_back(plane4);

    // create test lights
    QSharedPointer<DlnPointLight> light1(new DlnPointLight());
    light1->setColor(Qt::white);
    light1->setPosition(QVector3D(5.0, -10.0, -5.0));
    m_lightObjects.push_back(light1);

    QSharedPointer<DlnPointLight> light2(new DlnPointLight());
    light2->setColor(Qt::white);
    light2->setPosition(QVector3D(-5.0, -15.0, -15.0));
    m_lightObjects.push_back(light2);
}

void DlnScene::initialize(const QSizeF &viewSize)
{
    // setup camera
    m_camera.setAspectRatio(viewSize.width() / viewSize.height());
    m_camera.updateCameraGeometry();
}

bool DlnScene::render(DlnImage &outputImg)
{
    const int width = outputImg.width();
    const int height = outputImg.height();

    DlnRay cameraRay;
    QVector3D intersectionPoint;
    QVector3D localNormal;
    QColor localColor;

    const float xFactor = 1.0 / (static_cast<float>(width) / 2.0);
    const float yFactor = 1.0 / (static_cast<float>(height) / 2.0);

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            // normalize x and y
            const float normX = static_cast<float>(x) * xFactor - 1.0;
            const float normY = static_cast<float>(y) * yFactor - 1.0;

            // generate ray
            m_camera.generateRay(normX, normY, cameraRay);

            // test for intersections with all objects in the scene
            QSharedPointer<DlnGeometryObject> closestObject;
            QVector3D closestIntersectionPoint;
            QVector3D closestLocalNormal;
            QColor closestLocalColor;
            bool intersectionFound = false;
            float minDistance = std::numeric_limits<float>::max();

            for (auto currentObject : m_geometryObjects)
            {
                if (currentObject->testIntersection(cameraRay, intersectionPoint, localNormal, localColor))
                {
                    intersectionFound = true;

                    // compute the distance between the camera and the point of intersection
                    const float distance = (intersectionPoint - cameraRay.startPoint()).length();

                    // if this object is the closest to the camera - store the reference
                    if (distance < minDistance)
                    {
                        minDistance = distance;

                        closestObject = currentObject;
                        closestIntersectionPoint = intersectionPoint;
                        closestLocalNormal = localNormal;
                        closestLocalColor = localColor;
                    }
                }
            }

            // compute the illumination of the closest object
            if (intersectionFound)
            {
                // compute the intensity of illumination
                float intensity = 0.0;
                QColor color;
                float red = 0.0;
                float green = 0.0;
                float blue = 0.0;
                bool illuminated = false;
                bool illuminationFound = false;

                for (auto currentLight : m_lightObjects)
                {
                    illuminated = currentLight->computeIllumination(closestIntersectionPoint,
                                                                    closestLocalNormal,
                                                                    m_geometryObjects,
                                                                    closestObject,
                                                                    color,
                                                                    intensity);

                    if (illuminated)
                    {
                        illuminationFound = true;

                        red += color.redF() * intensity;
                        green += color.greenF() * intensity;
                        blue += color.blueF() * intensity;
                    }
                }

                if (illuminationFound)
                {
                    red *= closestLocalColor.redF();
                    green *= closestLocalColor.greenF();
                    blue *= closestLocalColor.blueF();

                    outputImg.setWidePixelColor(x, y, red, green, blue);
                }
            }
        }
    }

    return true;
}
