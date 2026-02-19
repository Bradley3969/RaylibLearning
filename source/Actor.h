#pragma once
#ifndef ACTOR_H
#define ACTOR_H

#include "raylib.h"
#include <string>

class Actor {
public:
	Actor();
	virtual ~Actor();


	// great value unreal methods
	virtual void BeginPlay();   // called when actor enters game
	virtual void Tick(float deltaTime); // called every frame
	virtual void Draw();     // called on every frame for rendering

	//transform (every actor will have a positive/rotation/scale)
	void SetPosition(Vector2 newPos) { position = newPos; }
	Vector2 GetPosition() const { return position; }

	// basic properties
	void SetActive(bool isActive) { active = isActive; }
	bool IsActive() const { return active; }

protected:
	Vector2 position;
	float rotation;
	Vector2 scale;
	bool active;
	std::string actorName;
};

#endif