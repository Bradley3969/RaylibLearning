#include "Actor.h"
Actor::Actor()
	:position({0,0}),
	rotation(0),
	scale({1, 1}),
	active(true),
	actorName("Actor")
{ }

Actor::~Actor(){}

void Actor::BeginPlay() {}	

void Actor::Tick(float deltaTime){}

void Actor::Draw()
{
	if (active)
	{
		DrawRectangle(position.x - 10, position.y - 10, 20, 20, RED);
	}
}