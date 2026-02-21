#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"
#include "HealthComponent.h"
class Enemy : public Actor{
public:
	Enemy(); // constructor to initialize enemy properties
	virtual ~Enemy(); // destructor for cleanup if needed
	virtual void BeginPlay() override; // called when enemy enters the game
	virtual void Tick(float deltaTime) override; // called every frame to update enemy behavior
	virtual void Draw() override; // called every frame to render the enemy

	void SetTarget(Actor* newTarget) { target = newTarget; }	// set the target for the enemy to follow or attack
	void TakeDamage(float amount); // method to apply damage to the enemy, will interact with health component
	bool IsDead() const { return healthComp.IsDead(); } // check if the enemy is dead based on health component
	float GetHealth() const { return healthComp.GetCurrentHealth(); } // get current health for display or logic

private:
	float speed; // movement speed of the enemy
	HealthComponent healthComp; // Health component to manage health and damage
	Actor* target; // Pointer to the player or other target

};

#endif // ENEMY_H