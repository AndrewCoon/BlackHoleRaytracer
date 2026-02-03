#ifndef RAY_H
#define RAY_H

#include "boiler.hpp"

class Ray {
    public:
        Ray() = default;
        Ray(const glm::vec3& origin, const glm::vec3& direction) : origin(origin), dir(direction), color(glm::vec3(0.0f, 0.0f, 0.0f)) { }; 

        const glm::vec3& GetOrigin() const { return origin; }
        const glm::vec3& GetDirection() const { return dir; }
        glm::vec3& GetColor() { return color; }

        static glm::vec3 RayColor(const Ray& r) {
            glm::vec3 unit_dir = glm::normalize(r.dir);
            float t = 0.5f * (unit_dir.y + 1.0f);
            return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
        }

        glm::vec3 at(float t) const {
            return origin + t * dir;
        }

    private:
        glm::vec3 origin;
        glm::vec3 dir;
        glm::vec3 color;
};
#endif // RAY_H