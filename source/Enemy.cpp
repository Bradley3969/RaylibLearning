#include "Enemy.h"
#include <cmath>
#include <cstdlib> // for random number functions

// CONSTRUCTOR
Enemy::Enemy()
	: Actor() // call base class constructor
	, speed(50.0f) // set default speed for enemy movement
	, healthComp() // initialize health component with default values
	, combatComp() // NEW: initialize combat component
	, target(nullptr) // start with no target via nullptr
{
	actorName = "Enemy"; // set a name for debugging or identification purposes

	healthComp.SetMaxHealth(50.0f); // set max health for the enemy
	healthComp.ResetHealth(); // start with full health

	// NEW: Set up combat component for enemy
	combatComp.Initialize(this); // Tell combat component who owns it
	combatComp.SetDamageAmount(10.0f); // Enemy deals 10 damage per hit
	combatComp.SetCanDamagePlayer(true); // Enemy CAN hurt the player
	combatComp.SetCanDamageEnemy(false); // Enemy should NOT hurt other enemies
	combatComp.SetAttackCooldown(1.0f); // Can attack once per second
	combatComp.SetKnockbackForce(50.0f); // Push player back when hit

	TraceLog(LOG_INFO, TextFormat("Enemy created with max health: %.1f and speed: %.1f",
		healthComp.GetMaxHealth(), speed));
}

// DESTRUCTOR
Enemy::~Enemy() // destructor, currently no special cleanup needed
{
	TraceLog(LOG_INFO, "Enemy destroyed.");
}

// BEGIN PLAY - Called once when enemy enters game
void Enemy::BeginPlay() {
	// spawn at random position
	position.x = static_cast<float>(GetRandomValue(50, 750)); // random x between 50 and 750
	position.y = static_cast<float>(GetRandomValue(50, 550)); // random y between 50 and 550

	TraceLog(LOG_INFO, TextFormat("Enemy spawned at position (%.1f, %.1f)", position.x, position.y));
}

// TICK - Called every frame
void Enemy::Tick(float deltaTime)
{
	// --------------------------------------------------------------------
	// PART 1: UPDATE COMPONENTS
	// --------------------------------------------------------------------
	healthComp.Update(deltaTime); // counts down invincibility timer
	combatComp.Update(deltaTime); // NEW: updates combat cooldown timer

	// if enemy is dead, don't do anything
	if (healthComp.IsDead())
	{
		return; // dead enemy does not update
	}

	// --------------------------------------------------------------------
	// PART 2: CHECK FOR COLLISION WITH PLAYER (COMBAT!)
	// --------------------------------------------------------------------
	// If we have a target AND we're close enough to attack
	if (target != nullptr)
	{
		// Calculate distance to target
		Vector2 targetPos = target->GetPosition();
		float dx = targetPos.x - position.x;
		float dy = targetPos.y - position.y;
		float distance = std::sqrt(dx * dx + dy * dy);

		// NEW: If close enough to attack, use combat component!
		if (distance < 30.0f)  // Attack range
		{
			// Let combat component handle the attack
			// It will check cooldown, deal damage, apply knockback
			combatComp.OnCollide(target);
		}

		// --------------------------------------------------------------------
		// PART 3: MOVE TOWARD TARGET (if not already attacking)
		// --------------------------------------------------------------------
		if (distance > 30.0f)  // Too far to attack, move closer
		{
			// Normalize direction
			float dirX = dx / distance;
			float dirY = dy / distance;

			// Move toward target
			position.x += dirX * speed * deltaTime;
			position.y += dirY * speed * deltaTime;
		}
	}
	else
	{
		// No target - maybe wander around later
		TraceLog(LOG_INFO, "Enemy has no target.");
	}
}

// DRAW - Called every frame
void Enemy::Draw()
{
	// don't draw if dead
	if (healthComp.IsDead())
	{
		// draw a tombstone X instead
		DrawLine(position.x - 15, position.y - 15, position.x + 15, position.y + 15, DARKGRAY);
		DrawLine(position.x - 15, position.y + 15, position.x + 15, position.y - 15, DARKGRAY);
		return;
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

	// draw enemy as a square
	DrawRectangle(position.x - 15, position.y - 15, 30, 30, enemyColor);

	// draw enemy name above them
	DrawText(actorName.c_str(), position.x - 20, position.y - 30, 10, DARKGRAY);

	// ask HealthComponent to draw health bar
	healthComp.DrawDebug(position);

	// if invincible, draw a white outline
	if (healthComp.IsInvincible())
	{
		DrawRectangleLines(position.x - 17, position.y - 17, 34, 34, WHITE);
	}

	// NEW: Show attack range for debugging
	if (target != nullptr)
	{
		// Draw attack range circle
		DrawCircleLines(position.x, position.y, 30.0f, Fade(ORANGE, 0.3f));
	}

	// for debugging, draw line to target
	if (target != nullptr)
	{
		Vector2 targetPos = target->GetPosition();
		DrawLine(position.x, position.y, targetPos.x, targetPos.y, LIGHTGRAY);
	}
}

// TAKE DAMAGE - Apply damage to enemy
void Enemy::TakeDamage(float amount)
{
	// call health component to apply damage
	bool died = healthComp.TakeDamage(amount);

	if (died)
	{
		TraceLog(LOG_INFO, "Enemy died!");
	}
	else
	{
		TraceLog(LOG_INFO, TextFormat("Enemy took %.1f damage, %.1f health remaining",
			amount, healthComp.GetCurrentHealth()));

		// brief invincibility after being hit
		healthComp.SetInvincibility(0.5f);
	}
}