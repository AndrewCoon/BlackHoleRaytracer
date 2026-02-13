#ifndef CAMERA_H
#define CAMERA_H

#include "boiler.hpp"
#include "ray.h"

class Camera {
public:
    Camera(const glm::vec3 pos);
    
    // Generate a ray for pixel coordinates (i, j) in image of size (width, height)
    Ray GetRay(float u, float v) const;
    
    // Accessors
    glm::vec3 GetOrigin() const { return m_Origin; }
    glm::vec3 GetUpperLeftCorner() const { return m_UpperLeftCorner; }
    glm::vec3 GetHorizontal() const { return m_Horizontal; }
    glm::vec3 GetVertical() const { return m_Vertical; }
    glm::vec3 GetPixelDeltaU() const { return m_pixel_delta_u; }
    glm::vec3 GetPixelDeltaV() const { return m_pixel_delta_v; }
    glm::vec3 GetPixel00Location() const { return m_pixel00_loc; }
    
private:
    // Camera parameters
    glm::vec3 m_Origin;
    glm::vec3 m_UpperLeftCorner;    
    glm::vec3 m_Horizontal;
    glm::vec3 m_Vertical;

    // U is horizontal V is vertical
    glm::vec3 m_pixel_delta_u;
    glm::vec3 m_pixel_delta_v;
    glm::vec3 m_pixel00_loc;
};

#endif