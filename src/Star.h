#pragma once
#include "Camera.h"
#include "GL/glew.h"

struct Star {
	bool dead = false;
	float posX, posY;
	int w, h;
	GLuint sprite;
};

void starDraw(Star* s, Camera * camera);