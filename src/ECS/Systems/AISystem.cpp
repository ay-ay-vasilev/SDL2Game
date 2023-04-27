#include "AISystem.h"
#include "AIComponentBasicEnemy.h"
#include "FactionComponent.h"
#include "TransformComponent.h"

#include "Game.h"

void AISystem::update()
{
	entitiesWithFactions = manager.getEntitiesWithComponents<FactionComponent>();
	aiActors = manager.getEntitiesWithComponent<AIComponentBasicEnemy>();

	for (const auto& aiActor : aiActors)
	{
		const auto& aiActorTransform = aiActor->getComponent<TransformComponent>();
		const auto& aiActorAI = aiActor->getComponent<AIComponentBasicEnemy>();
		const auto& aiActorFaction = aiActor->getComponent<FactionComponent>();

		bool enemyHasTarget = aiActorAI->hasTarget();

		if (aiActorAI->hasTarget() && aiActorAI->getDistance() > aiActorAI->getLoseAggroDistance())
		{
			aiActorAI->loseTarget();
			enemyHasTarget = false;
		}
		for (const auto& entityWithFaction : entitiesWithFactions)
		{
			if (!entityWithFaction->isActive())
				continue;

			const auto& entityFaction = entityWithFaction->getComponent<FactionComponent>();
			if (!aiActorFaction->checkIfFactionHostile(entityFaction->getFaction()))
				continue;

			const auto& playerTransform = entityWithFaction->getComponent<TransformComponent>();
			const auto distance = Vector2D::Distance(playerTransform->getPosition(), aiActorTransform->getPosition());

			if (!enemyHasTarget)
			{
				if (distance < aiActorAI->getAggroDistance())
					aiActorAI->setNewTarget(entityWithFaction);
			}
			else if (enemyHasTarget)
			{
				if (distance < aiActorAI->getAggroDistance() && distance < aiActorAI->getDistance())
					aiActorAI->setNewTarget(entityWithFaction);
			}
		}
	}
}

void AISystem::draw()
{
}