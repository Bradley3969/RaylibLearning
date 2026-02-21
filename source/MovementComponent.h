#pragma once
#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include "raylib.h"
#include <cmath>  // For std::sqrt

class MovementComponent {
public:
    // CONSTRUCTOR
    // The ": something(value)" syntax is called an "initializer list"
    // It sets up variables BEFORE the constructor body runs
    // This is MORE EFFICIENT than setting them inside the constructor!
    MovementComponent(float initialSpeed = 100.0f)  // Default speed 100 pixels/sec
        : speed(initialSpeed)        // Initialize speed with the provided value
        , maxSpeed(initialSpeed)      // Set max speed to initial speed by default
        , velocity({ 0, 0 })          // Initialize velocity to zero (not moving)
        , moveDirection({ 0, 0 })     // Initialize move direction to zero
        , isMoving(false)              // Initialize isMoving to false
        , isBoosted(false)             // Initialize boost status
        , originalSpeed(initialSpeed)  // Store original speed
        , boostTimeRemaining(0.0f)     // No boost active
    {
        // Constructor body is empty because initializer list did all the work
        // This is perfectly fine and actually preferred!
    }

    // DESTRUCTOR
    // virtual = allows child classes to override
    // = default = use the automatically generated version
    virtual ~MovementComponent() = default;

    // ------------------------------------------------------------------------
    // METHOD 1: Move based on input direction (for player)
    // ------------------------------------------------------------------------

    /**
     * Moves an actor based on a direction vector (like keyboard input)
     *
     * @param direction Which way to move (normalized automatically)
     * @param deltaTime Time since last frame (prevents speed tied to framerate)
     * @param currentPos Where the actor is right now
     * @return The NEW position after moving
     */
    Vector2 MoveInDirection(Vector2 direction, float deltaTime, Vector2 currentPos)
    {
        // Save direction for later use (animations, etc.)
        moveDirection = direction;

        // Calculate how long the direction vector is
        // If length is near zero, we're not pressing any keys
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0.1f)  // We ARE trying to move
        {
            isMoving = true;

            // NORMALIZE the direction vector (make its length = 1)
            // Without this, moving diagonally would be 1.4x faster!
            direction.x /= length;
            direction.y /= length;

            // Calculate velocity (direction × speed)
            velocity.x = direction.x * speed;
            velocity.y = direction.y * speed;

            // Apply velocity to current position
            currentPos.x += velocity.x * deltaTime;
            currentPos.y += velocity.y * deltaTime;
        }
        else  // Not trying to move
        {
            isMoving = false;
            velocity = { 0, 0 };  // Stop moving
            // position stays the same - we don't change it
        }

        // Return the new position (whether it changed or not)
        return currentPos;
    }

    // ------------------------------------------------------------------------
    // METHOD 2: Move toward a target (for AI enemies)
    // ------------------------------------------------------------------------

    /**
     * Moves an actor toward a target position (like enemy chasing player)
     *
     * @param currentPos Where the actor is right now
     * @param targetPos Where we want to go (usually player position)
     * @param deltaTime Time since last frame
     * @return The NEW position after moving closer to target
     */
    Vector2 MoveToward(Vector2 currentPos, Vector2 targetPos, float deltaTime)
    {
        // Calculate direction to target
        // Vector math: target - current = direction from current to target
        float dx = targetPos.x - currentPos.x;
        float dy = targetPos.y - currentPos.y;

        // Calculate distance to target using Pythagorean theorem
        // distance = √(dx² + dy²)
        float distance = std::sqrt(dx * dx + dy * dy);

        // Only move if we're not already on top of the target
        if (distance > 1.0f)  // 1 pixel threshold prevents jittering
        {
            isMoving = true;

            // NORMALIZE the direction (make it length 1)
            // This gives us a pure direction without distance
            float dirX = dx / distance;
            float dirY = dy / distance;

            // Save for animations
            moveDirection = { dirX, dirY };

            // Calculate velocity
            velocity.x = dirX * speed;
            velocity.y = dirY * speed;

            // !!! IMPORTANT FIX: Actually MOVE the position !!!
            currentPos.x += velocity.x * deltaTime;
            currentPos.y += velocity.y * deltaTime;

            // OPTIONAL: Prevent overshooting the target
            // Check if we would pass the target this frame
            float newDx = targetPos.x - currentPos.x;
            float newDy = targetPos.y - currentPos.y;
            float newDistance = std::sqrt(newDx * newDx + newDy * newDy);

            // If we're now closer than we were, but we passed the target
            if (newDistance > distance) {
                // We overshot! Just set position exactly to target
                currentPos = targetPos;
                velocity = { 0, 0 };
                isMoving = false;
            }
        }
        else  // Already at target (or very close)
        {
            isMoving = false;
            velocity = { 0, 0 };
            moveDirection = { 0, 0 };
            // Don't change position - we're already there!
        }

        // RETURN the new position (FIXED: was missing!)
        return currentPos;
    }

    // ------------------------------------------------------------------------
    // METHOD 3: Stop moving
    // ------------------------------------------------------------------------

    /**
     * Immediately stops all movement
     */
    void Stop()
    {
        velocity = { 0, 0 };
        isMoving = false;
        moveDirection = { 0, 0 };
    }

    // ------------------------------------------------------------------------
    // METHOD 4: Temporary speed boost (for power-ups)
    // ------------------------------------------------------------------------

    /**
     * Temporarily increases speed
     *
     * @param multiplier How much to boost (2.0 = double speed)
     * @param duration How many seconds the boost lasts
     */
    void TemporaryBoost(float multiplier, float duration)
    {
        // Store original speed if not already boosted
        if (!isBoosted) {
            originalSpeed = speed;
            isBoosted = true;
        }

        // Apply boost
        speed = originalSpeed * multiplier;
        boostTimeRemaining = duration;

        // Cap at max speed if needed
        if (speed > maxSpeed) {
            speed = maxSpeed;
        }
    }

    // ------------------------------------------------------------------------
    // METHOD 5: Update (call every frame)
    // ------------------------------------------------------------------------

    /**
     * Updates timers and effects (call this every frame in the Actor's Tick)
     *
     * @param deltaTime Time since last frame
     */
    void Update(float deltaTime)
    {
        // Handle boost timer
        if (isBoosted) {
            boostTimeRemaining -= deltaTime;
            if (boostTimeRemaining <= 0.0f) {
                // Boost ended, revert to original speed
                speed = originalSpeed;
                isBoosted = false;
            }
        }
    }

    // ------------------------------------------------------------------------
    // GETTERS & SETTERS - Controlled access to private variables
    // ------------------------------------------------------------------------

    /// Get current speed
    float GetSpeed() const { return speed; }

    /// Set new speed (can't be negative)
    void SetSpeed(float newSpeed) {
        if (newSpeed > 0) {
            speed = newSpeed;
        }
    }

    /// Get current velocity
    Vector2 GetVelocity() const { return velocity; }

    /// Get movement direction (for animations)
    Vector2 GetMoveDirection() const { return moveDirection; }

    /// Check if actor is moving
    bool IsMoving() const { return isMoving; }

    /// Get speed as percentage of max
    float GetSpeedPercent() const {
        return speed / maxSpeed;
    }

    /// Check if currently boosted
    bool IsBoosted() const { return isBoosted; }

private:
    // ------------------------------------------------------------------------
    // PRIVATE VARIABLES
    // ------------------------------------------------------------------------

    float speed;              // Current movement speed (pixels per second)
    float maxSpeed;           // Maximum allowed speed (for boosts)
    Vector2 velocity;         // Current velocity (direction × speed)
    Vector2 moveDirection;    // Last movement direction (for animations)
    bool isMoving;            // Are we currently moving?

    // Boost-related variables
    bool isBoosted;           // Are we currently boosted?
    float originalSpeed;      // Speed before boost (to revert)
    float boostTimeRemaining; // How many seconds of boost left
};

#endif // MOVEMENTCOMPONENT_H