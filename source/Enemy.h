#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

class Enemy : public Actor{
public:
	Enemy();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void SetTarget(Actor* newTarget) { target = newTarget; }	
	void TakeDamage(float amount) {
		health -= amount;}

private:
	float speed;
	float health;
	Actor* target; // Pointer to the player or other target

};

#endif // ENEMY_H