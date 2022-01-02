#include "DlnSimpleMaterial.h"

#include "primitives/DlnGeometryObject.h"
#include "lights/DlnLightObject.h"

#include <QtMath>

using namespace  dln;

DlnSimpleMaterial::DlnSimpleMaterial() {}

DlnSimpleMaterial::~DlnSimpleMaterial() {}

void DlnSimpleMaterial::setColor(const DlnColor &color)
{
    m_baseColor = color;
}

void DlnSimpleMaterial::setReflictivity(float reflictivity)
{
    m_reflictivity = reflictivity;
}

void DlnSimpleMaterial::setShininess(float shininess)
{
    m_shininess = shininess;
}

DlnColor DlnSimpleMaterial::computeColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                         const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                         const QSharedPointer<DlnGeometryObject> &currentObject,
                                         const DlnRay &castRay,
                                         const QVector3D &intersectionPoint,
                                         const QVector3D &localNormal)
{
    DlnColor materialColor;
    DlnColor reflectionlColor;
    DlnColor diffuseColor;
    DlnColor specularColor;

    // compute the diffuse component
    diffuseColor = computeDiffuseColor(geometryObjects, lightObjects, currentObject, intersectionPoint, localNormal, m_baseColor);

    // compute the specular component
    if (m_shininess > 0.0)
    {
        specularColor = computeSpecular(geometryObjects, lightObjects, castRay, intersectionPoint, localNormal);
    }

    // add specular component to the final color
    materialColor = diffuseColor + specularColor;

    return materialColor;
}

DlnColor DlnSimpleMaterial::computeSpecular(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                            const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                            const DlnRay &castRay,
                                            const QVector3D &intersectionPoint,
                                            const QVector3D &localNormal)
{
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    for (auto currentLight : lightObjects)
    {
        // check for intersections with all objects in the scene
        float intensity = 0.0;

        // construct a vector pointing from the intersection point to the light
        QVector3D lightDirection = (currentLight->position() - intersectionPoint).normalized();

        // compute a start point
        QVector3D startPoint = intersectionPoint + lightDirection * 0.001;

        // construct a ray from the intersection point to the light
        DlnRay lightRay(startPoint, startPoint + lightDirection);

        // loop through all the objects in the scene to check  if any
        // obstruct light from this source
        QVector3D poi;
        QVector3D poiNormal;
        DlnColor poiColor;
        bool validIntersection = false;

        for (auto currentObject : geometryObjects)
        {
            validIntersection = currentObject->testIntersection(lightRay, poi, poiNormal, poiColor);
            if (validIntersection)
            {
                break;
            }
        }

        // if no intersection were found then proceed with computing the specular component
        if (!validIntersection)
        {
            // compute the reflection vector
            QVector3D d = lightRay.direction();
            QVector3D r = d - 2 * QVector3D::dotProduct(d, localNormal) * localNormal;
            r.normalize();

            // compute the dot product
            QVector3D v = castRay.direction();
            v.normalize();

            float dotProduct = QVector3D::dotProduct(r, v);
            if (dotProduct > 0.0)
            {
                intensity = m_reflictivity * qPow(dotProduct, m_shininess);
            }
        }

        red += currentLight->color().red() * intensity;
        green += currentLight->color().green() * intensity;
        blue += currentLight->color().blue() * intensity;
        // TODO: we can loose color
    }

    return DlnColor(red, green, blue);
}
