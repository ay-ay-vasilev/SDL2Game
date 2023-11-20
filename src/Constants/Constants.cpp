#include "Constants.h"
#include "Vector2D.h"

#include <SDL_pixels.h>
#include <fstream>

namespace constants
{

void Constants::Init(const std::string& fileName)
{
	settingsFile = fileName;

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Failed to open constants file: " << fileName << std::endl;
		return;
	}

	nlohmann::json constantsJson;
	file >> constantsJson;
	LoadFromJsonObject(constantsJson);
}

void Constants::ReloadSettings()
{
	std::ifstream file(settingsFile);
	if (!file.is_open())
	{
		std::cerr << "Failed to open constants file: " << settingsFile << std::endl;
		return;
	}

	nlohmann::json constantsJson;
	file >> constantsJson;
	LoadFromJsonObject(constantsJson);
}

std::any Constants::Get(const std::string& key) const
{
	auto it = config.find(key);
	return (it != config.end()) ? it->second : std::any();
}

template <typename T>
void Constants::Set(const std::string& key, const T& value)
{
	config[key] = value;
}

template <typename T>
void Constants::SetFromJson(const std::string& key, const nlohmann::json& json)
{
	auto it = json.find(key);
	if (it != json.end())
		Set(key, it->get<T>());
}

void Constants::SetPositionVectorFromJsonArray(const std::string& key, const nlohmann::json& json)
{
	if (auto it = json.find(key); it != json.end())
	{
		std::vector<Vector2D> values;
		for (const auto& data : *it)
			values.push_back({ data["x"], data["y"] });
		Set(key, values);
	}
}

void Constants::LoadFromJsonObject(const nlohmann::json& constantsJson)
{
	SetFromJson<int>("screen_width", constantsJson);
	SetFromJson<int>("screen_height", constantsJson);
	SetFromJson<bool>("fullscreen", constantsJson);
	SetFromJson<float>("scale", constantsJson);

	SetFromJson<std::string>("window_title", constantsJson);
	SetFromJson<int>("map_tile_width", constantsJson);
	SetFromJson<int>("map_tile_height", constantsJson);
	SetFromJson<int>("tile_size", constantsJson);

	SetFromJson<int>("projectile_size", constantsJson);
	SetFromJson<int>("projectile_range", constantsJson);

	SetFromJson<int>("debug_font_size", constantsJson);
	SetFromJson<std::string>("player_race", constantsJson);

	if (auto it = constantsJson.find("player_pos"); it != constantsJson.end())
		Set<Vector2D>("player_pos", { (*it)["x"], (*it)["y"] });

	SetPositionVectorFromJsonArray("human_positions", constantsJson);
	SetPositionVectorFromJsonArray("skeleton_positions", constantsJson);
	SetPositionVectorFromJsonArray("goblin_positions", constantsJson);

	if (constantsJson.find("debug") != constantsJson.end())
	{
		const auto debugData = constantsJson["debug"];

		if (auto it = debugData.find("debug_projectiles"); it != debugData.end())
		{
			std::vector<ProjectileData> values;
			for (const auto& data : *it)
			{
				const auto& pos = data["pos"];
				const auto& vel = data["velocity"];
				values.push_back({ { pos["x"], pos["y"] }, { vel["x"], vel["y"] } });
			}
			Set("debug_projectiles", values);
		}

		SetFromJson<bool>("debug_draw_colliders", debugData);
		SetFromJson<bool>("debug_draw_hitboxes", debugData);
		SetFromJson<std::string>("debug_particle", debugData);
	}

	SetFromJson<float>("ai_aggro_distance", constantsJson);
	SetFromJson<float>("ai_deaggro_distance", constantsJson);
	SetFromJson<double>("time_scale", constantsJson);
}

void Constants::InitDefaultValues()
{
	Set<int>("screen_width", 800);
	Set<int>("screen_height", 800);
	Set<bool>("fullscreen", false);
	Set<float>("scale", 6.f);
	Set<std::string>("window_title", "Game Window");

	Set<int>("map_tile_width", 16);
	Set<int>("map_tile_height", 16);
	Set<int>("tile_size", 32);

	Set<int>("projectile_size", 32);
	Set<int>("projectile_range", 200);

	Set<SDL_Color>("color_white", { 255, 255, 255, 255 });

	Set<int>("debug_font_size", 16);

	Set<std::vector<Vector2D>>("human_positions", {});
	Set<std::vector<Vector2D>>("skeleton_positions", {});
	Set<std::vector<Vector2D>>("goblin_positions", {});

	Set<bool>("draw_colliders", false);
	Set<bool>("draw_hitboxes", false);
	Set<std::vector<Vector2D>>("debug_projectiles", {});
	Set<std::string>("debug_particle", "");

	Set<float>("ai_aggro_distance", 80.f);
	Set<float>("ai_deaggro_distance", 80.f);

	Set<double>("time_scale", 60.0);
}

}