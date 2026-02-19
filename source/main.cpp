#ifndef NDEBUG
#include <vld.h> // Visual Leak Detector, only in debug mode
#endif

#include "raylib.h"
#include "GameMode.h"
#include "Player.h"
#include "Enemy.h"

int main()
{
	InitWindow(800, 600, "My First Game");
	SetTargetFPS(60);

	// create dame mode

	GameMode gameMode;

	// spawn player
	Player* player = gameMode.SpawnActor<Player>({ 400, 300 });

	// spawn some enemies

	for (int i = 0; i < 5; ++i)
	{
		Enemy* enemy = gameMode.SpawnActor<Enemy>({ 0, 0 });
		enemy->SetTarget(player); // set player as target for enemy
	}

	// main game loop

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		gameMode.HandleInput();
		ClearBackground(RAYWHITE); // added this to clear frames 
		gameMode.Update(deltaTime);

		gameMode.Draw();

		// draw the FPS
		DrawText(TextFormat("FPS: &i", GetFPS()), 10, 10, 20, DARKGRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
} 