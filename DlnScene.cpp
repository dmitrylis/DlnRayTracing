#include "DlnScene.h"

#include "primitives/DlnSphere.h"
#include "primitives/DlnPlane.h"
#include "lights/DlnPointLight.h"
#include "materials/DlnSimpleMaterial.h"

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

    // create some material
    QSharedPointer<DlnSimpleMaterial> simpleMaterial(new DlnSimpleMaterial());
    simpleMaterial->setColor(DlnColor(Qt::gray));
    simpleMaterial->setReflictivity(0.5);
    simpleMaterial->setShininess(10.0);

    QSharedPointer<DlnSimpleMaterial> simpleMaterial2(new DlnSimpleMaterial());
    simpleMaterial2->setColor(DlnColor(Qt::red));
    simpleMaterial2->setReflictivity(0.5);
    simpleMaterial2->setShininess(10.0);

    QSharedPointer<DlnSimpleMaterial> simpleMaterial3(new DlnSimpleMaterial());
    simpleMaterial3->setColor(DlnColor(Qt::red));
    simpleMaterial3->setReflictivity(0.5);
    simpleMaterial3->setShininess(10.0);

    // create test objects
    QSharedPointer<DlnSphere> sphere1(new DlnSphere());
    sphere1->setMaterial(simpleMaterial);
    sphere1->setBaseColor(DlnColor(Qt::red));
    m_geometryObjects.push_back(sphere1);

    QSharedPointer<DlnSphere> sphere2(new DlnSphere());
    DlnTransform sphere2Transform;
    sphere2Transform.setTransform(QVector3D(-1.5, 0.0, 0.5), QVector3D(0.0, 0.0, 0.0), QVector3D(0.5, 0.5, 0.5));
    sphere2->setTransform(sphere2Transform);
    sphere2->setBaseColor(DlnColor(Qt::blue));
    sphere2->setMaterial(simpleMaterial3);
    m_geometryObjects.push_back(sphere2);

    QSharedPointer<DlnSphere> sphere3(new DlnSphere());
    DlnTransform sphere3Transform;
    sphere3Transform.setTransform(QVector3D(1.5, 0.0, 0.5), QVector3D(0.0, 0.0, 0.0), QVector3D(0.5, 0.5, 0.5));
    sphere3->setTransform(sphere3Transform);
    sphere3->setBaseColor(DlnColor(Qt::blue));
    sphere3->setMaterial(simpleMaterial3);
    m_geometryObjects.push_back(sphere3);

    // create white box
    QSharedPointer<DlnPlane> plane1(new DlnPlane());
    DlnTransform plane1Transform;
    plane1Transform.setTransform(QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 0.0, 0.0), QVector3D(2.5, 3.5, 2.5));
    plane1->setTransform(plane1Transform);
    plane1->setBaseColor(DlnColor(Qt::red));
    plane1->setMaterial(simpleMaterial2);
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
    light1->setColor(DlnColor(Qt::white));
    light1->setPosition(QVector3D(-10.0, -10.0, -10.0));
    m_lightObjects.push_back(light1);

//    QSharedPointer<DlnPointLight> light2(new DlnPointLight());
//    light2->setColor(DlnColor(Qt::white));
//    light2->setPosition(QVector3D(0.0, -10.0, -10.0));
//    m_lightObjects.push_back(light2);

    QSharedPointer<DlnPointLight> light3(new DlnPointLight());
    light3->setColor(DlnColor(Qt::white));
    light3->setPosition(QVector3D(10.0, -10.0, -10.0));
    m_lightObjects.push_back(light3);
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

    const float xFactor = 1.0 / (static_cast<float>(width) / 2.0);
    const float yFactor = 1.0 / (static_cast<float>(height) / 2.0);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // normalize x and y
            const float normX = static_cast<float>(x) * xFactor - 1.0;
            const float normY = static_cast<float>(y) * yFactor - 1.0;

            // generate ray
            m_camera.generateRay(normX, normY, cameraRay);

            // test for intersections with all objects in the scene
            QSharedPointer<DlnGeometryObject> object;
            QVector3D intersectionPoint;
            QVector3D localNormal;
            DlnColor localColor; // TODO: remove

            // compute the illumination of the closest object
            if (castRay(cameraRay, object, intersectionPoint, localNormal, localColor))
            {
                DlnMaterialObject::m_reflectionRayCount = 0;
                const DlnColor color = object->computeColor(m_geometryObjects,
                                                            m_lightObjects,
                                                            object, // stupid thing
                                                            cameraRay,
                                                            intersectionPoint,
                                                            localNormal);

                outputImg.setWidePixelColor(x, y, color.red(), color.green(), color.blue());
            }
        }
    }

    return true;
}

bool DlnScene::castRay(const DlnRay &castRay,
                       QSharedPointer<DlnGeometryObject> &object,
                       QVector3D &intersectionPoint,
                       QVector3D &localNormal,
                       DlnColor &localColor)
{
    QVector3D closestIntersectionPoint;
    QVector3D closestLocalNormal;
    DlnColor closestLocalColor;
    bool intersectionFound = false;
    float minDistance = std::numeric_limits<float>::max();

    for (auto currentObject : m_geometryObjects)
    {
        if (currentObject->testIntersection(castRay, closestIntersectionPoint, closestLocalNormal, closestLocalColor))
        {
            intersectionFound = true;

            // compute the distance between the camera and the point of intersection
            const float distance = (closestIntersectionPoint - castRay.startPoint()).length();

            // if this object is the closest to the camera - store the reference
            if (distance < minDistance)
            {
                minDistance = distance;

                object = currentObject;
                intersectionPoint = closestIntersectionPoint;
                localNormal = closestLocalNormal;
                localColor = closestLocalColor;
            }
        }
    }

    return intersectionFound;
}
