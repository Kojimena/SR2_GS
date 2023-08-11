#pragma once
#include "color.h"

struct Vertex {
    glm::vec3 position;
    Color color;
};

struct Fragment {
    glm::ivec2 position;
    Color color;
};