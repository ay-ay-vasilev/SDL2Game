#include "AISystem.h"
#include "AIComponentBasicFighter.h"
#include "FactionComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"

#include "Game.h"

void ecs::AISystem::update(double delta)
{
	entitiesWithFactions = manager.getEntitiesWithComponents<FactionComponent>();
	aiActors = manager.getEntitiesWithComponent<AIComponentBasicFighter>();

	for (const auto& aiActor : aiActors)
	{
		const auto& aiActorHealth = aiActor->getComponent<HealthComponent>();
		if (aiActorHealth && aiActorHealth->isDead()) continue;

		const auto& aiActorTransform = aiActor->getComponent<TransformComponent>();
		const auto& aiActorAI = aiActor->getComponent<AIComponentBasicFighter>();
		const auto& aiActorFaction = aiActor->getComponent<FactionComponent>();

		bool fighterHasTarget = aiActorAI->hasTarget();

		if (aiActorAI->hasTarget() && aiActorAI->getDistance() > aiActorAI->getLoseAggroDistance())
		{
			aiActorAI->loseTarget();
			fighterHasTarget = false;
		}
		for (const auto& entityWithFaction : entitiesWithFactions)
		{
			if (!entityWithFaction->isActive()) continue;

			const auto& entityHealth = entityWithFaction->getComponent<HealthComponent>();
			if (entityHealth && entityHealth->isDead()) continue;

			const auto& entityFaction = entityWithFaction->getComponent<FactionComponent>();
			if (!aiActorFaction->checkIfFactionHostile(entityFaction->getFaction()))
				continue;

			const auto& entityTransform = entityWithFaction->getComponent<TransformComponent>();
			const auto distance = Vector2D::Distance(entityTransform->getPosition(), aiActorTransform->getPosition());

			if (!fighterHasTarget)
			{
				if (distance < aiActorAI->getAggroDistance())
				{
					aiActorAI->setNewTarget(entityWithFaction);
					fighterHasTarget = true;
				}
			}
			else if (fighterHasTarget)
			{
				if (distance < aiActorAI->getAggroDistance() && distance < aiActorAI->getDistance())
				{
					aiActorAI->setNewTarget(entityWithFaction);
				}
			}
		}
	}
}

void ecs::AISystem::draw()
{
}