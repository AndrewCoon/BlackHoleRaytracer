#ifndef CAMERA_H
#define CAMERA_H

#include "boiler.hpp"
#include "ray.h"

class Camera {
public:
    Camera();
    
    // Generate a ray for pixel coordinates (i, j) in image of size (width, height)
    Ray GetRay(float u, float v) const;
    
    // Accessors
    glm::vec3 GetOrigin() const { return m_Origin; }
    
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