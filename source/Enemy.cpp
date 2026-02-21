#include "Enemy.h"
#include <cmath>
#include <cstdlib> // for random number functions


Enemy::Enemy()
	:Actor() // call base class constructor
	, speed(50.0f) // set default speed for enemy movement
	, healthComp() // initialize health component with default values
	, target(nullptr) // start with no target via nullptr
{
	actorName = "Enemy"; // set a name for debugging or identification purposes

	healthComp.SetMaxHealth(50.0f); // set max health for the enemy

	healthComp.ResetHealth(); // start with full health

	TraceLog(LOG_INFO, "Enemy created with, max health:", healthComp.GetMaxHealth(), "and speed:", speed);
}

Enemy::~Enemy() // destructor, currently no special cleanup needed
{
	TraceLog(LOG_INFO, "Enemy destroyed.");
}

void Enemy::BeginPlay() {
	// spawn at random position
	position.x = static_cast<float>(GetRandomValue(50, 750)); // random x between 50 and 750 to avoid spawning too close to edges
	position.y = static_cast<float>(GetRandomValue(50, 550)); // random y between 50 and 550 for same reason
	TraceLog(LOG_INFO, TextFormat("Enemy spawned at position (", position.x, ",", position.y, ")"));
}

void Enemy::Tick(float deltaTime) // called every frame to update enemy behavior
{
	// update healthcomponent counts down invincibility timer if active
	healthComp.Update(deltaTime);

	// if enemy is dead dont do anything
	if (healthComp.IsDead())
	{
		return; // return dead enemy does not update
	}
	// if we have a target , move towards it
	if (target != nullptr)
	{
		// get target position
		Vector2 targetPos = target->GetPosition();

		// calculate direction to target
		// vector math..... OH NO NOT THE MATHS THIS IS SUPPOSED TO BE FUN :(

		float dx = targetPos.x - position.x; // difference in x between target and enemy
		float dy = targetPos.y - position.y; // difference in y between target and enemy

		// distance to target calculation using Pythagorean theorem oh noo
		//Pythagorean theorem: distance = sqrt(dx^2 + dy^2) ewwwwwwwwwww brain cells != functional

		float distance = std::sqrt(dx * dx + dy * dy); // calculate distance to target
		if (distance > 5.0f)
		{
			float dirX = dx / distance; // normalize direction x component
			float dirY = dy / distance; // normalize direction y component

			position.x += dirX * speed * deltaTime; // move enemy towards target in x direction
			position.y += dirY * speed * deltaTime; // move enemy towards target in y direction

		}
		if (distance <= 5.0f)
		{
			// add attack logic later
			// for now just log enemy is close to target
			TraceLog(LOG_INFO, "Enemy is close to target, could attack now.");
		}
	}
	else
	{
		// no target, maybe wander around or do nothing for now
		// for now just log no target
		TraceLog(LOG_INFO, "Enemy has no target.");
	}
}

void Enemy::Draw() // called every frame to render the enemy
{
	// dont draw if dead
	if (healthComp.IsDead())
	{
		// draw a tombstone x istead
		DrawLine(position.x - 15, position.y - 15, position.x + 15, position.y + 15, DARKGRAY); // draw x for dead enemy
		DrawLine(position.x - 15, position.y + 15, position.x + 15, position.y - 15, DARKGRAY);// draw x for dead enemy
		return; // return dead enemy does not draw
	}

	// enemy color based on health percentage

	Color enemyColor;
	float healthPercent = healthComp.GetHealthPercentage();

	if (healthPercent > 0.5f)
	{
		enemyColor = GREEN; // healthy enemy is green
	}
	else if (healthPercent > 0.25f)
	{
		enemyColor = YELLOW; // injured enemy is yellow
	}
	else
	{
		enemyColor = RED; // critical enemy is red
	}

	// draw enemy as a square for simplicity

	DrawRectangle(position.x - 15, position.y - 15, 30, 30, enemyColor);

	// draw enemy name above them
	DrawText(actorName.c_str(), position.x - 20, position.y - 30, 10, DARKGRAY);

	// ask HealthComponent to draw health bar above enemy
	healthComp.DrawDebug(position);

	// if invincible, draw a white outline
	if (healthComp.IsInvincible())
	{
		DrawRectangle(position.x - 20, position.y - 20, 40, 40, Fade(WHITE, 0.5f)); // draw semi-transparent white outline
	}

	// for debugging, draw line to target if we have one
	if (target != nullptr)
	{
		Vector2 targetPos = target->GetPosition();
		DrawLine(position.x, position.y, targetPos.x, targetPos.y, LIGHTGRAY); // draw line to target
	}
}

// TAKE DAMAGE  METHOD TO APPLY DAMAGE TO THE ENEMY

void Enemy::TakeDamage(float amount)
{
	// call health component to apply damage
	bool died = healthComp.TakeDamage(amount);

	// if  damage kills enemy
	if (died)
	{
		// log death
		TraceLog(LOG_INFO, TextFormat("Enemy took", amount, "damage and has", healthComp.GetCurrentHealth(), "health left."));

	}
	else
	{
		// if not dead, log remaining health
		TraceLog(LOG_INFO, TextFormat("Enemy took", amount, "damage and has", healthComp.GetCurrentHealth(), "health left."));

		// prevent kills in one frame by setting invincibility for a short duration after taking damage
		healthComp.SetInvincibility(0.5f); // 0.5 seconds of invincibility after taking damage
	}
}