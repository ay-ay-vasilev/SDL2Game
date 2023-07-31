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
    * Add an arrow projectile DONE
    * Add shooting (basic variant) DONE
    * Add a pointer for aiming DONE
    * Implement the creation of the projectiles DONE
    * Make it prettier DONE
    * Add a ranged weapon to the human enemy
        - Aseprite art DONE
        - Parse texture atlases ALMOST
        - Add a new projectile (normal sized arrow)
        - Add the weapon json and test with player ALMOST
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
- Fix the projectile memory leak
    - Rework the ECS from the ground up (keep the same interface)
- Refactor the KeyboardManager
- Refactor the Splatter class maybe ?
- Set proper surface sizes (unarmed bug)
- Fix animations (attack animation morphs into idle for a frame)
- JSON files for AI
- Stop loading JSON files after inits (create a data structure for holding all weapons and armor data)
- Add map json
- Fix Tile-Collider logic
- Clean up in constants and settings.json.
- Better WeaponComponent implementation (add an object for weapon stats)
- Better ProjectileComponent implementation (add a name and make it a layer of abstraction, add "splatters" and "corpses")
- Specific weapon stats for actors in weapon json files?
- Refactor the debug draw methods
- Multiple colliders in the DamageColliderComponent
- Explosive projectiles ?
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