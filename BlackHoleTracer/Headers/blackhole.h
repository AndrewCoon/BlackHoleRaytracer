#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "boiler.hpp"

class BlackHole {
public:
    BlackHole(float mass, glm::vec3 position) : m_mass(mass), m_position(position) { }

    float& Mass() { return m_mass; }
    glm::vec3& Position() { return m_position; }

    float Radius() const {
        return 2.0f * Constants::G * m_mass / (Constants::c * Constants::c);
    }
private:
    float m_mass;
    glm::vec3 m_position;
};
#endif