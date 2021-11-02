#pragma once
#include <glm/glm.hpp>

#include <vector>

struct Transform{
    glm::vec2 displacement;
    // Counterclockwise rotation in radians
    float rotation;
};

struct Picture{
    std::vector< glm::u8vec4 > &data;
    glm::uvec2 size;
};

struct Sprite {
    //indicates a transform w.r.t. the containing class
    Transform transform;
    Picture picture;
    glm::u8vec4 tint;
    Sprite();
    void draw();
};
