#include "AIComponentBasicFighter.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "WeaponMeleeComponent.h"
// TODO: remove
#include "Game.h"

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
	weapon = entity->getComponent<WeaponMeleeComponent>();
	health = entity->getComponent<HealthComponent>();
	// TODO: remove
	aggroDistance = Game::constants->AI_AGGRO_DISTANCE * transform->getScale();
	loseAggroDistance = Game::constants->AI_DEAGGRO_DISTANCE * transform->getScale();
}

void ecs::AIComponentBasicFighter::update(double delta)
{
	if (health->isDead()) return;

	if (target)
	{
		distance = (Vector2D::Distance(transform->getPosition(), targetTransform->getPosition()));

		transform->setVeloctiy(0, 0);
		auto lockedWeapon = weapon.lock();
		if (!lockedWeapon)
		{
			lockedWeapon = entity->getComponent<WeaponMeleeComponent>();
			weapon = lockedWeapon;
		}
		if (!lockedWeapon)
			return;

		if (lockedWeapon->isInRange(targetHitbox->getHitbox()) && state != eState::ATTACK)
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
	targetID = target->getID();
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