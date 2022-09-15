#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

GLuint apple_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > apple_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("apple.pnct"));
	apple_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > apple_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("apple.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = apple_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = apple_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});

PlayMode::PlayMode() : scene(*apple_scene) {
	life = 2;
	game = true;
	//get pointers to leg for convenience:
	for (auto &transform : scene.transforms) {
		if (transform.name == "apple") apple = &transform;
		if (transform.name == "apple1") apple1 = &transform;
		if (transform.name == "apple2") apple2 = &transform;
		if (transform.name == "apple3") apple3 = &transform;
		if (transform.name == "apple4") apple4 = &transform;
		if (transform.name == "apple5") apple5 = &transform;
	}
	if (apple == nullptr) throw std::runtime_error("Apple to be rolled not found.");
	if (apple1 == nullptr) throw std::runtime_error("Apple1 to be rolled not found.");
	if (apple2 == nullptr) throw std::runtime_error("Apple2 to be rolled not found.");
	if (apple3 == nullptr) throw std::runtime_error("Apple3 to be rolled not found.");
	if (apple4 == nullptr) throw std::runtime_error("Apple4 to be rolled not found.");
	if (apple5 == nullptr) throw std::runtime_error("Apple5 to be rolled not found.");
	apple_base_rotation = apple->rotation;
	apple1_base_rotation = apple->rotation;
	apple2_base_rotation = apple->rotation;
	apple3_base_rotation = apple->rotation;
	apple4_base_rotation = apple->rotation;
	apple5_base_rotation = apple->rotation;

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} 
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		}
	} 

	return false;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	wobble += elapsed / 10.0f;

	apple->rotation = apple_base_rotation * glm::angleAxis(
		glm::radians(700.0f * wobble),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	apple1->rotation = apple1_base_rotation * glm::angleAxis(
		glm::radians(900.0f * wobble),
		glm::vec3(0.0f, -1.0f, 0.0f)
	);
	apple2->rotation = apple2_base_rotation * glm::angleAxis(
		glm::radians(600.0f * wobble),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	apple3->rotation = apple3_base_rotation * glm::angleAxis(
		glm::radians(700.0f * wobble),
		glm::vec3(0.0f, -1.0f, 0.0f)
	);
	apple4->rotation = apple4_base_rotation * glm::angleAxis(
		glm::radians(800.0f * wobble),
		glm::vec3(0.0f, -1.0f, 0.0f)
	);
	apple5->rotation = apple5_base_rotation * glm::angleAxis(
		glm::radians(900.0f * wobble),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);

	apple->position = apple->position + glm::vec3(-0.09f,0.0f,-0.038f);
	apple1->position = apple1->position + glm::vec3(-0.09f,0.0f,-0.038f);
	apple2->position = apple2->position + glm::vec3(-0.09f,0.0f,-0.038f);
	apple3->position = apple3->position + glm::vec3(-0.09f,0.0f,-0.03f);
	apple4->position = apple4->position + glm::vec3(-0.09f,0.0f,-0.03f);
	apple5->position = apple5->position + glm::vec3(-0.09f,0.0f,-0.03f);

	//collision
	
	if (camera->transform->position.y <= apple->position.y + gap &&
		camera->transform->position.y >= apple->position.y - gap &&
		apple->position.x <= camera->transform->position.x) {
			--life;
	}
	if (camera->transform->position.y <= apple1->position.y + gap &&
		camera->transform->position.y >= apple1->position.y - gap &&
		apple1->position.x <= camera->transform->position.x) {
			--life;
	}
	if (camera->transform->position.y <= apple2->position.y + gap &&
		camera->transform->position.y >= apple2->position.y - gap &&
		apple2->position.x <= camera->transform->position.x) {
			--life;
	}
	if (camera->transform->position.y <= apple3->position.y + gap &&
		camera->transform->position.y >= apple3->position.y - gap &&
		apple3->position.x <= camera->transform->position.x) {
			--life;
	}
	if (camera->transform->position.y <= apple4->position.y + gap &&
		camera->transform->position.y >= apple4->position.y - gap &&
		apple4->position.x <= camera->transform->position.x) {
			--life;
	}
	if (camera->transform->position.y <= apple5->position.y + gap &&
		camera->transform->position.y >= apple5->position.y - gap &&
		apple5->position.x <= camera->transform->position.x) {
			--life;
	}
	if (life <=0 && !game) game = false;

	// TODO for the future: randomize spawning locations for this and other 5 apples
	if (apple->position.x < -26.0f || apple->position.z < -2.5f) {
		apple->position = glm::vec3(50.f,-1.3f,18.f);
		//std::cout << "new position for apple!" << std::endl;
	}
	// debug locations
	// std::cout << apple->position.x<<", "<<apple->position.y<<", "<<apple->position.z<<std::endl;
	// std::cout << camera->transform->position.x<<", "<<camera->transform->position.y<<", "<<camera->transform->position.z<<std::endl;

	//move camera:
	{

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed && 
			camera->transform->position.x < -28.4f) move.x =-1.0f;
		if (!left.pressed && right.pressed && 
			camera->transform->position.x > -31.3f) move.x = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 frame_right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 frame_forward = -frame[2];

		camera->transform->position += move.x * frame_right + move.y * frame_forward;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	GL_ERRORS(); //print any errors produced by this setup code

	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		float ofs = 2.0f / drawable_size.y;
		if (life && game){
			lines.draw_text("Use A to go left D to go right. Dodge the rolling apples!",
				glm::vec3(-aspect + 0.4f * H, 0.85 + 0.3f * H, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			lines.draw_text("Use A to go left D to go right. Dodge the rolling apples!",
				glm::vec3(-aspect + 0.2f * H + ofs, 0.85 + + 0.1f * H + ofs, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		} else {
			lines.draw_text("Game Over",
				glm::vec3(-aspect + 0.4f * H, 0.85 + 0.3f * H, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			lines.draw_text("Game Over",
				glm::vec3(-aspect + 0.2f * H + ofs, 0.85 + + 0.1f * H + ofs, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		}
		
		if (life == 2) {
			lines.draw_text("2 lives left",
				glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		} else if (life == 1) {
			lines.draw_text("1 life left",
				glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		} else {
			lines.draw_text("0 life left",
				glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		}
	}
}
