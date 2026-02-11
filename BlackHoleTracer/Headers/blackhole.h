#include "boiler.hpp"

class BlackHole {
public:
    BlackHole(float mass, glm::vec3 position) : m_mass(mass), m_position(position) {
        m_radius_s = 2 * Constants::G * m_mass / (Constants::c * Constants::c);
    }

    float getMass() const { return m_mass; }
    glm::vec3 getPosition() const { return m_position; }
    float getRadius() const { return m_radius_s;}

private:
    float m_mass;
    float m_radius_s;
    glm::vec3 m_position;
};