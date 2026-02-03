#pragma once
#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "boiler.hpp"

class FrameBuffer {
    public: 
        FrameBuffer(int width, int height);

        void SetPixel(int x, int y, const glm::vec3& color);
        glm::vec3 GetPixel(int x, int y) const;
        void Clear(const glm::vec3& color = glm::vec3(0.0f));

        const float* GetData() const;

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }

    private:
        int mWidth;
        int mHeight;
        std::vector<glm::vec3> mPixels;

        int GetIndex(int x, int y) const;
};
#endif 