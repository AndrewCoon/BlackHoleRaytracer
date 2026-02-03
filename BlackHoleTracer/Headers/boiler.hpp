// Preprocessor Directives
#ifndef BOILER
#define BOILER
#pragma once

// Headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

namespace Config {
    // Window Settings
    constexpr int WINDOW_WIDTH = 512;
    constexpr float ASPECT_RATIO = 16.0f / 9.0f;
    constexpr int WINDOW_HEIGHT = (int)(WINDOW_WIDTH / ASPECT_RATIO);

    constexpr const char* WINDOW_TITLE = "Black Hole Raytracer";

    // Viewport Settings
    constexpr float VIEWPORT_HEIGHT = 2.0f;
    constexpr float VIEWPORT_WIDTH = VIEWPORT_HEIGHT * WINDOW_WIDTH / WINDOW_HEIGHT;
}

namespace RayConfig {
    constexpr int MAX_BOUNCES = 5;
    constexpr float EPSILON = 0.0001f;
}
#endif // Header
