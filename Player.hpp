#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "DungeonGenerator.hpp"
#include "Sprites.hpp"
	
#define PLAYER_SPEED 100.0f
#define PLAYER_MAX_SPEED 250.0f

class Player {
public:
    ~Player() = default;
	Player();
    Player(glm::vec2 position, glm::vec2 velocity, float width) : position(position), velocity(velocity), width(width) {};

    void move(float elapsed);
    void move(glm::vec2 direction, float elapsed);
    void update(float elapsed,  Map &map);

	float get_width();

    const glm::vec2& get_pos();
    const glm::vec2& get_vel();

    void set_pos(glm::vec2 &pos);
    void set_vel(const glm::vec2 &vel);
    void set_vel(float x, float y);
    void add_vel(float x, float y);

	void on_hit(float damage);
	float get_hp();
	void add_hp(float hp);

    //draw the player
    enum PlayerStatus {IDLE, RUNNING, SHOOTING};
    PlayerStatus status = IDLE;
    void update_status(float elapsed, PlayerStatus new_status);
    float anim_time_remaining = 0;
    void draw(std::vector<Vertex> &vertices, glm::vec2 player_size);
    float dash_cd = 10.0f;
    bool did_shoot = false;
protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    Sprite s;
	float width = 32.0f;

	float hp = 5.0f;
};