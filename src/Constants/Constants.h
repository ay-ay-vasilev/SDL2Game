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
	std::any Get(const std::string& key) const;
	void Init(const std::string& fileName);
	void ReloadSettings();

private:
	// Service
	void InitDefaultValues();

	template <typename T>
	void Set(const std::string& key, const T& value);

	template <typename T>
	void SetFromJson(const std::string& key, const nlohmann::json& json);

	void SetPositionVectorFromJsonArray(const std::string& key, const nlohmann::json& json);
	void LoadFromJsonObject(const nlohmann::json& constantsJson);

	std::string settingsFile;
	std::unordered_map<std::string, std::any> config;

};

template <typename T>
T GetConstant(const Constants& constants, const std::string& key)
{
	return std::any_cast<T>(constants.Get(key));
}

}