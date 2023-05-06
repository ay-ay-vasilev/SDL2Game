#include "ActorSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "ActorComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "AIComponentBasicFighter.h"
#include "ArmorComponent.h"
#include "WeaponComponent.h"
#include "FactionComponent.h"

#include <random>

void ecs::ActorSystem::init()
{
	const auto actorsEquipmentData = Game::assets->getGeneralDataJson("actor_equipment");
	for (auto& actorEntry : actorsEquipmentData["armor"].items())
	{
		const std::string actorName = actorEntry.key();
		std::unordered_map<std::string, std::vector<std::string>> actorEquipmentMap;

		for (auto& equipmentEntry : actorEntry.value().items())
		{
			const std::string slotName = equipmentEntry.key();
			const std::vector<std::string>& equipmentList = equipmentEntry.value();
			actorEquipmentMap.emplace(slotName, equipmentList);
		}
		actorArmors.emplace(actorName, actorEquipmentMap);
	}

	for (auto& [actorName, weaponVector] : actorsEquipmentData["weapons"].items())
	{
		actorWeapons.emplace(actorName, weaponVector);
	}

	const auto actorsCustomizationData = Game::assets->getGeneralDataJson("customization");
	for (auto& actorEntry : actorsCustomizationData.items())
	{
		const std::string actorName = actorEntry.key();
		for (const auto& sprite : actorEntry.value()["sprites"])
		{
			std::vector<std::string> textures;
			for (const auto& texture : sprite["textures"]) textures.push_back(texture);
			auto customizationSprite = ActorCustomizationSprite(sprite["slot"], textures, sprite["z"]);
			actorCustomizations[actorName].push_back(customizationSprite);
		}
		for (const auto& colorMapping : actorEntry.value()["color_variations"].items())
		{
			const std::string colorName = colorMapping.key();
			colorMappings[actorName][colorName].push_back({});
			for (const auto& color : colorMapping.value())
			{
				const auto& srcColorData = color["source_color"];
				SDL_Color srcColor{ srcColorData[0], srcColorData[1], srcColorData[2], srcColorData[3] };
				
				const auto& targetColorData = color["target_color"];
				SDL_Color targetColor{ targetColorData[0], targetColorData[1], targetColorData[2], targetColorData[3] };

				colorMappings[actorName][colorName].push_back({ srcColor, targetColor });
			}
		}
	}
}

void ecs::ActorSystem::update(double delta)
{
	actors = manager.getGroup(Game::eGroupLabels::ACTORS);
}

void ecs::ActorSystem::draw()
{
}

ecs::Entity* ecs::ActorSystem::instantiateActor(const Vector2D& pos, const std::string& filename)
{
	const auto actorData = Game::assets->getActorJson(filename);
	auto& actor(manager.addEntity());
	actor.addComponent<ecs::TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		actorData.value("width", 0), actorData.value("height", 0),
		manager.getScale(), actorData["speed"]
	);
	actor.addComponent<ecs::SpriteComponent>(actorData["sprite_data"], true);
	actor.addComponent<ecs::SpriteOutlineComponent>(actorData["sprite_data"].contains("outline") ? actorData["sprite_data"]["outline"] : nullptr);
	actor.addComponent<ecs::ActorComponent>(filename);
	actor.addComponent<ecs::ColliderComponent>(filename, actorData["collider_rect"]);
	actor.addComponent<ecs::HitboxComponent>(filename, actorData["hitbox_rect"]);
	actor.addComponent<ecs::HealthComponent>(actorData["health"]);
	actor.addComponent<ecs::ArmorComponent>();
	actor.addComponent<ecs::AIComponentBasicFighter>();
	actor.addComponent<ecs::FactionComponent>(actorData.contains("faction") ? actorData["faction"] : "neutral");

	actor.addGroup(Game::eGroupLabels::ACTORS);

	return &actor;
}

void ecs::ActorSystem::equipWeapon(ecs::Entity& actor, const std::string& weaponName)
{
	if (!actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();

	if (actor.hasComponent<ecs::WeaponComponent>())
	{
		if (actor.getComponent<ecs::WeaponComponent>()->getTag() == weaponName) return;
		actor.removeComponent<ecs::WeaponComponent>();
	}
	actor.addComponent<ecs::WeaponComponent>(weaponName, actorType);
}

void ecs::ActorSystem::equipArmor(ecs::Entity& actor, const std::string& armorName, const std::string& slotName)
{
	if (!actor.hasComponent<ecs::ActorComponent>() || !actor.hasComponent<ecs::ArmorComponent>()) return;
	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();

	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
	armorComponent->equipArmorToSlot(armorName, actorType, slotName);
}

void ecs::ActorSystem::unequipArmorPiece(ecs::Entity& actor, const std::string& slotName)
{
	if (!actor.hasComponent<ecs::ArmorComponent>()) return;

	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
	armorComponent->unequipArmorFromSlot(slotName);
}

void ecs::ActorSystem::unequipAllArmor(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ArmorComponent>()) return;

	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
	armorComponent->unequipAllArmor();
}

void ecs::ActorSystem::equipRandomArmor(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ArmorComponent>() || !actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();
	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();

	std::random_device rd;
	std::mt19937 gen(rd());

	for (const auto& [slotName, equipmentVector] : actorArmors[actorType])
	{
		if (!equipmentVector.empty())
		{
			std::uniform_int_distribution<> dis(0, equipmentVector.size() - 1);
			const int equipmentIndex = dis(gen);
			const std::string& equipmentName = equipmentVector[equipmentIndex];
			if (equipmentName != "none")
			{
				armorComponent->equipArmorToSlot(equipmentName, actorType, slotName);
			}
		}
	}
}

void ecs::ActorSystem::equipRandomWeapon(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();
	if (actorWeapons[actorType].empty()) return;

	const auto weaponsVector = actorWeapons[actorType];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, weaponsVector.size() - 1);

	const int weaponIndex = dis(gen);
	const auto weaponName = weaponsVector[weaponIndex] == "none" ? "unarmed" : weaponsVector[weaponIndex];

	if (actor.hasComponent<ecs::WeaponComponent>())
	{
		if (actor.getComponent<ecs::WeaponComponent>()->getTag() == weaponName) return;
		actor.removeComponent<ecs::WeaponComponent>();
	}
	actor.addComponent<ecs::WeaponComponent>(weaponName, actorType);
}

void ecs::ActorSystem::addRandomCustomization(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ActorComponent>() || !actor.hasComponent<ecs::SpriteComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();
	const auto spriteComponent = actor.getComponent<ecs::SpriteComponent>();

	const auto& customization = actorCustomizations[actorType];
	const auto& colorVariations = colorMappings[actorType];

	if (customization.empty()) return;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dist(0, colorVariations.size() - 1);
	int index = dist(gen);

	// select the random color variation
	auto it = std::begin(colorVariations);
	std::advance(it, index);
	const auto& randomColorVariation = it->second;

	for (const auto& sprite : customization)
	{
		std::uniform_int_distribution<> surfacesDistribution(0, sprite.availableSurfaces.size() - 1);
		const auto surfaceId = sprite.availableSurfaces[surfacesDistribution(gen)];
		spriteComponent->addSprite(sprite.slotName, std::make_shared<Sprite>(surfaceId, sprite.z, randomColorVariation));
	}
}