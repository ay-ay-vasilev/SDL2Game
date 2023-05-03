#pragma once
#include <vector>
#include <memory>

namespace ecs
{
	class Entity;
	class Manager;
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

	void update();
	void handleEvents();

private:
	std::vector<ecs::Entity*> controlledEntities;
	std::shared_ptr<ecs::Manager> manager;
};