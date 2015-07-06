#pragma once
#include "Animation.h"
#include "Definitions.h"
#include "Camera.h"
#include "GL/glew.h"

typedef struct Player
{
	AnimData anim;
	float posXStart = 40;
	float posYStart = (MAPHEIGHT-3) *32 ;
	float posX = 40;
	float posY = (MAPHEIGHT-3)*32;
	GLuint sprite;
	bool grounded = false;
	bool onLadder = false;
} Player;

static void playerUpdate(Player* player, float dt);
void playerDraw(Player* player, Camera * camera, GLuint* bgTextures);
void playerReset(Player* player);