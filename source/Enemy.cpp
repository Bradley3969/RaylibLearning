#include "Enemy.h"
#include <cmath>

Enemy::Enemy()
	: speed(50.0f)
	, health(50.0f)
	, target(nullptr)
{
	actorName = "Enemy";
}

void Enemy::BeginPlay() {
	// spawn at random position
	position.x = static_cast<float>(GetRandomValue(50, 750));
	position.y = static_cast<float>(GetRandomValue(50, 550));
}

void Enemy::Tick(float deltaTime) {
	// ai: if target exists move towards it
	if (target && health > 0)
	{
		Vector2 targetPos = target->GetPosition();

		// calculate direction to target
		float dx = targetPos.x - position.x;
		float dy = targetPos.y - position.y;
		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance > 0)
		{
			// move towards target
			position.x += (dx / distance) * speed * deltaTime;
			position.y += (dy / distance) * speed * deltaTime;
		}
	}
}

void Enemy::Draw()
{
	if (health <= 0) return;  // FIXED: removed extra 'r'

	// draw enemy as a red square
	DrawRectangle(position.x - 15, position.y - 15, 30, 30, RED);

	// draw enemy health bar
	DrawRectangle(position.x - 20, position.y - 25, 40, 4, LIGHTGRAY);  // FIXED: LightGray -> LIGHTGRAY
	DrawRectangle(position.x - 20, position.y - 25, 40 * (health / 50.0f), 4, ORANGE);  // FIXED: parentheses and syntax
}