#pragma once
#include <vector>
#include <memory>
#include "Vector2D.h"
#include "SDL.h"

namespace ecs
{
	class Entity;
	class Manager;
	class ActorSystem;
}

class KeyboardManager
{
public:
	explicit KeyboardManager(std::shared_ptr<ecs::Manager> manager);
	~KeyboardManager();

	// Disallow copy
	KeyboardManager(const KeyboardManager&) = delete;
	KeyboardManager& operator=(const KeyboardManager&) = delete;

	// Disallow move
	KeyboardManager(KeyboardManager&&) = delete;
	KeyboardManager& operator=(KeyboardManager&&) = delete;

	void setActorSystem(std::shared_ptr<ecs::ActorSystem> actorSystem);

	void update();
	void handleEvents(const SDL_Event& gameEvent);
	void handleMouse();

	const inline Vector2D getMousePos() const { return Vector2D(static_cast<float>(mouseX), static_cast<float>(mouseY)); };

private:
	int mouseX{ 0 };
	int mouseY{ 0 };

	bool pressed = false;

	std::vector<ecs::Entity*> controlledEntities;
	std::shared_ptr<ecs::Manager> manager;

	std::shared_ptr<ecs::ActorSystem> actorSystem; // bad idea maybe?
};