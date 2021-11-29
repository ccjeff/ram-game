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

struct Animation {
    std::vector<TexRectangle> anim;
    std::vector<float> durations;
    glm::vec2 sprite_center;
    glm::vec2 sprite_radius;
};

struct SpriteMap {
    ~SpriteMap();
    SpriteMap();
    GLuint tex = 0;
    std::unordered_map<std::string, Animation> sprites;
};

struct Vertex {
    Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
        Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
    glm::vec3 Position;
    glm::u8vec4 Color;
    glm::vec2 TexCoord;
};

static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "Sprites::Vertex should be packed");

struct Sprite {
    Sprite(const SpriteMap &s_map, const std::string &s_name);
    Sprite();
    ~Sprite();
    GLuint tex = 0;
    TexRectangle tex_coords;

    void draw(glm::vec2 camera_center,
        glm::vec2 object_center,
        glm::vec2 size,
        float rotation,
        glm::u8vec4 tint,
        ColorTextureProgram &color_texture_program,
        GLuint vertex_buffer_for_color_texture_program,
        GLuint vertex_buffer);
};

extern Load<SpriteMap> black;
extern Load<SpriteMap> green_tile;
extern Load<SpriteMap> green_smiley;
extern Load<SpriteMap> red_smiley;
extern Load<SpriteMap> melee_enemy;
extern Load<SpriteMap> green_circle;
extern Load<SpriteMap> red_circle;
extern Load<SpriteMap> r_learning;
extern Load<SpriteMap> ray_tracing;
extern Load<SpriteMap> dijkstra;
extern Load<SpriteMap> p_np;
extern Load<SpriteMap> door_locked;
extern Load<SpriteMap> door_unlocked;
