#include "Player.h"

Player::Player()
	: Actor() // call base class constructor
	, speed(200.0f) // set default speed
	, healthComp() // default construct health component
{
	actorName = "Player"; // set actor name for debugging
	healthComp.SetMaxHealth(150.0f); // set player max health to 150
	healthComp.ResetHealth(); // start with full health
	TraceLog(LOG_INFO, "player created with", healthComp.GetMaxHealth(), "max health.");
}

Player::~Player() /// destructor FOR LA-payer el- RAII
{
	TraceLog(LOG_INFO, "player destroyed.");
}

void Player::BeginPlay()
{
	// intitalize player specifics
	position = { 400, 300 };  //currently starting in center screen
}

void Player::Tick(float deltaTime) // called every frame to update player logic
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



	/////////////////////////////////////////////
	//testing if player can take damage properly

	if (IsKeyPressed(KEY_H))
	{
		TakeDamage(20.0f); // take 20 damage when H is pressed
		TraceLog(LOG_INFO, "Player took 20 damage.");
	}


	// heal player testing
	if (IsKeyPressed(KEY_R))
	{
		healthComp.Heal(20.f);
		TraceLog(LOG_INFO, "Player healed for 20 health.");
	}
}

/////////////////////////////
void Player::TakeDamage(float amount) // public method to apply damage to player, calls health component's TakeDamage
{
	bool died = healthComp.TakeDamage(amount); // ask HealthComponent to process damage
	
	if (died)
	{
		// FIX LATER add animation or sound effect for death
		TraceLog(LOG_WARNING, "player died");
		// for now just reset health to keep testing
		// add game over or respann logic later
	}
}

void Player::Draw()
{
	// only draw if alive
	if (healthComp.IsDead())
	{
		// draw a gray x when player is dead 
		DrawLine(position.x - 15, position.y - 15, position.x + 15, position.y + 15, DARKGRAY); // diagonal line 1
		DrawLine(position.x - 15, position.y + 15, position.x + 15, position.y - 15, DARKGRAY); // diagonal line 2
		return; // skip drawing the player sprite if dead
	}

	// Draw player as a circle ( color is based around healh)
	Color playerColor;
	float healthPercent = healthComp.GetHealthPercentage();

	if(healthPercent > 0.7f)
	{
		playerColor = BLUE; // health high color blue
	}
	else if (healthPercent > 0.3f)
	{
		playerColor = YELLOW; // health medium color yellow
	}
	else
	{
		playerColor = RED; // health low color red
	}

	//Draw player 
	DrawCircle(position.x, position.y, 20, playerColor);

	// draw players name above them
	DrawText(actorName.c_str(), position.x - 30, position.y - 40, 10, DARKGRAY);

	// ask HealthComponent , nicely, to please draw the health bar above the player
	healthComp.DrawDebug(position); // pass player position so health bar can be drawn above player

	//draw invincibility effect if player is invincible
	if (healthComp.IsInvincible())
	{
		DrawCircleLines(position.x, position.y, 25, GOLD); // draw a gold circle around player when invincible
	}
}