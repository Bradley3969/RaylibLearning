#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

class Player : public Actor {
public:
	Player();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	float speed;
	float health;
};

#endif