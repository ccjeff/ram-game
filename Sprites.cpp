#include "Sprites.hpp"
#include "ColorTextureProgram.hpp"
#include "gl_errors.hpp"

#include <iostream>

Sprite::Sprite(std::vector< glm::u8vec4 > data, glm::uvec2 size, Transform transform){
    this->transform = transform;
    this->picture = {data, size};
    this->tint = {0,0,0,0};
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    //upload a 1x1 image of solid white to the texture:
    std::vector< glm::u8vec4 > data2(1, glm::u8vec4(255, 255, 255, 255));
    glm::uvec2 size2(1,1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size2.x, size2.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2.data());

    //set filtering and wrapping parameters:
    //(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //since texture uses a mipmap and we haven't uploaded one, instruct opengl to make one for us:
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
Sprite::Sprite(){
    this->tint = {0,0,0,0};
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    this->picture = {std::vector<glm::u8vec4>(), glm::uvec2(0,0)};
    //upload a 1x1 image of solid white to the texture:
    std::vector< glm::u8vec4 > data2(1, glm::u8vec4(255, 255, 255, 255));
    glm::uvec2 size2(1,1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size2.x, size2.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2.data());

    //set filtering and wrapping parameters:
    //(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //since texture uses a mipmap and we haven't uploaded one, instruct opengl to make one for us:
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Sprite::~Sprite(){
    glDeleteTextures(1, &tex);
    tex = 0;
}

void Sprite::draw(glm::vec2 camera_center,
    ColorTextureProgram &color_texture_program,
    GLuint vertex_buffer_for_color_texture_program,
    GLuint vertex_buffer){
    //TODO: add tinting

    glm::vec2 sprite_center = transform.displacement - camera_center;

    std::vector < Vertex > rect;

    #define WORLD_TO_SCREEN 2.f

	//inline helper function for rectangle drawing:
	auto draw_rectangle = [&rect](glm::vec2 const &center, glm::vec2 const &radius, float rotation, glm::u8vec4 const &color) {
		//draw rectangle as two CCW-oriented triangles:
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(0.f, 0.f));
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(1.f, 0.f));
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(1.f, 1.f));

		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(-radius.x, -radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(0.f, 0.f));
        rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(1.f, 1.f));
		rect.emplace_back(glm::vec3(center + glm::rotate(glm::vec2(-radius.x, radius.y), rotation), 0.0f) * WORLD_TO_SCREEN, color, glm::vec2(0.f, 1.f));
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
