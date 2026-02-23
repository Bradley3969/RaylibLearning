// Player.cpp - UPDATED VERSION
#include "Player.h"

// CONSTRUCTOR
Player::Player()
    : Actor()                    // First, initialize Actor base class
    , healthComp()                // Then initialize HealthComponent
    , movement(200.0f)            // NEW: Initialize MovementComponent with speed 200
{
    actorName = "Player";

    // Customize player health (tougher than enemies)
    healthComp.SetMaxHealth(150.0f);
    healthComp.ResetHealth();

    TraceLog(LOG_INFO, "Player created with 150 health and 200 speed");
}

// DESTRUCTOR
Player::~Player() {
    TraceLog(LOG_INFO, "Player destroyed");
}

// BEGIN PLAY - Called once when player enters game
void Player::BeginPlay() {
    position = { 400, 300 };  // Start in the middle
    TraceLog(LOG_INFO, "Player BeginPlay at (400, 300)");
}

// TICK - Called every frame (heart of player logic)
void Player::Tick(float deltaTime) {
    // --------------------------------------------------------------------
    // PART 1: GET INPUT DIRECTION
    // --------------------------------------------------------------------
    // We read keyboard and build a direction vector
    // (1,0) = right, (-1,0) = left, (0,1) = down, (0,-1) = up
    // Combining gives diagonals: (1,-1) = up-right

    Vector2 direction = { 0, 0 };

    if (IsKeyDown(KEY_RIGHT)) direction.x += 1.0f;
    if (IsKeyDown(KEY_LEFT)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_DOWN)) direction.y += 1.0f;
    if (IsKeyDown(KEY_UP)) direction.y -= 1.0f;

    // --------------------------------------------------------------------
    // PART 2: LET MOVEMENT COMPONENT DO THE MATH
    // --------------------------------------------------------------------
    // This is DELEGATION - we're asking movement to handle the hard part!
    // MovementComponent will:
    // - Normalize direction (prevent faster diagonal)
    // - Apply speed
    // - Use deltaTime for frame-independent movement
    // - Return the new position

    position = movement.MoveInDirection(direction, deltaTime, position);

    // --------------------------------------------------------------------
    // PART 3: KEEP PLAYER ON SCREEN
    // --------------------------------------------------------------------
    // MovementComponent doesn't know about screen boundaries,
    // so we still need to handle this!

    if (position.x < 0) position.x = 0;
    if (position.x > 800) position.x = 800;
    if (position.y < 0) position.y = 0;
    if (position.y > 600) position.y = 600;

    // --------------------------------------------------------------------
    // PART 4: UPDATE COMPONENTS
    // --------------------------------------------------------------------
    // Both components need to update every frame
    // (HealthComponent for invincibility timers,
    //  MovementComponent for boost timers)

    healthComp.Update(deltaTime);
    movement.Update(deltaTime);

    // --------------------------------------------------------------------
    // PART 5: TEST CONTROLS
    // --------------------------------------------------------------------
    // These let us test our components!

    if (IsKeyPressed(KEY_H)) {
        TakeDamage(20.0f);  // Test taking damage
    }

    if (IsKeyPressed(KEY_R)) {
        healthComp.Heal(20.0f);  // Test healing
        TraceLog(LOG_INFO, "Player healed");
    }

    if (IsKeyPressed(KEY_B)) {
        // NEW: Test speed boost! Double speed for 3 seconds
        movement.TemporaryBoost(2.0f, 3.0f);
        TraceLog(LOG_INFO, "Speed boost activated!");
    }
}

// TAKE DAMAGE - Called when player gets hit
void Player::TakeDamage(float amount) {
    bool died = healthComp.TakeDamage(amount);

    if (died) {
        TraceLog(LOG_WARNING, "Player died! Respawning...");
        healthComp.ResetHealth();  // Simple respawn

        // Could also trigger death animation, game over, etc.
    }
}

// DRAW - Called every frame to render player
void Player::Draw() {
    // Don't draw if dead
    if (healthComp.IsDead()) {
        // Draw tombstone X
        DrawLine(position.x - 15, position.y - 15,
            position.x + 15, position.y + 15, DARKGRAY);
        DrawLine(position.x + 15, position.y - 15,
            position.x - 15, position.y + 15, DARKGRAY);
        return;
    }

    // --------------------------------------------------------------------
    // Choose color based on health percentage
    // --------------------------------------------------------------------
    Color playerColor;
    float healthPercent = healthComp.GetHealthPercentage();

    if (healthPercent > 0.7f) {
        playerColor = BLUE;        // Healthy
    }
    else if (healthPercent > 0.3f) {
        playerColor = ORANGE;       // Wounded
    }
    else {
        playerColor = RED;          // Critical
    }

    // --------------------------------------------------------------------
    // Draw the player
    // --------------------------------------------------------------------
    DrawCircle(position.x, position.y, 20, playerColor);

    // --------------------------------------------------------------------
    // Draw movement direction indicator (NEW!)
    // --------------------------------------------------------------------
    if (movement.IsMoving()) {
        Vector2 dir = movement.GetMoveDirection();
        // Draw a line showing where we're going
        DrawLine(position.x, position.y,
            position.x + dir.x * 30, position.y + dir.y * 30, WHITE);
    }

    // --------------------------------------------------------------------
    // Draw boost indicator (NEW!)
    // --------------------------------------------------------------------
    if (movement.IsBoosted()) {
        // Yellow circle when boosted
        DrawCircleLines(position.x, position.y, 25, YELLOW);
    }

    // --------------------------------------------------------------------
    // Draw UI elements
    // --------------------------------------------------------------------
    DrawText(actorName.c_str(), position.x - 30, position.y - 40, 10, DARKGRAY);
    healthComp.DrawDebug(position);
}