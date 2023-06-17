# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Containers for UI elements
3. Scene manager to "switch" levels

My plan:
- Ranged weapon for player.
    * Draw goblin sprites DONE
    * Parse spritesheets DONE
    * Added the bow weapon DONE
    * Add an arrow projectile
        - Add an arrow sprite DONE
        - Velocity and sprite logic
    * Add a pointer for aiming
    * Add shooting
    * Make it prettier
    * Add a ranged weapon to the human enemy
    * Add a ranged weapon to the skeleton enemy
- Combat overhaul (make it fun - add defensive abilities (i-frames, dodge rolls, parries), attack movement, combos etc.)
- Add enemy classes - warrior, rogue, ranger
- Improve combat AI
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
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Better WeaponComponent implementation (add an object for weapon stats).
- Specific weapon stats for actors in weapon json files?
- Refactor the debug draw methods
- Deal with strings and string_views
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