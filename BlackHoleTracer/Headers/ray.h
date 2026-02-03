#ifndef RAY_H
#define RAY_H

#include "boiler.hpp"

class Ray {
    public:
        Ray() { };
        Ray(const glm::vec3& origin, const glm::vec3& direction) : origin(origin), dir(direction) { }; 

        const glm::vec3& GetOrigin() const { return origin; }
        const glm::vec3& GetDirection() const { return dir; }

        glm::vec3 at(float t) {
            return origin + t * dir;
        }

    private:
        glm::vec3 origin;
        glm::vec3 dir;
};
#endif // RAY_H