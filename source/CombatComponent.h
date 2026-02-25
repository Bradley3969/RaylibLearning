// CombatComponent.h
// 
// WHAT IS A COMBAT COMPONENT?
// This handles damage dealing when two actors touch!
// Think of it like:
// - If a sword touches an enemy → enemy takes damage
// - If an enemy touches player → player takes damage
// - If a fireball touches anything → BOOM!
//
// WHY A COMPONENT?
// Because ANYTHING can be combat-capable:
// - Player (can hurt enemies)
// - Enemies (can hurt player)
// - Projectiles (can hurt both)
// - Traps (can hurt player)
// - Power-ups (can heal instead of hurt)

#pragma once
#ifndef COMBATCOMPONENT_H
#define COMBATCOMPONENT_H

#include "raylib.h"
#include "Actor.h"  // We need Actor for owner pointer
#include <cmath>   // For sqrt in knockback calculation
/**
 * CombatComponent
 *
 * Attach this to any Actor that can deal damage to other Actors.
 *
 * Examples:
 * - Player's sword → damageEnemy = true, damagePlayer = false
 * - Enemy body → damagePlayer = true, damageEnemy = false
 * - Explosion → damageEveryone = true
 * - Health pack → healAmount = positive
 */
class CombatComponent {
public:
    // CONSTRUCTOR
    // Sets up default combat values
    CombatComponent()
        : owner(nullptr)           // Who owns this component? (set later)
        , damageAmount(10.0f)      // How much damage we deal (default 10)
        , healAmount(0.0f)          // If positive, heals instead of hurts
        , damagePlayer(true)        // Can we hurt the player?
        , damageEnemy(true)          // Can we hurt enemies?
        , damageSelf(false)          // Can we hurt ourselves? (usually false)
        , damageNeutral(true)        // Can we hurt neutral things?
        , knockbackForce(100.0f)     // How hard we push when hitting
        , attackCooldown(0.5f)       // Seconds between attacks
        , currentCooldown(0.0f)      // Time until next attack
        , lastAttacker(nullptr)       // Who last hit us? (for revenge!)
    {
    }

    // DESTRUCTOR
    virtual ~CombatComponent() = default;

    // ------------------------------------------------------------------------
    // SETUP - Call this after creating the component
    // ------------------------------------------------------------------------

    /**
     * Initialize - Links component to its owner
     * @param ownerActor The Actor that owns this component
     */
    void Initialize(Actor* ownerActor) {
        owner = ownerActor;
    }

    // ------------------------------------------------------------------------
    // UPDATE - Call every frame
    // ------------------------------------------------------------------------

    /**
     * Update - Handles cooldown timers
     * @param deltaTime Time since last frame
     */
    void Update(float deltaTime) {
        // Count down attack cooldown
        if (currentCooldown > 0.0f) {
            currentCooldown -= deltaTime;
            if (currentCooldown < 0.0f) {
                currentCooldown = 0.0f;  // Can attack again!
            }
        }
    }

    // ------------------------------------------------------------------------
    // COLLISION HANDLING - Called when two actors touch
    // ------------------------------------------------------------------------

    /**
     * OnCollide - Called when owner touches another actor
     * @param other The other actor we touched
     * @return true if damage was dealt
     *
     * This is the HEART of combat! When two things touch,
     * this figures out if damage should happen.
     */
    bool OnCollide(Actor* other) {
        // Safety checks
        if (!owner || !other) return false;
        if (owner == other) return false;  // Can't hurt yourself (unless damageSelf is true)

        // Check if we can attack yet (cooldown)
        if (currentCooldown > 0.0f) {
            TraceLog(LOG_INFO, "Attack on cooldown!");
            return false;
        }

        // --------------------------------------------------------------------
        // FIGURE OUT WHAT THE OTHER ACTOR IS
        // --------------------------------------------------------------------
        // We need to know if the other actor is:
        // - Player
        // - Enemy
        // - Neutral
        // - Projectile
        // 
        // For now, we'll use a simple method: check their name!
        // In a real game, you'd use tags or types.

        bool otherIsPlayer = (other->GetName() == "Player");
        bool otherIsEnemy = (other->GetName() == "Enemy");

        // --------------------------------------------------------------------
        // DECIDE IF WE SHOULD DAMAGE THE OTHER ACTOR
        // --------------------------------------------------------------------
        bool shouldDamage = false;

        if (otherIsPlayer && damagePlayer) {
            shouldDamage = true;
            TraceLog(LOG_INFO, "Hit player!");
        }
        else if (otherIsEnemy && damageEnemy) {
            shouldDamage = true;
            TraceLog(LOG_INFO, "Hit enemy!");
        }
        else if (!otherIsPlayer && !otherIsEnemy && damageNeutral) {
            shouldDamage = true;
            TraceLog(LOG_INFO, "Hit neutral thing!");
        }

        // Also check if we should damage ourselves (rare, but possible)
        if (damageSelf && other == owner) {
            shouldDamage = true;
            TraceLog(LOG_INFO, "Hit self!");
        }

        // --------------------------------------------------------------------
        // APPLY DAMAGE OR HEALING
        // --------------------------------------------------------------------
        if (shouldDamage) {
            // Check if this is healing instead of damage
            if (healAmount > 0.0f) {
                // Try to heal the other actor
                // We need to add a Heal() method to Actor!
                // For now, we'll just log it
                TraceLog(LOG_INFO, TextFormat("Healed for %.1f", healAmount));
            }
            else {
                // Try to damage the other actor
                // We need the other actor to have a TakeDamage method!
                // For now, we'll just log it
                TraceLog(LOG_INFO, TextFormat("Dealt %.1f damage", damageAmount));

                // Store who hit them (for revenge targeting!)
                lastAttacker = owner;
            }

            // Set cooldown so we don't attack again immediately
            currentCooldown = attackCooldown;

            // Apply knockback (push the other actor)
            ApplyKnockback(other);

            return true;  // Damage was dealt
        }

        return false;  // No damage dealt
    }

    // ------------------------------------------------------------------------
    // KNOCKBACK - Push actors when they get hit
    // ------------------------------------------------------------------------

    /**
     * ApplyKnockback - Pushes the other actor away
     * @param other The actor to push
     */
    void ApplyKnockback(Actor* other) {
        if (!owner || !other || knockbackForce <= 0.0f) return;

        // Calculate direction from owner to other
        Vector2 ownerPos = owner->GetPosition();
        Vector2 otherPos = other->GetPosition();

        float dx = otherPos.x - ownerPos.x;
        float dy = otherPos.y - ownerPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 0) {
            // Normalize direction
            float dirX = dx / distance;
            float dirY = dy / distance;

            // Apply knockback (simple: just move them)
            // In a real game, you'd add velocity or force
            Vector2 newPos = {
                otherPos.x + dirX * knockbackForce,
                otherPos.y + dirY * knockbackForce
            };

            other->SetPosition(newPos);
            TraceLog(LOG_INFO, "Knockback applied!");
        }
    }

    // ------------------------------------------------------------------------
    // GETTERS & SETTERS
    // ------------------------------------------------------------------------

    // Damage settings
    void SetDamageAmount(float amount) { damageAmount = amount; }
    float GetDamageAmount() const { return damageAmount; }

    void SetHealAmount(float amount) { healAmount = amount; }
    float GetHealAmount() const { return healAmount; }

    // Target filters
    void SetCanDamagePlayer(bool can) { damagePlayer = can; }
    void SetCanDamageEnemy(bool can) { damageEnemy = can; }
    void SetCanDamageSelf(bool can) { damageSelf = can; }
    void SetCanDamageNeutral(bool can) { damageNeutral = can; }

    // Knockback
    void SetKnockbackForce(float force) { knockbackForce = force; }
    float GetKnockbackForce() const { return knockbackForce; }

    // Cooldown
    void SetAttackCooldown(float cooldown) { attackCooldown = cooldown; }
    float GetAttackCooldown() const { return attackCooldown; }
    bool CanAttack() const { return currentCooldown <= 0.0f; }

    // Who hit us last? (for AI revenge)
    Actor* GetLastAttacker() const { return lastAttacker; }
    void ClearLastAttacker() { lastAttacker = nullptr; }

private:
    // ------------------------------------------------------------------------
    // PRIVATE VARIABLES
    // ------------------------------------------------------------------------

    Actor* owner;              // Who owns this component

    float damageAmount;        // How much damage we deal
    float healAmount;           // If >0, we heal instead of damage
    float knockbackForce;       // How hard we push when hitting

    bool damagePlayer;          // Can we hurt the player?
    bool damageEnemy;           // Can we hurt enemies?
    bool damageSelf;            // Can we hurt ourselves?
    bool damageNeutral;          // Can we hurt neutral things?

    float attackCooldown;       // Time between attacks
    float currentCooldown;       // Time until next attack

    Actor* lastAttacker;        // Who last hit us (for AI)
};

#endif // COMBATCOMPONENT_H