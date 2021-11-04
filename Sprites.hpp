#pragma once
#include <glm/glm.hpp>
#include "GL.hpp"
#include "ColorTextureProgram.hpp"
#include <glm/gtx/rotate_vector.hpp>

#include <vector>

//transform order: position + rotation * scale * pt
struct Transform{
    // the location of the sprite when drawn, in world units
    glm::vec2 displacement;
    // Counterclockwise rotation in radians
    float rotation = 0.f;
    // the length/width of the sprite when drawn, in world units
    glm::vec2 size = {1.f, 1.f};
};

struct Picture{

    std::vector< glm::u8vec4 > data;
    glm::uvec2 size;
    // Picture();
};

struct Sprite {
    Sprite(std::vector< glm::u8vec4 > data, glm::uvec2 size, Transform transform = {{0.f, 0.f}, 0.f, {16.f, 16.f}});
    ~Sprite();
    Sprite();
    struct Vertex {
        Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
            Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
        glm::vec3 Position;
        glm::u8vec4 Color;
        glm::vec2 TexCoord;
    };
    static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "Sprites::Vertex should be packed");
    //indicates a transform w.r.t. the containing class
    GLuint tex = 0;
    Transform transform;
    Picture picture;
    glm::uvec4 tint;

    void draw(glm::vec2 camera_center,
        ColorTextureProgram &color_texture_program,
        GLuint vertex_buffer_for_color_texture_program,
        GLuint vertex_buffer);
};
