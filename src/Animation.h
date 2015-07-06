#pragma once
#include <array>
#include "SDL.h"
#include "SDL_scancode.h"
#include "GL/glew.h"
#include "DrawUtils.h"
// Define a single frame used in an animation
struct AnimFrameDef {
	// combined with the AnimDef's name to make
	// the actual texture name
	int frameNum;
	float frameTime;
};

// Defines a single animation used by a character
struct AnimDef {
	const char* name;
	std::array<AnimFrameDef*,8> frames;
	int numFrames;
};

// Runtime state for an animation
struct AnimData {
	AnimDef* def;
	int curFrame;
	float timeToNextFrame;
	bool isPlaying;
};

void animTick(AnimData* data, float dt);
void animDraw(AnimData* anim, int x, int y, int w, int h, GLuint* bgTextures);
void animReset(AnimData* anim);
void animSet(AnimData* anim,
	AnimDef* toPlay);