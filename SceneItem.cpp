#include "SceneItem.h"

#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>
#include <QSGTexture>
#include <QtGlobal>
#include <QtMath>

namespace {
constexpr int WIDTH = 400;
constexpr int HEIGHT = 300;
} // namespace

#include <algorithm>
#include <fstream>
#include <limits>
#include <vector>

#include <cassert>
#include <cmath>
#include <iostream>

template <size_t DIM> struct vec {
  float &operator[](const size_t i) {
    assert(i < DIM);
    return data[i];
  }
  const float &operator[](const size_t i) const {
    assert(i < DIM);
    return data[i];
  }
  float data[DIM] = {};
};

template <size_t DIM> vec<DIM> operator*(const vec<DIM> &lhs, const float rhs) {
  vec<DIM> ret;
  for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs)
    ;
  return ret;
}

template <size_t DIM>
float operator*(const vec<DIM> &lhs, const vec<DIM> &rhs) {
  float ret = 0;
  for (size_t i = DIM; i--; ret += lhs[i] * rhs[i])
    ;
  return ret;
}

template <size_t DIM> vec<DIM> operator+(vec<DIM> lhs, const vec<DIM> &rhs) {
  for (size_t i = DIM; i--; lhs[i] += rhs[i])
    ;
  return lhs;
}

template <size_t DIM> vec<DIM> operator-(vec<DIM> lhs, const vec<DIM> &rhs) {
  for (size_t i = DIM; i--; lhs[i] -= rhs[i])
    ;
  return lhs;
}

template <size_t DIM> vec<DIM> operator-(const vec<DIM> &lhs) {
  return lhs * (-1.f);
}

template <> struct vec<3> {
  float &operator[](const size_t i) {
    assert(i < 3);
    return i == 0 ? x : (1 == i ? y : z);
  }
  const float &operator[](const size_t i) const {
    assert(i < 3);
    return i == 0 ? x : (1 == i ? y : z);
  }
  float norm() { return std::sqrt(x * x + y * y + z * z); }
  vec<3> &normalize(float l = 1) {
    *this = (*this) * (l / norm());
    return *this;
  }
  float x = 0, y = 0, z = 0;
};

typedef vec<3> vec3;
typedef vec<4> vec4;

vec3 cross(vec3 v1, vec3 v2) {
  return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
          v1.x * v2.y - v1.y * v2.x};
}

template <size_t DIM>
std::ostream &operator<<(std::ostream &out, const vec<DIM> &v) {
  for (size_t i = 0; i < DIM; i++)
    out << v[i] << " ";
  return out;
}

struct Light {
  vec3 position;
  float intensity;
};

struct Material {
  float refractive_index = 1;
  vec4 albedo = {1, 0, 0, 0};
  vec3 diffuse_color = {0, 0, 0};
  float specular_exponent = 0;
};

struct Sphere {
  vec3 center;
  float radius;
  Material material;
};

bool ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s,
                          float &t0) {
  vec3 L = s.center - orig;
  float tca = L * dir;
  float d2 = L * L - tca * tca;
  if (d2 > s.radius * s.radius)
    return false;
  float thc = sqrtf(s.radius * s.radius - d2);
  t0 = tca - thc;
  float t1 = tca + thc;
  if (t0 < 1e-3)
    t0 =
        t1; // offset the original point to avoid occlusion by the object itself
  if (t0 < 1e-3)
    return false;
  return true;
}

vec3 reflect(const vec3 &I, const vec3 &N) { return I - N * 2.f * (I * N); }

vec3 refract(const vec3 &I, const vec3 &N, const float eta_t,
             const float eta_i = 1.f) { // Snell's law
  float cosi = -std::max(-1.f, std::min(1.f, I * N));
  if (cosi < 0)
    return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the
                                         // object, swap the air and the media
  float eta = eta_i / eta_t;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  return k < 0 ? vec3{1, 0, 0}
               : I * eta +
                     N * (eta * cosi -
                          std::sqrt(k)); // k<0 = total reflection, no ray to
                                         // refract. I refract it anyways, this
                                         // has no physical meaning
}

bool scene_intersect(const vec3 &orig, const vec3 &dir,
                     const std::vector<Sphere> &spheres, vec3 &hit, vec3 &N,
                     Material &material) {
  float spheres_dist = std::numeric_limits<float>::max();
  for (const Sphere &s : spheres) {
    float dist_i;
    if (ray_sphere_intersect(orig, dir, s, dist_i) && dist_i < spheres_dist) {
      spheres_dist = dist_i;
      hit = orig + dir * dist_i;
      N = (hit - s.center).normalize();
      material = s.material;
    }
  }

  float checkerboard_dist = std::numeric_limits<float>::max();
  if (std::abs(dir.y) > 1e-3) { // avoid division by zero
    float d =
        -(orig.y + 4) / dir.y; // the checkerboard plane has equation y = -4
    vec3 pt = orig + dir * d;
    if (d > 1e-3 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 &&
        d < spheres_dist) {
      checkerboard_dist = d;
      hit = pt;
      N = vec3{0, 1, 0};
      material.diffuse_color = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1
                                   ? vec3{.3, .3, .3}
                                   : vec3{.3, .2, .1};
    }
  }
  return std::min(spheres_dist, checkerboard_dist) < 1000;
}

vec3 cast_ray(const vec3 &orig, const vec3 &dir,
              const std::vector<Sphere> &spheres,
              const std::vector<Light> &lights, size_t depth = 0) {
  vec3 point, N;
  Material material;

  if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, material))
    return vec3{0.2, 0.7, 0.8}; // background color

  vec3 reflect_dir = reflect(dir, N).normalize();
  vec3 refract_dir = refract(dir, N, material.refractive_index).normalize();
  vec3 reflect_color = cast_ray(point, reflect_dir, spheres, lights, depth + 1);
  vec3 refract_color = cast_ray(point, refract_dir, spheres, lights, depth + 1);

  float diffuse_light_intensity = 0, specular_light_intensity = 0;
  for (const Light light : lights) {
    vec3 light_dir = (light.position - point).normalize();

    vec3 shadow_pt, trashnrm;
    Material trashmat;
    if (scene_intersect(point, light_dir, spheres, shadow_pt, trashnrm,
                        trashmat) &&
        (shadow_pt - point).norm() <
            (light.position - point).norm()) // checking if the point lies in
                                             // the shadow of the light
      continue;

    diffuse_light_intensity += light.intensity * std::max(0.f, light_dir * N);
    specular_light_intensity +=
        std::pow(std::max(0.f, -reflect(-light_dir, N) * dir),
                 material.specular_exponent) *
        light.intensity;
  }
  return material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
         vec3{1., 1., 1.} * specular_light_intensity * material.albedo[1] +
         reflect_color * material.albedo[2] +
         refract_color * material.albedo[3];
}

SceneItem::SceneItem(QQuickItem *parent) : QQuickItem(parent) {
  setFlag(QQuickItem::ItemHasContents);

  setSize(QSize(::WIDTH, ::HEIGHT));
  m_Img = QImage(::WIDTH, ::HEIGHT, QImage::Format_RGB32);

  // test draw code
  const Material ivory = {1.0, {0.6, 0.3, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.};
  const Material glass = {1.5, {0.0, 0.5, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.};
  const Material red_rubber = {1.0, {0.9, 0.1, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.};
  const Material mirror = {1.0, {0.0, 10.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

  std::vector<Sphere> spheres = {Sphere{vec3{-3, 0, -16}, 2, ivory},
                                 Sphere{vec3{-1.0, -1.5, -12}, 2, glass},
                                 Sphere{vec3{1.5, -0.5, -18}, 3, red_rubber},
                                 Sphere{vec3{7, 5, -18}, 4, mirror}};

  std::vector<Light> lights = {
      {{-20, 20, 20}, 1.5}, {{30, 50, -25}, 1.8}, {{30, 20, 30}, 1.7}};

  const float fov = M_PI / 3.;

  for (int y = 0; y < ::HEIGHT; ++y) {
    for (int x = 0; x < ::WIDTH; ++x) {

      QColor pixelColor = Qt::white;

      //      int cX = 400;
      //      int cY = 300;
      //      int cRad = 200;

      //      if (qPow(x - cX, 2) + qPow(y - cY, 2) < qPow(cRad, 2)) {
      //        pixelColor = Qt::black;
      //      }

      //      m_Img.setPixelColor(x, y, pixelColor);

      float dir_x = (x + 0.5) - ::WIDTH / 2.;
      float dir_y =
          -(y + 0.5) + ::HEIGHT / 2.; // this flips the image at the same time
      float dir_z = -::HEIGHT / (2. * tan(fov / 2.));
      vec3 color =
          cast_ray(vec3{0, 0, 0}, vec3{dir_x, dir_y, dir_z}.normalize(),
                   spheres, lights);
      pixelColor.setRed(qBound(0, static_cast<int>(color.x * 255), 255));
      pixelColor.setGreen(qBound(0, static_cast<int>(color.y * 255), 255));
      pixelColor.setBlue(qBound(0, static_cast<int>(color.z * 255), 255));
      m_Img.setPixelColor(x, y, pixelColor);
    }
  }
}

void SceneItem::updateFrame(const QImage &img) {
  do {
    QMutexLocker locker(&m_imgMutex);
    m_Img = img.copy();
  } while (false);
  update();
}

QSGNode *SceneItem::updatePaintNode(QSGNode *old, UpdatePaintNodeData *) {
  QMutexLocker locker(&m_imgMutex);
  if (m_Img.isNull()) {
    return old;
  }

  QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(old);
  if (node == nullptr) {
    node = new QSGSimpleTextureNode();
  }

  QSGTexture *t = window()->createTextureFromImage(
      m_Img.scaled(boundingRect().size().toSize()));

  if (t) {
    QSGTexture *tt = node->texture();
    if (tt) {
      tt->deleteLater();
    }
    node->setRect(boundingRect());
    node->setTexture(t);
  }

  return node;
}
