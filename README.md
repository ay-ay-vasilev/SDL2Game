# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Health
3. Shooting tower
4. Containers for UI elements
5. Scene manager to "switch" levels

My plan:
- Add random customization for actors
    - Add a RandomEquipment Component/System/Manager ?
    - Add random appearance features

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
- Remove unused CameraSystem and add CameraComponent (after RenderSystem refactor)
- Refactor KeyboardComponent (make it a system instead ?)
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update).
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?
- Specific weapon stats for actors in weapon json files?
- Refactor ECS.h (uninline some methods, inline explicitly the rest)
- Refactor the debug draw methods
- Refactor FactionSystem (make it a manager instead ? avoid duplication but worsen encapsulation)

Future plans:
- Add a second container for Sprites that should not be outlined? (sword swings)
- Customization for characters
- Actor randomization
- Player randomization
- Looting
- Inventory system
- Death animation
- Hit feedback
- Hit cooldown ?
- Add body parts and body parts damage and armor ?
- More animations for AI actors ?
- Divide pants into boots and pants ?
- Add armor condition ?