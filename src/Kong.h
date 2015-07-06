#pragma once
#include "Animation.h"
#include "Camera.h"
#include "GL/glew.h"
#include "Definitions.h"

struct Kong {
	AnimData anim;
	bool dead = false;
	bool needANewDecision = true;
	float posX = 840;
	float posY = 64;
	float speedX, speedY;
	GLuint sprite;
	int w, h;
	int type = 1;
	float enemySpeed = ROLLSPEED;
	bool move = true;
};

//void enemyUpdate(Kong *kong, float dt);
void kongDraw(Kong *kong, Camera * camera, GLuint* bgTextures);
static void kongUpdate(Kong *dong, float dt);