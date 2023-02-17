#pragma once
#include <SDL_pixels.h>

class Constants
{
public:

	// ECS
	static inline const int MAX_COMPONENTS{ 32 };
	static inline const int MAX_GROUPS{ 32 };

	// game settings
	static inline const int SCREEN_WIDTH {800};
	static inline const int SCREEN_HEIGHT {640};
	static inline const float SCALE {6.f};

	// map settings - todo: maps will have different sizes
	static inline const int MAP_TILE_WIDTH {16};
	static inline const int MAP_TILE_HEIGHT {16};
	static inline const int TILE_SIZE {32};

	// projectile settings - todo: projectiles will have different size and range
	static inline const int PROJECTILE_SIZE {32};
	static inline const int PROJECTILE_RANGE {200};

	// colors
	static inline const SDL_Color WHITE {255, 255, 255, 255};

	// fonts
	static inline const int DEBUG_FONT_SIZE {16};

	// player
	static inline const int PLAYER_WIDTH { 16 };
	static inline const int PLAYER_HEIGHT{ 16 };
	static inline const int PLAYER_SPEED { 5 };

};