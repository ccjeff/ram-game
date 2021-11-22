#include "Sprites.hpp"
#include "ColorTextureProgram.hpp"
#include "gl_errors.hpp"
#include "load_save_png.hpp"
#include "data_path.hpp"

#include <iostream>
#include <string>

SpriteMap* load_func(std::string png) {
	glm::uvec2 size;
    std::vector < glm::u8vec4 > data;
    load_png(data_path(png), &size, &data, LowerLeftOrigin);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    
    //set filtering and wrapping parameters:
    //(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GL_ERRORS();

    SpriteMap *smap = new SpriteMap();
    smap->tex = tex;
    smap->sprites["sprite"] = TexRectangle(0.f, 0.f, 1.f, 1.f);
    return smap;
}

Load <SpriteMap> green_tile(LoadTagDefault, [](){ return load_func("green_tile.png"); });
Load <SpriteMap> black(LoadTagDefault, [](){ return load_func("black.png"); });
Load <SpriteMap> green_circle(LoadTagDefault, [](){ return load_func("green_circle.png"); });
Load <SpriteMap> red_circle(LoadTagDefault, [](){ return load_func("red_circle.png"); });
Load <SpriteMap> green_smiley(LoadTagDefault, [](){ return load_func("green_smiley.png"); });
Load <SpriteMap> red_smiley(LoadTagDefault, [](){ return load_func("red_smiley.png"); });
Load <SpriteMap> melee_enemy(LoadTagDefault, [](){ return load_func("melee_enemy.png"); });
Load <SpriteMap> r_learning(LoadTagDefault, [](){ return load_func("r_learning.png"); });
Load <SpriteMap> ray_tracing(LoadTagDefault, [](){ return load_func("ray_tracing.png"); });
Load <SpriteMap> dijkstra(LoadTagDefault, [](){ return load_func("32dijkstra.png"); });
Load <SpriteMap> p_np(LoadTagDefault, [](){ return load_func("pnp.png"); });
Load <SpriteMap> door_locked(LoadTagDefault, [](){ return load_func("door_locked.png"); });
Load <SpriteMap> door_unlocked(LoadTagDefault, [](){ return load_func("door_unlocked.png"); });

TexRectangle::TexRectangle(float _x0, float _y0, float _x1, float _y1){
    x0 = _x0; y0 = _y0; x1 = _x1; y1 = _y1;
}

TexRectangle::TexRectangle(){
    x0=y0=x1=y1=0.f;
}

SpriteMap::SpriteMap(){
    this->tex = 0;
}

SpriteMap::~SpriteMap(){}

Sprite::Sprite(const SpriteMap &s_map, const std::string &s_name, Transform transform){
    this->transform = transform;
    this->tex = s_map.tex;
    this->tex_coords = s_map.sprites.at(s_name);
}
Sprite::Sprite(){}

Sprite::~Sprite(){}

void Sprite::draw(glm::vec2 camera_center,
    ColorTextureProgram &color_texture_program,
    GLuint vertex_buffer_for_color_texture_program,
    GLuint vertex_buffer){
    //TODO: add tinting

    glm::vec2 sprite_center = transform.displacement - camera_center;

    std::vector < Vertex > rect;

    #define WORLD_TO_SCREEN 2.f

	//inline helper function for rectangle drawing:
	auto draw_rectangle = [&rect, this](glm::vec2 const &center, glm::vec2 const &radius, float rotation, glm::u8vec4 const &color) {
		//draw rectangle as two CCW-oriented triangles:
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(tex_coords.x0, tex_coords.y0));
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(tex_coords.x1, tex_coords.y0));
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(tex_coords.x1, tex_coords.y1));

		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(tex_coords.x0, tex_coords.y0));
        rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(tex_coords.x1, tex_coords.y1));
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(-radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(tex_coords.x0, tex_coords.y1));
	};

    draw_rectangle(sprite_center, glm::vec2(transform.size.x / 2, transform.size.y / 2), transform.rotation, tint);

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);
    // add uniform to shader
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, rect.size() * sizeof(rect[0]), rect.data(), GL_STREAM_DRAW);
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
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(rect.size()));

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    GL_ERRORS();
}
