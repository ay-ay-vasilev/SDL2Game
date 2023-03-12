#include "AISystem.h"

void AISystem::update()
{
	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
	basicEnemies = manager.getEntitiesWithComponent<AIComponentBasicEnemy>();

	for (const auto& basicEnemy : basicEnemies)
	{
		const auto& basicEnemyTransform = basicEnemy->getComponent<TransformComponent>();
		const auto& basicEnemyAI = basicEnemy->getComponent<AIComponentBasicEnemy>();

		bool enemyHasTarget = basicEnemyAI->hasTarget();

		if (basicEnemyAI->hasTarget() && basicEnemyAI->getDistance() > basicEnemyAI->getLoseAggroDistance())
		{
			basicEnemyAI->loseTarget();
			enemyHasTarget = false;
		}
		for (const auto& player : players)
		{
			if (!player->isActive())
				continue;
			const auto& playerTransform = player->getComponent<TransformComponent>();
			const auto distance = Vector2D::Distance(playerTransform->getPosition(), basicEnemyTransform->getPosition());

			if (!enemyHasTarget)
			{
				if (distance < basicEnemyAI->getAggroDistance())
					basicEnemyAI->setNewTarget(player);
			}
			else if (enemyHasTarget)
			{
				if (distance < basicEnemyAI->getAggroDistance() && distance < basicEnemyAI->getDistance())
					basicEnemyAI->setNewTarget(player);
			}
		}
	}
}

void AISystem::draw()
{
}