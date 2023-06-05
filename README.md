# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Containers for UI elements
3. Scene manager to "switch" levels

My plan:
- Add corpses and death animations
	- Blood particles
		* Add a ParticleSystem DONE
		* Add a ParticleManager to manage and parse particles and their types DONE
		* Change ParticleManager to a ParticleEmitter class and make ParticleManager hold a vector of emitters DONE
		* Add a ParticleComponent that communicates with the ParticleManager DONE
		* Optimize particles (better emitter deletion + a dedicated thread for particle calculations ? )
	- Blood pools
- Ranged weapon for player.
- Add a simple menu screen + UI system.
- Add simple sounds.
- Add an inventory system.
- Add skills.
- Add rotate to Transform.
- Improve movement and controls.
- Tweak attack movement.
- Implement attack combos, remove attack holding ?
- Default behavior for enemies.

Maintenance:
- Set proper surface sizes (unarmed bug)
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?
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