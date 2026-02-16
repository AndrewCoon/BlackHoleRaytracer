#ifndef CAMERA_H
#define CAMERA_H

#include "boiler.hpp"

class Camera {
public:
    Camera() {}
    Camera(float radius, float azimuth, float polar) : radius(radius), azimuth(azimuth), polar(polar) {}
    
    // Accessors
    float& Radius() { return radius; }
    float& Azimuth() { return azimuth; }
    float& Polar() { return polar; }
    float& Zoom() { return zoom; }

    glm::vec3 GetPosition() const {
        return glm::vec3(
            radius * sin(polar) * cos(azimuth),
            radius * cos(polar),
            radius * sin(polar) * sin(azimuth)
        );
    }

    // This creates the look-at rotation
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(GetPosition(), glm::vec3(0,0,0), glm::vec3(0,1,0));
    }
    
    // Mutators
    void SetRadius(float r) { radius = r; }
    void SetAzimuth(float a) { azimuth = a; }
    void SetPolar(float p) { polar = p; }
    
private:
    // Camera parameters
    float zoom = 90.0f;
    float radius = 10.0f;
    float azimuth = 0.0f;
    float polar = Constants::PI / 2.0f;
};

#endif