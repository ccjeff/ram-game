#include "PongMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include "DungeonGenerator.hpp"
#include <cmath>

using namespace std;

PongMode::PongMode() {
	DungeonGenerator dg = DungeonGenerator(100, 100);
	dg.Generate(50);

	Room r1 = Room(0, 0, 12, 10);
	Room r2 = Room(15, 9, 1, 1);
	printf("Collision: %d\n", r1.Collides(r2));
	printf("AGH CHECK: %d\n", r2.Collides(r1));

	//----- allocate OpenGL resources -----
	{ //vertex buffer:
		glGenBuffers(1, &vertex_buffer);
		//for now, buffer will be un-filled.

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}

	{ //vertex array mapping buffer for color_texture_program:
		//ask OpenGL to fill vertex_buffer_for_color_texture_program with the name of an unused vertex array object:
		glGenVertexArrays(1, &vertex_buffer_for_color_texture_program);

		//set vertex_buffer_for_color_texture_program as the current vertex array object:
		glBindVertexArray(vertex_buffer_for_color_texture_program);

		//set vertex_buffer as the source of glVertexAttribPointer() commands:
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		//set up the vertex array object to describe arrays of PongMode::Vertex:
		glVertexAttribPointer(
			color_texture_program.Position_vec4, //attribute
			3, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 0 //offset
		);
		glEnableVertexAttribArray(color_texture_program.Position_vec4);
		//[Note that it is okay to bind a vec3 input to a vec4 attribute -- the w component will be filled with 1.0 automatically]

		glVertexAttribPointer(
			color_texture_program.Color_vec4, //attribute
			4, //size
			GL_UNSIGNED_BYTE, //type
			GL_TRUE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 4*3 //offset
		);
		glEnableVertexAttribArray(color_texture_program.Color_vec4);

		glVertexAttribPointer(
			color_texture_program.TexCoord_vec2, //attribute
			2, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 4*3 + 4*1 //offset
		);
		glEnableVertexAttribArray(color_texture_program.TexCoord_vec2);

		//done referring to vertex_buffer, so unbind it:
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//done setting up vertex array object, so unbind it:
		glBindVertexArray(0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}
	{
		std::vector< glm::u8vec4 > data(64, glm::u8vec4(255,0,0,255));
		glm::uvec2 size(8,8);
		player_sprite = Sprite(data, size);
		player_sprite.tint = glm::u8vec4(255, 0, 0, 255);
	}
	{
		std::vector< glm::u8vec4 > data(64, glm::u8vec4(255,255,255,255));
		glm::uvec2 size(8,8);
		dummy_sprite = Sprite(data, size);
		dummy_sprite.tint = glm::u8vec4(255, 255, 255, 255);
	}

	{ //solid white texture:
		//ask OpenGL to fill white_tex with the name of an unused texture object:
		glGenTextures(1, &white_tex);

		//bind that texture object as a GL_TEXTURE_2D-type texture:
		glBindTexture(GL_TEXTURE_2D, white_tex);

		//upload a 1x1 image of solid white to the texture:
		glm::uvec2 size = glm::uvec2(1,1);
		std::vector< glm::u8vec4 > data(size.x*size.y, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

		//set filtering and wrapping parameters:
		//(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//since texture uses a mipmap and we haven't uploaded one, instruct opengl to make one for us:
		glGenerateMipmap(GL_TEXTURE_2D);

		//Okay, texture uploaded, can unbind it:
		glBindTexture(GL_TEXTURE_2D, 0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}

	{
		// initializing player
		player = std::make_shared<Player>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	}
}

PongMode::~PongMode() {

	//----- free OpenGL resources -----
	glDeleteBuffers(1, &vertex_buffer);
	vertex_buffer = 0;

	glDeleteVertexArrays(1, &vertex_buffer_for_color_texture_program);
	vertex_buffer_for_color_texture_program = 0;

	glDeleteTextures(1, &white_tex);
	white_tex = 0;

}

bool PongMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	this->window_size = window_size;

	if(evt.type == SDL_MOUSEBUTTONDOWN) {
		Pistol p;
		Bullet* b = p.do_shoot(player->get_pos(), glm::normalize(
				glm::vec2(
					float(evt.motion.x) / window_size.x * 2.0f - 1.0f,
					float(evt.motion.y)  / window_size.y *-2.0f + 1.0f
				)
			)
		);

		// cout << float(evt.motion.x) / window_size.x * 2.0f - 1.0f << " " <<  float(evt.motion.y) / window_size.y *-2.0f + 1.0f << endl;
		// cout << evt.motion.x << " " <<  evt.motion.y << endl;

		bullets.emplace_back(b);
	} else {
		if (evt.type == SDL_KEYDOWN) {
			if (evt.key.keysym.sym == SDLK_a) {
				left.downs += 1;
				left.pressed = true;
				return true;
			} else if (evt.key.keysym.sym == SDLK_d) {
				right.downs += 1;
				right.pressed = true;
				return true;
			} else if (evt.key.keysym.sym == SDLK_w) {
				up.downs += 1;
				up.pressed = true;
				return true;
			} else if (evt.key.keysym.sym == SDLK_s) {
				down.downs += 1;
				down.pressed = true;
				return true;
			}
		} else if (evt.type == SDL_KEYUP) {
			if (evt.key.keysym.sym == SDLK_a) {
				left.pressed = false;
				return true;
			} else if (evt.key.keysym.sym == SDLK_d) {
				right.pressed = false;
				return true;
			} else if (evt.key.keysym.sym == SDLK_w) {
				up.pressed = false;
				return true;
			} else if (evt.key.keysym.sym == SDLK_s) {
				down.pressed = false;
				return true;
			}
		}
	}

	return false;
}

void PongMode::update(float elapsed, glm::vec2 const &drawable_size) {
	int deleted = 0;
	glm::vec2 player_vel = player->get_vel();

	if (left.pressed && !right.pressed) {
		player_vel.x -= PLAYER_SPEED;
		player_vel.x = std::max(player_vel.x, -PLAYER_MAX_SPEED);
	}
	if (!left.pressed && right.pressed) {
		player_vel.x += PLAYER_SPEED;
		player_vel.x = std::min(player_vel.x, PLAYER_MAX_SPEED);
	}
	if (down.pressed && !up.pressed) {
		player_vel.y -= PLAYER_SPEED;
		player_vel.y = std::max(player_vel.y, -PLAYER_MAX_SPEED);
	}
	if (!down.pressed && up.pressed) {
		player_vel.y += PLAYER_SPEED;
		player_vel.y = std::min(player_vel.y, PLAYER_MAX_SPEED);
	}
	player->set_vel(player_vel);

	for(size_t i = 0; i < bullets.size(); i++) {
		bullets[i]->update_pos(elapsed * 500.0f);

		glm::vec2 pos = bullets[i]->get_pos();
		
		//cout << pos.x << " " << pos.y << endl;
		
		if(abs(pos.x - player->get_pos().x) > drawable_size.x
			|| abs(pos.y - player->get_pos().y) > drawable_size.y) {
				//cout << "del " << i << " " << bullets.size() - deleted << endl;
				
				deleted++;
				delete bullets[i];
				bullets.erase(bullets.begin() + (i--));
		}
	}
	player->update(elapsed);

	player->set_vel(player->get_vel() * 0.8f);

	player_sprite.transform.displacement = player->get_pos();
}

void PongMode::draw(glm::uvec2 const &drawable_size) {
	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		// constexpr float H = 0.09f;
		// lines.draw_text("Player pos: " + to_string(player->get_pos().x) + " " + to_string(player->get_pos().y),
		// 	glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
		// 	glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		// 	glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		// float ofs = 2.0f / drawable_size.y;
		// lines.draw_text("Player pos: " + to_string(player->get_pos().x) + " " + to_string(player->get_pos().y),
		// 	glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
		// 	glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		// 	glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}

	//some nice colors from the course web page:
	#define HEX_TO_U8VEC4( HX ) (glm::u8vec4( (HX >> 24) & 0xff, (HX >> 16) & 0xff, (HX >> 8) & 0xff, (HX) & 0xff ))
	const glm::u8vec4 bg_color = HEX_TO_U8VEC4(0x193b59ff);
	const glm::u8vec4 fg_color = HEX_TO_U8VEC4(0xf2d2b6ff);
	// const glm::u8vec4 shadow_color = HEX_TO_U8VEC4(0xf2ad94ff);
	const std::vector< glm::u8vec4 > trail_colors = {
		HEX_TO_U8VEC4(0xf2ad9488),
		HEX_TO_U8VEC4(0xf2897288),
		HEX_TO_U8VEC4(0xbacac088),
	};
	#undef HEX_TO_U8VEC4

	//build matrix that scales and translates appropriately:
	glm::mat4 court_to_clip = glm::mat4(
		glm::vec4(1.0f / drawable_size.x, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f / drawable_size.y, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(0, 0, 0.0f, 1.0f)
	);
	//NOTE: glm matrices are specified in *Column-Major* order,
	// so each line above is specifying a *column* of the matrix(!)

	//other useful drawing constants:
	// const float wall_radius = 0.05f;
	// const float shadow_offset = 0.07f;
	// const float padding = 0.14f; //padding between outside of walls and edge of window

	//---- compute vertices to draw ----

	//vertices will be accumulated into this list and then uploaded+drawn at the end of this function:
	std::vector< Vertex > vertices;

	//inline helper function for rectangle drawing:
	// auto draw_rectangle = [&vertices](glm::vec2 const &center, glm::vec2 const &radius, glm::u8vec4 const &color) {
	// 	//draw rectangle as two CCW-oriented triangles:
	// 	vertices.emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
	// 	vertices.emplace_back(glm::vec3(center.x+radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
	// 	vertices.emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));

	// 	vertices.emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
	// 	vertices.emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
	// 	vertices.emplace_back(glm::vec3(center.x-radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
	// };

	//clear the color buffer:
	dummy_sprite.tint = fg_color;
	glClearColor(bg_color.r / 255.0f, bg_color.g / 255.0f, bg_color.b / 255.0f, bg_color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	player_sprite.transform.scale = glm::vec2(10.0f, 10.0f);
	player_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
	
	for(auto b : bullets) {
		dummy_sprite.transform.displacement = b->get_pos();
		dummy_sprite.transform.scale = glm::vec2(2.0f, 2.0f);
		dummy_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
		//draw_rectangle(b->get_pos(), glm::vec2(0.2f, 0.2f), fg_color);
	}

	//---- actual drawing ----

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	//upload vertices to vertex_buffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //set vertex_buffer as current
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STREAM_DRAW); //upload vertices array
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//set color_texture_program as current program:
	glUseProgram(color_texture_program.program);

	//upload OBJECT_TO_CLIP to the proper uniform location:
	glUniformMatrix4fv(color_texture_program.OBJECT_TO_CLIP_mat4, 1, GL_FALSE, glm::value_ptr(court_to_clip));

	//use the mapping vertex_buffer_for_color_texture_program to fetch vertex data:
	glBindVertexArray(vertex_buffer_for_color_texture_program);

	//bind the solid white texture to location zero so things will be drawn just with their colors:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, white_tex);

	//run the OpenGL pipeline:
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));

	//unbind the solid white texture:
	glBindTexture(GL_TEXTURE_2D, 0);

	//reset vertex array to none:
	glBindVertexArray(0);

	//reset current program to none:
	glUseProgram(0);
	

	GL_ERRORS(); //PARANOIA: print errors just in case we did something wrong.
}
