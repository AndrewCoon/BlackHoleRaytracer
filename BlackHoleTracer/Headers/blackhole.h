#include "boiler.hpp"

class BlackHole {
public:
    BlackHole(double mass, glm::dvec3 position) : m_mass(mass), m_position(position) {
        m_radius_s = 2 * Constants::G * m_mass / (Constants::c * Constants::c);
    }

    double getMass() const { return m_mass; }
    glm::dvec3 getPosition() const { return m_position; }
    double getRadius() const { return m_radius_s;}

private:
    double m_mass;
    double m_radius_s;
    glm::dvec3 m_position;
};