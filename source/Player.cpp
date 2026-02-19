#include "Player.h"

Player::Player()
	:speed(200.0f),
	health(100.0f) {
	actorName = "Player";
}

void Player::BeginPlay()
{
	// intitalize player specifics
	position = { 400, 300 };  //currently starting in center screen
}

void Player::Tick(float deltaTime)
{
	//handles player movement
	if (IsKeyDown(KEY_RIGHT)) position.x += speed * deltaTime;
	if (IsKeyDown(KEY_LEFT)) position.x -= speed * deltaTime;
	if (IsKeyDown(KEY_DOWN)) position.y += speed * deltaTime;
	if (IsKeyDown(KEY_UP)) position.y -= speed * deltaTime;

	// keep player on screen
	if (position.x < 0) position.x = 0;
	if (position.x > 800) position.x = 800;
	if (position.y < 0) position.y = 0;
	if (position.y > 600) position.y = 600;


}

void Player::Draw()
{
	// draws player as triangle by default
	DrawCircle(position.x, position.y, 20, BLUE);

	//Drad health bar
	DrawRectangle(position.x - 25, position.y - 30, 50, 5, LIGHTGRAY);
	DrawRectangle(position.x - 25, position.y - 30, 50 * (health / 100.0f), 5, GREEN);
}