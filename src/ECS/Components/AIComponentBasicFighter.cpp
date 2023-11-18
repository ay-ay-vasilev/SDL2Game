#include "AIComponentBasicFighter.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "DamageColliderComponent.h"
// TODO: remove
#include "Constants.h"

ecs::AIComponentBasicFighter::AIComponentBasicFighter() :
	target(nullptr),
	targetID(-1),
	distance(0.f), aggroDistance(0.f), loseAggroDistance(0.f),
	state(eState::IDLE) {}

void ecs::AIComponentBasicFighter::init()
{
	actorComponent = entity->getComponent<ActorComponent>();
	registerWithSubject(actorComponent);

	transform = entity->getComponent<TransformComponent>();
	damageCollider = entity->getComponent<DamageColliderComponent>();
	health = entity->getComponent<HealthComponent>();
	// TODO: remove
	auto& constants = constants::Constants::Instance();
	aggroDistance = std::any_cast<float>(constants.Get("ai_aggro_distance"));
	loseAggroDistance = std::any_cast<float>(constants.Get("ai_deaggro_distance"));
}

void ecs::AIComponentBasicFighter::update(double delta)
{
	if (health->isDead()) return;

	if (target)
	{
		distance = (Vector2D::Distance(transform->getPosition(), targetTransform->getPosition()));

		transform->setVeloctiy(0, 0);
		damageCollider = entity->getComponent<DamageColliderComponent>();

		if (damageCollider->isInRange(targetHitbox->getHitbox()) && state != eState::ATTACK)
		{
			transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
			actorComponent->playAction("attack");
			state = eState::ATTACK;
		}
		else if (state != eState::ATTACK)
		{
			transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
			transform->setVeloctiy(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
			actorComponent->playAction("walk");
			state = eState::WALK;
		}
	}
	else if (state != eState::ATTACK)
	{
		// default behavior
		actorComponent->playAction("idle");
	}
}

void ecs::AIComponentBasicFighter::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_end")
	{
		actorComponent->playAction("idle");
		state = eState::IDLE;
	}
	if (observedEvent == targetID + "_died")
	{
		if (!target) return;
		targetDestroyed();
	}
}

void ecs::AIComponentBasicFighter::setNewTarget(const Entity* newTarget)
{
	if (!newTarget || target == newTarget)
		return;

	target = newTarget;
	targetHealth = target->getComponent<HealthComponent>();
	if (targetHealth->isDead())
	{
		resetTarget();
		return;
	}

	registerWithSubject(targetHealth);
	targetTransform = target->getComponent<TransformComponent>();
	targetHitbox = target->getComponent<HitboxComponent>();
	targetID = static_cast<int>(target->getID());
	distance = (Vector2D::Distance(transform->getPosition(), targetTransform->getPosition()));
}

void ecs::AIComponentBasicFighter::resetTarget()
{
	targetHealth.reset();
	targetTransform.reset();
	targetHitbox.reset();
	target = nullptr;
	targetID = -1;

	transform->setVeloctiy(0, 0);
	distance = 0.f;
}

void ecs::AIComponentBasicFighter::loseTarget()
{
	resetTarget();
}

void ecs::AIComponentBasicFighter::targetDestroyed()
{
	resetTarget();
}