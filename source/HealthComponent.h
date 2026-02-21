#pragma once
#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "raylib.h"

class HealthComponent {
public:
	HealthComponent()
	: maxHealth(100.0f)
	, currentHealth(maxHealth) // starts with full health
	, isDead(false) // start alive
	, invincibleTime(0.0f) // not invincible at start
	{
	}

	virtual ~HealthComponent() {}

	// quick refence for calls to use health

	// TakeDamage - reduces health
	// @param amount how much damage
	// @return true if the actor died

	bool TakeDamage(float amount)
	{
		//check if entity is invincible
		if (invincibleTime > 0.0f)
		{
			TraceLog(LOG_INFO, "Damage ignored due to invincibility");
			return false; // no damage taken so no death occurs
		}

		if (amount < 0.0f) // make sure is positive cant heal by taking negative damage
		{
			TraceLog(LOG_WARNING, "TakeDamage called with negative amount, ignoring.");
			return false;
		}

		//apply damage to currentHealth
		currentHealth -= amount;

		// log of what happened 
		TraceLog(LOG_INFO, TextFormat("Took damage", amount, currentHealth, maxHealth));

		// check if health hit zero or below 
		if (currentHealth <= 0.0f)
		{
			currentHealth = 0.0f; // prevent negative health
			isDead = true;

			TraceLog(LOG_INFO, "Enity died.");
			return true; // true to indicate death
		}
		return false; // means not dead yet
	}

	// Heal - increases health
	// @param amount how much to heal
	// @return true if healed successfully

	bool Heal(float amount)
	{
		// cant heal the dead
		if (isDead)
		{
			TraceLog(LOG_WARNING, "Heal called on dead entity, ignoring.");
			return false;
		}
		// make sure amount is positive
		if (amount < 0.0f)

			// add health but dont exceed maxHealth
			currentHealth += amount;
		if (currentHealth > maxHealth)
			{
				currentHealth = maxHealth;
		}

		TraceLog(LOG_INFO, TextFormat("Healed for", amount, currentHealth, maxHealth));
		return true; // healed successfully
	}

	// SetInvincibility - makes entity invincible for a duration
	// @param duration how long to be invincible in seconds
	
	void SetInvincibility(float time)
	{
		invincibleTime = time;
		TraceLog(LOG_INFO, TextFormat("Entity is now invincible for", time, "seconds."));
	}

	// Update - should be called every frame to update timers and states
	// @param deltaTime time since last frame in seconds

	void Update(float deltaTime)
	{
		// updatecountdown invincibility timer
		if (invincibleTime > 0.0f)
		{
			invincibleTime -= deltaTime;
			if (invincibleTime <= 0.0f) // invincibility has ended
			{
				invincibleTime = 0.0f; // reset to zero to avoid negative time
				TraceLog(LOG_INFO, "Entity is no longer invincible.");
			}
		}
	}



	// Getters for health status
	// DrawDebug - draw health bar or info (optional, for debugging purposes)
	// @param position where to draw the health info

	void DrawDebug(Vector2 position)
	{
		if (!isDead && currentHealth < maxHealth)  // only draws between dead and maxhealth
		{
			// draw background bar (gray)
			DrawRectangle(position.x - 25, position.y - 30, 50, 5, LIGHTGRAY); // background bar

			// CALCULATE health percentage & DRAW foreground bar (green)
			float healthPercent = currentHealth / maxHealth;
			DrawRectangle(position.x - 25, position.y - 30, 50 * healthPercent, 5, GREEN); // foreground bar
		}
	}

	/////////////////////////////////////////////
	//GETTERS ALLOW ALLL CODE TO READ BUT NOT MODIFY HEALTH VALUES DIRECTLY

	float GetCurrentHealth() const { return currentHealth; } // returns current health value
	float GetMaxHealth() const { return maxHealth; } // returns max health value
	float GetHealthPercentage() const { return currentHealth / maxHealth; } // returns health as percentage
	bool IsDead() const { return isDead; } // returns if entity is dead
	bool IsInvincible() const { return invincibleTime > 0.0f; } // returns if entity is currently invincible

	//////////////////////////////////////////
	// SETTERS ALLOW CONTROLLED MODIFICATION OF HEALTH VALUES

	void SetMaxHealth(float newMax)
	{
		if (newMax > 0) {
			maxHealth = newMax;
			// make sure current health isnt higher than newmax
			if (currentHealth > maxHealth) {
				currentHealth = maxHealth;
			}
		}
	}

	void ResetHealth()
	{
		currentHealth = maxHealth;
		isDead = false;
		invincibleTime = 0.0f;
	}

	///////////////////////////// 
	// PRIVATE VARIABLES TO STORE HEALTH DATA
	private:
		float maxHealth; // maximum health value
		float currentHealth; // current health value
		bool isDead; // whether the entity is dead
		float invincibleTime; // how long the entity is invincible for (in seconds)
		};

#endif // HEALTHCOMPONENT_H