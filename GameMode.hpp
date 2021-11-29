#pragma once
#include "GL.hpp"
#include "ColorTextureProgram.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Mode.hpp"
#include "GameState.hpp"

#include "Sprites.hpp"
#include "DrawLines.hpp"
#include "DungeonGenerator.hpp"
#include "Load.hpp"
#include "Sound.hpp"

/*
 * GameMode is a game mode that implements a single-player game of Pong.
 */

struct GameMode : Mode {
	GameMode();
	virtual ~GameMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed, glm::vec2 const &drawable_size) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	glm::vec2 court_radius = glm::vec2(7.0f, 5.0f);
	glm::vec2 paddle_radius = glm::vec2(0.2f, 1.0f);
	glm::vec2 ball_radius = glm::vec2(0.2f, 0.2f);
	glm::vec2 score_radius = glm::vec2(0.1f, 0.1f);

	glm::vec2 window_size;

	GameState* gs;

	Sprite floor_sprite,
		player_sprite,
		basic_enemy_red_sprite,
		basic_enemy_green_sprite,
		basic_enemy_blue_sprite,
		melee_enemy_red_sprite,
		melee_enemy_green_sprite,
		melee_enemy_blue_sprite,
		p_bullet,
		e_bullet,
		blank_sprite,
		r_learning_sprite,
		ray_tracing_sprite,
		dijkstra_sprite,
		p_np_sprite,
		door_locked_sprite,
		door_unlocked_sprite;
	
	// sound effects
	std::shared_ptr<Sound::PlayingSample> bgm;
	std::shared_ptr<Sound::PlayingSample> shoot_sound;
	std::shared_ptr<Sound::PlayingSample> walk_sound;
	float walk_sound_cd = 0.0f;
	float shoot_cd = 0.0f;
	//----- opengl assets / helpers ------


	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;
};
