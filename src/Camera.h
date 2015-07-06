#pragma once
typedef struct Camera
{
	float posX;
	float posY;
} Camera;

static void cameraUpdate(Camera* camera, float deltaTime);