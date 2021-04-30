#include "graphics/graphics_engine.hpp"

namespace GE {
GraphicsEngine::GraphicsEngine(unsigned short width, unsigned short height)
    : WIDTH{width}, HEIGHT{height} {}

GraphicsEngine::~GraphicsEngine() {}
}  // namespace GE