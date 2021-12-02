#include "Sprites.hpp"
#include "ColorTextureProgram.hpp"
#include "gl_errors.hpp"
#include "load_save_png.hpp"
#include "data_path.hpp"

#include <iostream>
#include <fstream>
#include <string>

SpriteMap* load_func(std::string png, std::string filename, GLint interpolation=GL_NEAREST) {
	glm::uvec2 size;
    std::vector < glm::u8vec4 > data;
    std::ifstream data_in(data_path("sprites/" + filename));
    load_png(data_path("sprites/" + png), &size, &data, LowerLeftOrigin);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    
    //set filtering and wrapping parameters:
    //(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GL_ERRORS();

    SpriteMap *smap = new SpriteMap();
    smap->tex = tex;
    size_t sprite_size, num_row, num_col, num_anims;
    data_in >> sprite_size >> num_row >> num_col >> num_anims;
    if (num_row * sprite_size != size.y || num_col * sprite_size != size.x) {
        std::cerr << "Issue with sprite layouts, file: " << filename << std::endl;
        assert(false);
    }
    if (num_row <= 0 || num_col <= 0){
        std::cerr << "columns and rows have to be positive." << std::endl;
        assert(false);
    }
    constexpr float tex_eps = 0.0001f;
    for(size_t i = 0; i < num_anims; i++) {
        glm::vec2 sprite_center(0.f);
        glm::vec2 sprite_radius(0.f);
        std::string sprite_name;
        data_in >> sprite_name;
        smap->sprites[sprite_name] = Animation();
        Animation &current_sprite = smap->sprites[sprite_name];
        current_sprite.sprite_size = sprite_size;
        data_in >> sprite_center.x
            >> sprite_center.y
            >> sprite_radius.x
            >> sprite_radius.y;
        float half_sprite = sprite_size / 2.f;
        current_sprite.transform.translation = (1 / half_sprite) * sprite_center - glm::vec2(1.f);
        current_sprite.transform.scale = glm::vec2(half_sprite / sprite_radius.x, half_sprite / sprite_radius.y);

        //read in the sprites for the animation
        size_t num_sprites;
        data_in >> num_sprites;
        current_sprite.anim.resize(num_sprites);
        current_sprite.durations.resize(num_sprites);
        for(size_t j = 0; j < num_sprites; j++) {
            glm::vec2 sprite_location;
            data_in >> sprite_location.y >> sprite_location.x >> current_sprite.durations[j];
            assert(sprite_location.y <= num_row-1 && sprite_location.x <= num_col-1);
            assert(sprite_location.x >= 0 && sprite_location.y >= 0);
            if (j != 0) 
                current_sprite.durations[j] += current_sprite.durations[j-1];
            current_sprite.anim[j] = TexRectangle((sprite_location.x + tex_eps) / num_col, (sprite_location.y + tex_eps) / num_row, (sprite_location.x + 1 - tex_eps) / num_col, (sprite_location.y + 1 - tex_eps) / num_row);
        }
    }
    data_in.close();
    return smap;
}

Load <SpriteMap> tile_sprites(LoadTagDefault, [](){ return load_func("tiles.png", "tiles.info"); });
Load <SpriteMap> item_sprites(LoadTagDefault, [](){ return load_func("items.png", "items.info"); });
Load <SpriteMap> player_sprites(LoadTagDefault, [](){ return load_func("player.png", "player.info"); });
Load <SpriteMap> enemy_sprites(LoadTagDefault, [](){ return load_func("enemies_gray.png", "enemies.info"); });
Load <SpriteMap> bullet_sprites(LoadTagDefault, [](){ return load_func("bullets.png", "bullets.info"); });
Load <SpriteMap> ui_sprites(LoadTagDefault, [](){ return load_func("ui.png", "ui.info"); });
Load <SpriteMap> main_menu_spritemap(LoadTagDefault, [](){ return load_func("mm_background.png", "mm_background.info", GL_LINEAR); });

void SpriteMap::vbuffer_to_GL(
        std::vector<Vertex> &vertices,
        ColorTextureProgram &color_texture_program,
        GLuint vertex_buffer_for_color_texture_program,
        GLuint vertex_buffer) const {
    //use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);
    // add uniform to shader
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //color program
    //set color_texture_program as current program:
	glUseProgram(color_texture_program.program);
    //use the mapping vertex_buffer_for_color_texture_program to fetch vertex data:
	glBindVertexArray(vertex_buffer_for_color_texture_program);
    
    // bind texture to shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    // run the OpenGL pipeline:
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    vertices.clear();

    GL_ERRORS();
}

TexRectangle::TexRectangle(float _x0, float _y0, float _x1, float _y1){
    x0 = _x0; y0 = _y0; x1 = _x1; y1 = _y1;
}

TexRectangle::TexRectangle(){
    x0=y0=x1=y1=0.f;
}

SpriteMap::SpriteMap(){
    this->tex = 0;
}

SpriteMap::~SpriteMap(){
    glDeleteTextures(1, &tex);
    tex = 0;
}


Sprite::Sprite(const SpriteMap &s_map, const std::string &s_name, size_t index){
    const Animation &sprite_col = s_map.sprites.at(s_name);
    this->tex_coords = sprite_col.anim[index];
    this->transform = sprite_col.transform;
}
Sprite::Sprite(){}
Sprite::~Sprite(){}


void Animation::draw(
        float elapsed,
        glm::vec2 camera_center,
        glm::vec2 object_center,
        glm::vec2 size,
        float rotation,
        glm::u8vec4 tint,
        std::vector<Vertex> &rect
    ) {
    glm::vec2 center = object_center - camera_center;
    glm::vec2 radius = size * 0.5f * transform.scale;
    glm::vec2 tot_trans = -transform.translation * radius;

    #define WORLD_TO_SCREEN 2.f

    int idx = 0;
    for (; durations[idx] < elapsed; idx++);
    TexRectangle tex_coords = anim[idx];

    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x0, tex_coords.y0));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x1, tex_coords.y0));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x1, tex_coords.y1));

    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x0, tex_coords.y0));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x1, tex_coords.y1));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(-radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x0, tex_coords.y1));
}

void Sprite::draw(glm::vec2 camera_center,
    glm::vec2 object_center,
    glm::vec2 size,
    float rotation,
    glm::u8vec4 tint,
    std::vector<Vertex> &rect){
    glm::vec2 center = object_center - camera_center;
    glm::vec2 radius = size * 0.5f * transform.scale;
    glm::vec2 tot_trans = -transform.translation * radius;

    #define WORLD_TO_SCREEN 2.f
    
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x0, tex_coords.y0));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x1, tex_coords.y0));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x1, tex_coords.y1));

    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x0, tex_coords.y0));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x1, tex_coords.y1));
    rect.emplace_back(glm::vec3(center + glm::rotate(tot_trans + glm::vec2(-radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, tint, glm::vec2(tex_coords.x0, tex_coords.y1));
}
