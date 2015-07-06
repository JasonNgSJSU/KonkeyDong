#include "Animation.h"
#include "SDL.h"
#include "DrawUtils.h"

void animReset(AnimData* anim)
{
	animSet(anim, anim->def);
}

void animDraw(AnimData* anim, int x, int y,
	int w, int h, GLuint* bgTextures)
{
	int curFrameNum
		= anim->def->frames[anim->curFrame]->frameNum;	
	GLuint tex = bgTextures[curFrameNum];
	glDrawSprite(tex, x, y, w, h);
}

void animTick(AnimData* data, float dt)
{
	if (!data->isPlaying) {
		return;
	}

	int numFrames = data->def->frames.size();
	data->timeToNextFrame -= dt;
	if (data->timeToNextFrame < 0) {
		++data->curFrame;
		if (data->curFrame >= numFrames) {
			// end of the animation, stop it
			data->curFrame = numFrames - 1;
			data->timeToNextFrame = 0;
			data->isPlaying = false;
			animReset(data);
		}
		else {
			AnimFrameDef* curFrame
				= data->def->frames[data->curFrame];
			data->timeToNextFrame
				+= curFrame->frameTime;
		}
	}
}

void animSet(AnimData* anim,
	AnimDef* toPlay)
{
	anim->def = toPlay;
	anim->curFrame = 0;
	anim->timeToNextFrame
		= anim->def->frames[0]->frameTime;
	anim->isPlaying = true;
}