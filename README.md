# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Containers for UI elements
3. Scene manager to "switch" levels

My plan:
- Ranged weapon for player.
- Combat overhaul (make it fun - add defensive abilities (i-frames, dodge rolls, parries), attack movement, combos etc.)
- Add a simple menu screen + UI system.
- Add simple sounds.
- Add an inventory system.
- Add skills.
- Add rotate to Transform ?
- Improve movement and controls.
- Default behavior for enemies.

Maintenance:
- Set proper surface sizes (unarmed bug)
- Fix animations (attack animation morphs into idle for a frame)
- Fix scale issues with particles and splatters (if broken)
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Better WeaponComponent implementation (add an object for weapon stats).
- Specific weapon stats for actors in weapon json files?
- Refactor ECS.h (uninline some methods, inline explicitly the rest)
- Refactor the debug draw methods
- Deal with strings and string_views
- Use aliases to make stuff readable
- CMake, cotire, CPM ?

Future plans:
- Add a second container for Sprites that should not be outlined? (sword swings)
- Looting
- Inventory system
- Hit feedback
- Hit cooldown ?
- Add body parts and body parts damage and armor ?
- More animations for AI actors ?
- Add armor condition ?