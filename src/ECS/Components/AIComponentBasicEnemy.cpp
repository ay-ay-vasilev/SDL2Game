#include "AIComponentBasicEnemy.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"

AIComponentBasicEnemy::AIComponentBasicEnemy() :
	target(nullptr),
	distance(0.f), aggroDistance(0.f), loseAggroDistance(0.f),
	state(eState::IDLE) {}

void AIComponentBasicEnemy::init()
{
	actorComponent = entity->getComponent<ActorComponent>();
	registerWithSubject(actorComponent);
	transform = entity->getComponent<TransformComponent>();
	weapon = entity->getComponent<WeaponComponent>();
	// todo: read from json
	aggroDistance = 80 * transform->getScale();
	loseAggroDistance = 100 * transform->getScale();
}

void AIComponentBasicEnemy::update()
{
	if (target)
	{
		distance = (Vector2D::Distance(transform->getPosition(), targetTransform->getPosition()));

		transform->setVeloctiy(0, 0);
		auto lockedWeapon = weapon.lock();
		if (!lockedWeapon)
		{
			lockedWeapon = entity->getComponent<WeaponComponent>();
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

void AIComponentBasicEnemy::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_end")
	{
		actorComponent->playAction("idle");
		state = eState::IDLE;
	}
	if (observedEvent == targetID + "_died")
	{
		targetDestroyed();
	}
}

void AIComponentBasicEnemy::setNewTarget(const Entity* newTarget)
{
	if (!newTarget || target == newTarget)
		return;
	target = newTarget;
	targetHealth = target->getComponent<HealthComponent>();
	registerWithSubject(targetHealth);
	targetTransform = target->getComponent<TransformComponent>();
	targetHitbox = target->getComponent<HitboxComponent>();
	targetID = target->getID();
}

void AIComponentBasicEnemy::resetTarget()
{
	targetHealth.reset();
	targetTransform.reset();
	targetHitbox.reset();
	target = nullptr;
	targetID = -1;

	transform->setVeloctiy(0, 0);
	distance = 0.f;

	std::cout << actorComponent->getActorType() << "_" << entity->getID() << " AI: target lost!\n";
}

void AIComponentBasicEnemy::loseTarget()
{
	resetTarget();
}

void AIComponentBasicEnemy::targetDestroyed()
{
	resetTarget();
}