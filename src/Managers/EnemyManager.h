#pragma once
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"

class EnemyManager
{
public:
	EnemyManager(Manager* manager);
	~EnemyManager();

	void InstantiateEnemy(const Vector2D pos, const Vector2D size, const float scale, const int speed, const std::string_view id) const;

private:
	Manager* manager;
};