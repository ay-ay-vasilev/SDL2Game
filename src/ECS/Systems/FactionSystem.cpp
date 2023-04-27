#include "FactionSystem.h"
#include "AssetManager.h"
#include "FactionComponent.h"

void FactionSystem::init()
{
	const auto factionData = Game::assets->getGeneralDataJson("factions")["factions"];
	for (const auto faction : factionData)
	{
		std::unordered_map<std::string, int> relations;
		const auto relationsData = faction["relations"];
		for (const auto relation : relationsData)
		{
			relations[relation["faction"]] = relation["value"];
		}
		factions[faction["id"]] = relations;
	}
}

void FactionSystem::update()
{
	updateRelations();
}

void FactionSystem::draw()
{
}

void FactionSystem::updateRelations()
{
	auto entitiesWithFactions = manager.getEntitiesWithComponent<FactionComponent>();

	for (const auto entityWithFaction : entitiesWithFactions)
	{
		auto factionComponent = entityWithFaction->getComponent<FactionComponent>();
		
		if (factions.count(factionComponent->getFaction()))
		{
			for (const auto [factionName, relationVal] : factions[factionComponent->getFaction()])
				factionComponent->setFactionRelation(factionName, relationVal);
		}
	}
}