#include "PongMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <random>
#include "DungeonGenerator.hpp"
#include <cmath>

using namespace std;

Load< Sound::Sample > load_bgm(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("bgm.opus"));
});


Load< Sound::Sample > load_shoot(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("gun_shot.wav"));
});

Load< Sound::Sample > load_walk(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("walk.wav"));
});

PongMode::PongMode() {
	// Room r1 = Room(0, 0, 12, 10);
	// Room r2 = Room(15, 9, 1, 1);
	// printf("Collision: %d\n", r1.Collides(r2));
	// printf("AGH CHECK: %d\n", r2.Collides(r1));

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

	floor_sprite = Sprite(*green_tile, "sprite");
	player_sprite = Sprite(*green_smiley, "sprite");
	enemy_sprite = Sprite(*red_smiley, "sprite");
	p_bullet = Sprite(*green_circle, "sprite");
	e_bullet = Sprite(*red_circle, "sprite");
	blank_sprite = Sprite(*black, "sprite");
	r_learning_sprite = Sprite(*r_learning, "sprite");
	ray_tracing_sprite = Sprite(*ray_tracing, "sprite");
	door_locked_sprite = Sprite(*door_locked, "sprite");
	door_unlocked_sprite = Sprite(*door_unlocked, "sprite");
	
	bgm = Sound::loop(*load_bgm, 0.5f, 0.0f);

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
		// initializing player and dungeon
		dg = new DungeonGenerator(100, 100);
		dg->Generate(20);
		dg->map.SetScalingFactor(64.0f);

		player = std::make_shared<Player>(dg->map.GetWorldCoord(dg->player_start), glm::vec2(0.0f, 0.0f), 32.0f);

		for (glm::ivec2 pos : dg->monsterPositions)
		{
			enemies.emplace_back(new BasicEnemy(dg->map.GetWorldCoord(pos), glm::vec2(0.0f, 0.0f)));
		}
		
	}
	//Add things for testing
	{
		items_on_ground.emplace_back(new ReinforcementLearning(player, glm::vec2(dg->player_start) * dg->map.scalingFactor + 32.0f, &r_learning_sprite));
		items.emplace_back(new RayTracing(player, glm::vec2(0.0f, 0.0f), &ray_tracing_sprite));
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

	for(auto e : enemies) {
		delete e;
	}

	for(auto b : bullets) {
		delete b;
	}

	for(auto b : enemy_bullets) {
		delete b;
	}
}

bool PongMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	this->window_size = window_size;

	if(evt.type == SDL_MOUSEBUTTONDOWN) {
		Pistol p;
		Bullet* b = p.do_shoot(player->get_pos(), glm::normalize(
				glm::vec2(
					(float(evt.motion.x) / window_size.x * 2.0f - 1.0f) * window_size.x,
					(float(evt.motion.y)  / window_size.y *-2.0f + 1.0f) * window_size.y
				)
			)
		);

		shoot_sound = Sound::play(*load_shoot, 0.5f, 0.0f);
		
		bullets.emplace_back(b);

		for(auto i : items) {
			i->on_shoot(b);
		}
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
	for(auto item : items) {
		item->preupdate();
	}

	glm::vec2 player_vel = player->get_vel();
	if (left.pressed && !right.pressed) {
		if (walk_sound_cd >= 0.25f) {
			walk_sound = Sound::play(*load_walk, 0.25f, 0.0f);
			walk_sound_cd = 0.0f;
		}
		walk_sound_cd += elapsed;
		player_vel.x -= PLAYER_SPEED;
		player_vel.x = std::max(player_vel.x, -PLAYER_MAX_SPEED);
	}
	if (!left.pressed && right.pressed) {
		if (walk_sound_cd >= 0.25f) {
			walk_sound = Sound::play(*load_walk, 0.25f, 0.0f);
			walk_sound_cd = 0.0f;
		}
		walk_sound_cd += elapsed;
		player_vel.x += PLAYER_SPEED;
		player_vel.x = std::min(player_vel.x, PLAYER_MAX_SPEED);
	}
	if (down.pressed && !up.pressed) {
		if (walk_sound_cd >= 0.25f && !left.pressed && !right.pressed) {
			walk_sound = Sound::play(*load_walk, 0.25f, 0.0f);
			walk_sound_cd = 0.0f;
		}
		if (!left.pressed && !right.pressed) {
			walk_sound_cd += elapsed;
		}
		player_vel.y -= PLAYER_SPEED;
		player_vel.y = std::max(player_vel.y, -PLAYER_MAX_SPEED);
	}
	if (!down.pressed && up.pressed) {
		if (walk_sound_cd >= 0.25f && !left.pressed && !right.pressed) {
			walk_sound = Sound::play(*load_walk, 0.25f, 0.0f);
			walk_sound_cd = 0.0f;
		}
		if (!left.pressed && !right.pressed) {
			walk_sound_cd += elapsed;
		}
		player_vel.y += PLAYER_SPEED;
		player_vel.y = std::min(player_vel.y, PLAYER_MAX_SPEED);
	}

	//Item pickups
	{
		int deleted = 0;
		for(size_t i = 0; i < items_on_ground.size(); i++) {
			float dist_x = abs(items_on_ground[i]->get_pos().x - player->get_pos().x);
			float dist_y = abs(items_on_ground[i]->get_pos().y - player->get_pos().y);

			if(dist_x < player->get_width() && dist_y < player->get_width()) {
				items.emplace_back(items_on_ground[i]);

				//DO NOT delete here because the ptr is reused
				deleted++;
				items_on_ground.erase(items_on_ground.begin() + (i--));

				continue;
			}
			
		}
	}

	//Player bullet updates
	{
		int deleted = 0;
		for(size_t i = 0; i < bullets.size(); i++) {
			glm::vec2 old_pos = bullets[i]->get_pos();
			bullets[i]->update_pos(elapsed * 500.0f);

			glm::vec2 pos = bullets[i]->get_pos();
			
			if (dg->map.ValueAtWorld(pos.x, pos.y) == 0 
				|| dg->map.ValueAtWorld(old_pos.x, pos.y) == 0
				|| dg->map.ValueAtWorld(pos.x, old_pos.y) == 0) {

				//Bounce the bullet if there are bounces left
				if(bullets[i]->get_bounces() >= 1) {
					bullets[i]->set_vel(-bullets[i]->get_vel());
					bullets[i]->set_bounces(bullets[i]->get_bounces() - 1);
					bullets[i]->update_pos(elapsed * 500.0f);
					continue;
				}

				deleted++;
				delete bullets[i];
				bullets.erase(bullets.begin() + (i--));

				for(auto item : items) {
					item->on_bullet_destroyed();
				}

				continue;
			}

			float dist_player_x = abs(player->get_pos().x - pos.x);
			float dist_player_y = abs(player->get_pos().y - pos.y);

			//Enemy got hit
			bool enemy_hit = false;
			for(auto e : enemies) {
				float dist_x = abs(e->get_pos().x - pos.x);
				float dist_y = abs(e->get_pos().y - pos.y);

				if(dist_x < e->get_width()/2.0f && dist_y < e->get_width()/2.0f) {
					//std::cout << "Enemy was hit by a bullet" << std::endl;
					e->on_hit(bullets[i]->get_damage());
					enemy_hit = true;

					for(auto item : items) {
						item->on_dealt_damage();
					}

					break;
				}
			}

			//An enemy was hit, destroy the bullet, check enemy hp, and continue
			if(enemy_hit) {
				deleted++;
				delete bullets[i];
				bullets.erase(bullets.begin() + (i--));

				int enemies_deleted = 0;
				for(size_t i = 0; i < enemies.size(); i++) {

					//If enemy died
					if(enemies[i]->get_hp() <= 0.0f) {
						for(auto item : items) {
							item->on_kill();
						}

						//Drop item with rng
						int drop = rand() % 10;
						cout << drop << endl;

						if(drop == 0) {
							cout << "yay item" << endl;
							drop = rand() % 2;

							if(drop == 1)
								items_on_ground.emplace_back(new ReinforcementLearning(player, glm::vec2(enemies[i]->get_pos()) * dg->map.scalingFactor, &r_learning_sprite));
							else
								items_on_ground.emplace_back(new RayTracing(player, glm::vec2(enemies[i]->get_pos()) * dg->map.scalingFactor, &ray_tracing_sprite));
						}

						enemies_deleted++;
						delete enemies[i];
						enemies.erase(enemies.begin() + (i--));
					}
				}

				for(auto item : items) {
					item->on_bullet_destroyed();
				}

				continue;
			}
			
			if(dist_player_x > drawable_size.x
				|| dist_player_y > drawable_size.y) {
				//cout << "del " << i << " " << bullets.size() - deleted << endl;
				
				deleted++;
				delete bullets[i];
				bullets.erase(bullets.begin() + (i--));

				for(auto item : items) {
					item->on_bullet_destroyed();
				}

				continue;
			}
		}
	}
	player->set_vel(player_vel);
	
	//Enemy bullet updates
	{
		int deleted = 0;
		for(size_t i = 0; i < enemy_bullets.size(); i++) {
			glm::vec2 old_pos = enemy_bullets[i]->get_pos();
			enemy_bullets[i]->update_pos(elapsed * 500.0f);
			glm::vec2 pos = enemy_bullets[i]->get_pos();

			if (dg->map.ValueAtWorld(pos.x, pos.y) == 0 
				|| dg->map.ValueAtWorld(old_pos.x, pos.y) == 0
				|| dg->map.ValueAtWorld(pos.x, old_pos.y) == 0) {
				deleted++;
				delete enemy_bullets[i];
				enemy_bullets.erase(enemy_bullets.begin() + (i--));
				continue;
			}
			
			//cout << pos.x << " " << pos.y << endl;

			float dist_x = abs(player->get_pos().x - pos.x);
			float dist_y = abs(player->get_pos().y - pos.y);

			//Player got hit
			if(dist_x < player->get_width()/2.0f && dist_y < player->get_width()/2.0f) {
				player->on_hit(enemy_bullets[i]->get_damage());

				deleted++;
				delete enemy_bullets[i];
				enemy_bullets.erase(enemy_bullets.begin() + (i--));

				//Player death
				//TODO: pull this out to a method and add other fancy stuff like remove items
				if(player->get_hp() <= 0) {
					glm::vec2 pos = dg->map.GetWorldCoord(dg->player_start);
					player->set_pos(pos);
					player->add_hp(5.0f);
					return;
				}

				for(auto item : items) {
					item->on_recv_damage();
				}

				continue;
			}
			
			if(dist_x > drawable_size.x
				|| dist_y > drawable_size.y) {
				//cout << "del " << i << " " << bullets.size() - deleted << endl;
				
				deleted++;
				delete enemy_bullets[i];
				enemy_bullets.erase(enemy_bullets.begin() + (i--));

				continue;
			}
		}
	}

	//Ask enemies to attack after to give players more advantage
	for(auto e : enemies) {
		e->update(elapsed);
		e->move(elapsed, player->get_pos(), dg->map);
		Bullet* b = e->do_attack(player->get_pos());
		if(b != nullptr) {
			//cout << "enemy attack D: " << endl;
			enemy_bullets.emplace_back(b);
		}
	}

	{ //Room locking updates
		if (activeRoom == NULL)
		{
			for (size_t i = 0; i < dg->rooms.size(); i++)
			{
				if (dg->rooms[i].is_inside(player->get_pos()))
				{
					activeRoom = &dg->rooms[i];
					activeRoom->SetMap(&dg->map);
					activeRoom->LockRoom();
					break;
				}
			}
		}
		else
		{
			if (activeRoom->locked)
			{
				bool valid = true;
				for (auto e : enemies) {
					if (activeRoom->is_inside(e->get_pos()))
					{
						valid = false;
						break;
					}
				}
				if (valid)
				{
					activeRoom->UnlockRoom();
				}
			}
			else
			{
				if (!activeRoom->is_inside(player->get_pos()))
				{			
					activeRoom = NULL;
				}
			}
		}
	}
	
	//cout <<player->get_pos().x << " " << player->get_pos().y << endl;

	player->update(elapsed, dg->map);
	player_sprite.transform.displacement = player->get_pos();

	for(auto item : items) {
		item->postupdate();
	}

	//cout << player->get_hp() << endl;
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

	//---- compute vertices to draw ----

	//vertices will be accumulated into this list and then uploaded+drawn at the end of this function:
	// std::vector< Vertex > vertices;

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
	glClearColor(bg_color.r / 255.0f, bg_color.g / 255.0f, bg_color.b / 255.0f, bg_color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	{
		const float FLOOR_TILE_SIZE = dg->map.scalingFactor;
		floor_sprite.transform.size = glm::vec2(FLOOR_TILE_SIZE, FLOOR_TILE_SIZE);
		glm::ivec2 tile_id = dg->map.GetTile(player->get_pos().x, player->get_pos().y);
		for(int i = tile_id.x - 12; i <= tile_id.x + 12; i++){
			for(int j = tile_id.y - 12; j <= tile_id.y + 12; j++){
				floor_sprite.transform.displacement = glm::vec2(float(i) + 0.5f, float(j) + 0.5f) * FLOOR_TILE_SIZE;
				glm::ivec2 cur_tile_id = dg->map.GetTile(floor_sprite.transform.displacement.x, floor_sprite.transform.displacement.y);
				if(cur_tile_id.x < 0 || cur_tile_id.y < 0)
					blank_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
				else if (dg->map.ValueAt(cur_tile_id.x, cur_tile_id.y) == 0) //TODO: Change this when do sprites, this check is backwards but looks nice for the demo.
					blank_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
				else if (dg->map.ValueAt(cur_tile_id.x, cur_tile_id.y) == 2)
				{
					door_unlocked_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
				}
				else if (dg->map.ValueAt(cur_tile_id.x, cur_tile_id.y) == 3)
				{
					door_locked_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
				}
				else
				{
					floor_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
				}
			}
		}
	}
	player_sprite.transform.size = glm::vec2(
		player->get_vel().x < 0 ? 
				-1.0f * player->get_width() : 
				player->get_width(), player->get_width()
	);
	player_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);

	for(auto b : bullets) {
		p_bullet.transform.displacement = b->get_pos();
		p_bullet.transform.size = glm::vec2(
			b->get_vel().x < 0 ? 
				-1.0f * b->get_width() : 
				b->get_width(), b->get_width()
		);
		p_bullet.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
	}

	for(auto b : enemy_bullets) {
		e_bullet.transform.displacement = b->get_pos();
		e_bullet.transform.size = glm::vec2(
			b->get_vel().x < 0 ? 
				-1.0f * b->get_width() : 
				b->get_width(), b->get_width()
		);
		e_bullet.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
	}

	for(auto e : enemies) {
		enemy_sprite.transform.displacement = e->get_pos();
		enemy_sprite.transform.size = glm::vec2(
			e->get_vel().x < 0 ? 
				-1.0f * e->get_width() : 
				e->get_width(), e->get_width()
		);
		enemy_sprite.draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
	}

	for(auto i : items_on_ground) {

		i->get_sprite()->transform.displacement = i->get_pos();
		i->get_sprite()->transform.size = glm::vec2(i->get_width(), i->get_width());
		i->get_sprite()->draw(player->get_pos(), color_texture_program, vertex_buffer_for_color_texture_program, vertex_buffer);
		//cout << "Drawn item on ground " << i->get_width() << " " << glm::to_string(i->get_pos()) << endl; 
	}

	//---- actual drawing ----

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	// //upload vertices to vertex_buffer:
	// glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //set vertex_buffer as current
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STREAM_DRAW); //upload vertices array
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	//set color_texture_program as current program:
	glUseProgram(color_texture_program.program);

	//upload OBJECT_TO_CLIP to the proper uniform location:
	glUniformMatrix4fv(color_texture_program.OBJECT_TO_CLIP_mat4, 1, GL_FALSE, glm::value_ptr(court_to_clip));

	//use the mapping vertex_buffer_for_color_texture_program to fetch vertex data:
	glBindVertexArray(vertex_buffer_for_color_texture_program);

	// //bind the solid white texture to location zero so things will be drawn just with their colors:
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, white_tex);

	// //run the OpenGL pipeline:
	// glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));

	// //unbind the solid white texture:
	// glBindTexture(GL_TEXTURE_2D, 0);

	// //reset vertex array to none:
	// glBindVertexArray(0);

	//reset current program to none:
	glUseProgram(0);
	

	GL_ERRORS(); //PARANOIA: print errors just in case we did something wrong.
}
