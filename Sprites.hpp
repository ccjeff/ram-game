#include <glm/glm.hpp>
#include "GL.hpp"

#include <vector>

struct Transform{
    glm::vec2 displacement;
    // Counterclockwise rotation in radians
    float rotation;
}

struct Picture{
    std::vector< glm::u8vec4 > &data;
    glm::uvec2 size;
}

struct Sprite {
    Sprite(std::vector< glm::u8vec4 > data, glm::uvec2 size, Transform transform={{0.f,0.f},0.f});
    ~Sprite();
    GLuint tex = 0;
    //indicates a transform w.r.t. the containing class
    Transform transform;
    Picture picture;
    glm::uvec4 tint;

    void draw();
};
