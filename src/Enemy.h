#pragma once
#include "Animation.h"
#include "Camera.h"
#include "GL/glew.h"
#include "Definitions.h"

struct Enemy {
	AnimData anim;	
	bool dead = false;
	bool needANewDecision = true;
	float posX = 850;
	float posY = 96;
	//float speedX = -0.1;
	float speedY = 0;
	GLuint sprite;	
	int type = 1;
	float enemySpeed = -ROLLSPEED;
	bool move = true;
};

void enemyUpdate(Enemy* enemy, float dt);
void enemyDraw(Enemy* enemy, Camera * camera, GLuint* bgTextures);
static bool destroyEnemy(Enemy*);