// Actor.h
// 
// WHAT IS AN ACTOR?
// An Actor is ANYTHING that exists in your game world!
// - Player (you control)
// - Enemies (they chase you)
// - Items (health packs, power-ups)
// - Projectiles (bullets, fireballs)
// - Effects (explosions, particles)
//
// Think of Actor like a blank canvas - it has a position in the world
// and can be drawn on screen, but by itself it doesn't DO much.
// SPECIFIC actors (like Player, Enemy) add their own behavior!

#pragma once  // Prevents this file from being included multiple times
#ifndef ACTOR_H  // If ACTOR_H hasn't been defined yet...
#define ACTOR_H  // ...define it now (this is called an "include guard")

// Include raylib so we can use Vector2, Color, etc.
#include "raylib.h"
// Include string so we can give actors names (for debugging)
#include <string>

/**
 * Actor Class
 *
 * This is the BASE CLASS for EVERYTHING in your game.
 * Any class that inherits from Actor gets:
 * - A position in the world (where it is)
 * - A rotation (which way it's facing)
 * - A scale (how big it is)
 * - An active flag (whether it should update/draw)
 * - A name (for debugging)
 *
 * UNREAL CONNECTION:
 * This is exactly like Unreal Engine's AActor class!
 * In Unreal, everything that goes in a level is an Actor.
 */
class Actor {
public:
    // =========================================================================
    // CONSTRUCTOR & DESTRUCTOR
    // =========================================================================

    /**
     * Constructor - Runs when an actor is FIRST CREATED
     * Example: Player* guy = new Player();  ← This calls the constructor
     *
     * Sets up default values for position, rotation, etc.
     */
    Actor();

    /**
     * Destructor - Runs when an actor is DESTROYED
     * Example: delete guy;  ← This calls the destructor
     *
     * Virtual = allows child classes (Player, Enemy) to have their own destructors
     * that run AFTER this one
     */
    virtual ~Actor();

    // =========================================================================
    // UNREAL-STYLE LIFECYCLE METHODS
    // =========================================================================
    // These are called at specific times during the actor's life
    // They're "virtual" so child classes can override them with their own behavior

    /**
     * BeginPlay - Called ONCE when the actor first appears in the game
     *
     * Use this to:
     * - Set starting position
     * - Initialize variables
     * - Find other actors (like finding the player)
     * - Play spawn sounds/effects
     *
     * Example: Enemy appears at random position
     */
    virtual void BeginPlay();

    /**
     * Tick - Called EVERY FRAME while the actor is active
     * @param deltaTime Time since last frame (in seconds)
     *
     * Use this for things that change over time:
     * - Movement (player moving, enemies chasing)
     * - Health regeneration
     * - Cooldown timers
     * - AI thinking
     * - Animations
     *
     * Example: Player moves when arrow keys are pressed
     */
    virtual void Tick(float deltaTime);

    /**
     * Draw - Called EVERY FRAME to put the actor on screen
     *
     * Use this to:
     * - Draw shapes (circles, squares, rectangles)
     * - Draw sprites (images)
     * - Draw UI elements (health bars, names)
     * - Draw particle effects
     *
     * Example: Draw player as blue circle, enemy as red square
     */
    virtual void Draw();

    // =========================================================================
    // TRANSFORM METHODS (Position, Rotation, Scale)
    // =========================================================================
    // These control WHERE the actor is and how it's oriented

    /**
     * SetPosition - Moves the actor to a specific spot
     * @param newPos The new coordinates (x, y) in pixels
     *
     * Example: player.SetPosition({400, 300});  // Move to center of screen
     */
    void SetPosition(Vector2 newPos) {
        position = newPos;  // Store the new position
    }

    /**
     * GetPosition - Asks "where is this actor right now?"
     * @return The actor's current coordinates (x, y)
     *
     * Example: Vector2 playerPos = player.GetPosition();
     */
    Vector2 GetPosition() const {  // 'const' means this function won't change anything
        return position;  // Give back the current position
    }

    // NOTE: We'll add rotation and scale getters/setters later if needed!

    // =========================================================================
    // BASIC PROPERTIES
    // =========================================================================

    /**
     * SetActive - Turns the actor ON or OFF
     * @param isActive true = actor updates and draws, false = actor does nothing
     *
     * Use this to:
     * - Pause enemies
     * - Hide objects
     * - Disable things temporarily
     *
     * Example: enemy.SetActive(false);  // Enemy freezes and disappears
     */
    void SetActive(bool isActive) {
        active = isActive;  // Store whether we're active or not
    }

    /**
     * IsActive - Checks if the actor is currently active
     * @return true if actor updates/draws, false if it's disabled
     *
     * Example: if (enemy.IsActive()) { enemy.Tick(); }
     */
    bool IsActive() const {  // 'const' means this function won't change anything
        return active;  // Tell the caller if we're active
    }

    // =========================================================================
    // NAME METHODS (Added so other classes can read the actor's name)
    // =========================================================================

    /**
     * GetName - Asks "what is this actor's name?"
     * @return The actor's name as a string (like "Player", "Enemy", "Goblin")
     *
     * Use this to identify what kind of actor this is
     *
     * Example: if (other->GetName() == "Player") { /* chase the player * / }
     */
    std::string GetName() const {
        return actorName;  // Give back the actor's name
    }

    /**
     * SetName - Gives the actor a name
     * @param newName What to call this actor
     *
     * Use this to identify different types of actors
     *
     * Example: actor.SetName("Health Pack");
     */
    void SetName(const std::string& newName) {
        actorName = newName;  // Store the new name
    }

    // =========================================================================
    // PROTECTED MEMBERS
    // =========================================================================
    // "protected" means:
    // - ONLY this class AND classes that INHERIT from it can access these directly
    // - Other classes (like GameMode, CombatComponent) MUST use public getters/setters
    //
    // Think of it like:
    // - public = front door (anyone can enter)
    // - protected = family room (Actor and its children only)
    // - private = bedroom (only Actor itself)

protected:
    // POSITION - Where the actor is in the world (x, y coordinates)
    // Measured in pixels from top-left corner of screen
    Vector2 position;

    // ROTATION - Which way the actor is facing (in degrees)
    // 0 = facing right, 90 = facing down, 180 = facing left, 270 = facing up
    float rotation;

    // SCALE - How big the actor is (1.0 = normal size)
    // 2.0 = double size, 0.5 = half size
    Vector2 scale;

    // ACTIVE - Whether the actor should update and draw
    // true = actor is alive and doing things
    // false = actor is disabled (won't move, won't draw)
    bool active;

    // ACTOR NAME - What kind of actor this is (for identification)
    // Examples: "Player", "Enemy", "HealthPack", "Projectile"
    // Used to figure out what we're colliding with
    std::string actorName;
};

#endif // End of include guard