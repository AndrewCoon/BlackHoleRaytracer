#include "framebuffer.h"
#include <algorithm>

FrameBuffer::FrameBuffer(int width, int height) 
    : mWidth(width), 
    mHeight(height) 
{
    mPixels.resize(width * height);
    Clear();
}

void FrameBuffer::SetPixel(int x, int y, const glm::vec3& color) {
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) return;

    glm::vec3 clamped = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
    mPixels[GetIndex(x, y)] = clamped;
}

glm::vec3 FrameBuffer::GetPixel(int x, int y) const {
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) return glm::vec3(0.0f);
    return mPixels[GetIndex(x, y)];
}

void FrameBuffer::Clear(const glm::vec3& color) {
    std::fill(mPixels.begin(), mPixels.end(), color);
}

const float* FrameBuffer::GetData() const {
    return &mPixels[0].x;
}

int FrameBuffer::GetIndex(int x, int y) const {
    return y * mWidth + x;
}