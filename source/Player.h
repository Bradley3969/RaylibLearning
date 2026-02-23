#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"
#include "HealthComponent.h"
#include "MovementComponent.h"

class Player : public Actor {
public:
	Player();
	virtual ~Player(); // rember to have deeeeestoryer of player raaaaaah

	virtual void BeginPlay() override; // called when player enters game
	virtual void Tick(float deltaTime) override; // called every frame to update player logic
	virtual void Draw() override; // called every frame to render 
	
	void TakeDamage(float amount); // apply damage to player because i forgot this derr

private:
	
	HealthComponent healthComp; // health component to manage player's health
	MovementComponent movement; // movement component to handle player movement
};

#endif