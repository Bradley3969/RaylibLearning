#pragma once
#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "raylib.h"
#include <vector>
#include <memory>
#include <type_traits>


class Actor;

class GameMode {
public:
	GameMode();
	virtual ~GameMode();

	// main loop functions
	virtual void HandleInput();
	virtual void Update(float deltaTime);
	virtual void Draw();

	// Actor Management (great value unreal spawnactor)
	template<typename T>
	T* SpawnActor(Vector2 location)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T must be derived from Actor");

		auto actor = std::make_unique<T>();
		actor->SetPosition(location);

		T* actorPtr = actor.get();
		actors.push_back(std::move(actor));

		actorPtr->BeginPlay();
		return actorPtr;
	}

	//level transitioner ( great value OpenLevel)
	virtual void LoadLevel(const char* levelName);

protected:
	std::vector<std::unique_ptr<Actor>> actors;
	float gameTime;
	bool isPaused;
};

#endif