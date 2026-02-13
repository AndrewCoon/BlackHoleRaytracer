#include "camera.h"

Camera::Camera(const glm::vec3 pos) {
    m_Origin = pos;
    m_Horizontal = glm::vec3(ViewportConfig::VIEWPORT_WIDTH, 0.0f, 0.0f);
    m_Vertical = glm::vec3(0.0f, ViewportConfig::VIEWPORT_HEIGHT, 0.0f);
    m_UpperLeftCorner = m_Origin - m_Horizontal / 2.0f - m_Vertical / 2.0f - glm::vec3(0.0f, 0.0f, CameraConfig::FOCAL_LENGTH);

    m_pixel_delta_u = m_Horizontal / (float)Config::WINDOW_WIDTH;
    m_pixel_delta_v = m_Vertical / (float)Config::WINDOW_HEIGHT;
    m_pixel00_loc = m_UpperLeftCorner + 0.5f * (m_pixel_delta_u + m_pixel_delta_v);
}

Ray Camera::GetRay(float u, float v) const {
    return Ray(m_Origin, m_UpperLeftCorner + u * m_pixel_delta_u + v * m_pixel_delta_v - m_Origin);
}