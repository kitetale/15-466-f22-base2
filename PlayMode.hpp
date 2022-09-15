#include "Mode.hpp"

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	//apple to roll
	Scene::Transform *apple = nullptr;
	glm::quat apple_base_rotation;
	Scene::Transform *apple1 = nullptr;
	glm::quat apple1_base_rotation;
	Scene::Transform *apple2 = nullptr;
	glm::quat apple2_base_rotation;
	Scene::Transform *apple3 = nullptr;
	glm::quat apple3_base_rotation;
	Scene::Transform *apple4 = nullptr;
	glm::quat apple4_base_rotation;
	Scene::Transform *apple5 = nullptr;
	glm::quat apple5_base_rotation;

	float wobble = 0.0f;
	
	//camera:
	Scene::Camera *camera = nullptr;

	//life (score)
	uint8_t life = 2;
	bool game = true;
	float gap = 0.0f;

};
