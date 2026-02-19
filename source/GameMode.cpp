#include "GameMode.h"
#include "Actor.h"

GameMode::GameMode()
	: gameTime(0)
	, isPaused(false) {
}

GameMode::~GameMode() {  // FIXED: GameMOde -> GameMode
	actors.clear();
}

void GameMode::HandleInput() {
	if (IsKeyPressed(KEY_P)) {  // FIXED: isKeyPressed -> IsKeyPressed
		isPaused = !isPaused;
	}
}

void GameMode::Update(float deltaTime) {
	if (isPaused) return;

	gameTime += deltaTime;

	for (auto& actor : actors) {
		if (actor->IsActive()) {
			actor->Tick(deltaTime);
		}
	}
}

void GameMode::Draw() {
	for (auto& actor : actors) {
		if (actor->IsActive()) {
			actor->Draw();
		}
	}

	if (isPaused) {
		DrawText("PAUSED", 350, 280, 40, RED);
	}
}

void GameMode::LoadLevel(const char* levelName) {
	actors.clear();
}