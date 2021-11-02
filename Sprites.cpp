#include "Sprites.hpp"

Sprite::Sprite(std::vector< glm::u8vec4 > data, glm::uvec2 size, Transform transform={{0.f,0.f},0.f}){
    this->transform = transform;
    this->picture.data = data;
    this->picture.size = size;
    this->tint = {0,0,0,0};
    glGenTextures(1, &tex);
}

Sprite::~Sprite(){
    glDeleteTextures(1, &tex);
}

void Sprite::draw(){
    //TODO: add transforms. Need player location/parent transform in order to do this.
    //TODO: add tinting
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, picture.size.x, picture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &picture.data);
    glBindTexture(GL_TEXTURE_2D, 0);
}
