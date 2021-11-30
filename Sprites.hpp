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
    glm::vec2 translation = {0.f, 0.f};
    // the length/width of the sprite when drawn, in world units
    glm::vec2 scale = {1.f, 1.f};
};

struct TexRectangle{
    TexRectangle();
    TexRectangle(float _x0, float _y0, float _x1, float _y1);
    float x0;
    float y0;
    float x1;
    float y1;
};

struct Vertex {
    Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
        Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
    glm::vec3 Position;
    glm::u8vec4 Color;
    glm::vec2 TexCoord;
};

static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "Sprites::Vertex should be packed");

struct Animation {
    std::vector<TexRectangle> anim;
    std::vector<float> durations;
    Transform transform;
    size_t sprite_size;
    void draw(
        float elapsed,
        glm::vec2 camera_center,
        glm::vec2 object_center,
        glm::vec2 size,
        float rotation,
        glm::u8vec4 tint,
        std::vector<Vertex> &rect);
};

struct SpriteMap {
    ~SpriteMap();
    SpriteMap();
    GLuint tex = 0;
    std::unordered_map<std::string, Animation> sprites;
    void vbuffer_to_GL(
        std::vector<Vertex> &vertices,
        ColorTextureProgram &color_texture_program,
        GLuint vertex_buffer_for_color_texture_program,
        GLuint vertex_buffer
    ) const;
};

struct Sprite {
    Sprite(const SpriteMap &s_map, const std::string &s_name, size_t index=0);
    Sprite();
    ~Sprite();
    TexRectangle tex_coords;
    Transform transform;

    void draw(glm::vec2 camera_center,
        glm::vec2 object_center,
        glm::vec2 size,
        float rotation,
        glm::u8vec4 tint,
        std::vector<Vertex> &rect);
};

extern Load<SpriteMap> tile_sprites;
extern Load<SpriteMap> item_sprites;
extern Load<SpriteMap> player_sprites;
extern Load<SpriteMap> enemy_sprites;
extern Load<SpriteMap> bullet_sprites;
