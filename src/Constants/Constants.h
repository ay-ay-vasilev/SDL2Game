#pragma once
#include "Vector2D.h"

#include <wrappedJson.h>
#include <string>
#include <any>
#include <unordered_map>

namespace constants
{

struct ProjectileData
{
	Vector2D pos;
	Vector2D velocity;
};

class Constants
{
public:

	static Constants& Instance()
	{
		static Constants instance;
		return instance;
	}

private:
	Constants() { InitDefaultValues(); }

public:
	Constants(const Constants&) = delete;
	Constants& operator=(const Constants&) = delete;
	Constants(Constants&&) = delete;
	Constants& operator=(Constants&&) = delete;

	// Interface
	std::any Get(const std::string& key) const
	{
		auto it = config.find(key);
		return (it != config.end()) ? it->second : std::any();
	}

	void Init(const std::string& fileName);
	void ReloadSettings();

private:
	// Service
	void InitDefaultValues();
	template <typename T>
	void Set(const std::string& key, const T& value)
	{
		config[key] = value;
	}

	template <typename T>
	void SetFromJson(const std::string& key, const nlohmann::json& json)
	{
		auto it = json.find(key);
		if (it != json.end())
			Set(key, it->get<T>());
	}

	void SetPositionVectorFromJsonArray(const std::string& key, const nlohmann::json& json)
	{
		if (auto it = json.find(key); it != json.end())
		{
			std::vector<Vector2D> values;
			for (const auto& data : *it)
				values.push_back({ data["x"], data["y"] });
			Set(key, values);
		}
	}

	// helper function to load constants from JSON object
	void LoadFromJsonObject(const nlohmann::json& constantsJson);

	std::string settingsFile;
	std::unordered_map<std::string, std::any> config;

};

}