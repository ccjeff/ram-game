#pragma once
#include <glm/glm.hpp>
#include "GL.hpp"
#include "ColorTextureProgram.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "Load.hpp"

#include <vector>
#include <unordered_map>
#include <string>

//transform order: position + rotation * scale * pt
struct Transform{
    // the location of the sprite when drawn, in world units
    glm::vec2 displacement;
    // Counterclockwise rotation in radians
    float rotation = 0.f;
    // the length/width of the sprite when drawn, in world units
    glm::vec2 size = {1.f, 1.f};
};

struct TexRectangle{
    TexRectangle();
    TexRectangle(float _x0, float _y0, float _x1, float _y1);
    float x0;
    float y0;
    float x1;
    float y1;
};

struct SpriteMap {
    // Figure out how to fix the sprite map
    //SpriteMap(GLuint tex, glm::uvec2 size, Transform transform = {{0.f, 0.f}, 0.f, {16.f, 16.f}});
    ~SpriteMap();
    SpriteMap();
    GLuint tex = 0;
    std::unordered_map<std::string, TexRectangle> sprites;
};

struct Sprite {
    Sprite(const SpriteMap &s_map, const std::string &s_name, Transform transform = {{0.f, 0.f}, 0.f, {1.f, 1.f}});
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
    TexRectangle tex_coords;
    Transform transform;
    glm::uvec4 tint = {255, 255, 255, 255};

    void draw(glm::vec2 camera_center,
        ColorTextureProgram &color_texture_program,
        GLuint vertex_buffer_for_color_texture_program,
        GLuint vertex_buffer);
};

extern Load<SpriteMap> black;
extern Load<SpriteMap> green_tile;
extern Load<SpriteMap> green_smiley;
extern Load<SpriteMap> red_smiley;
extern Load<SpriteMap> green_circle;
extern Load<SpriteMap> red_circle;
