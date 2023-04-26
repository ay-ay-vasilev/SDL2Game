#include "FactionSystem.h"
#include "AssetManager.h"

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

}

void FactionSystem::draw()
{

}