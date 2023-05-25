#pragma once
#include "Components.h"
#include "../Vector2D.h"
struct TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;
	int speed = 3;
	int height = 32;
	int width = 32;
	int scale = 2;

	TransformComponent()
	{
		position.Zero();
	}
	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	TransformComponent(int sc)
	{
		position.Zero();
		scale = sc;
	}
	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}
	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};