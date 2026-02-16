#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "boiler.hpp"

class BlackHole {
public:
    BlackHole(float mass, glm::vec3 position) : m_mass(mass), m_position(position) {
        m_radius_s = 2 * Constants::G * m_mass / (Constants::c * Constants::c);
    }

    float GetMass() const { return m_mass; }
    glm::vec3 GetPosition() const { return m_position; }
    float GetRadius() const { return m_radius_s;}

    void SetMass(float mass) { 
        m_mass = mass; 
        m_radius_s = 2 * Constants::G * m_mass / (Constants::c * Constants::c);
    }
    void SetPosition(glm::vec3 position) { m_position = position; }


private:
    float m_mass;
    float m_radius_s;
    glm::vec3 m_position;
};
#endif