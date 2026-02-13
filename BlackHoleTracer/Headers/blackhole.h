#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "boiler.hpp"

class BlackHole {
public:
    BlackHole(double mass, glm::dvec3 position) : m_mass(mass), m_position(position) {
        m_radius_s = 2 * Constants::G * m_mass / (Constants::c * Constants::c);
    }

    double GetMass() const { return m_mass; }
    glm::dvec3 GetPosition() const { return m_position; }
    double GetRadius() const { return m_radius_s;}

private:
    double m_mass;
    double m_radius_s;
    glm::dvec3 m_position;
};
#endif