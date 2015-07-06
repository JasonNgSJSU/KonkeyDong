#include "kong.h"
#include "Definitions.h"

/* Draw Enemy*/
void kongDraw(Kong* kong, Camera * camera, GLuint* bgTextures){
	if (kong->posX + KONGWIDTH >= camera->posX&&kong->posX <= camera->posX + CAMERAWIDTH && kong->posY + KONGHEIGHT >= camera->posY&&kong->posY <= camera->posY + CAMERAHEIGHT){
		if (kong->anim.isPlaying){			
			animDraw(&kong->anim, kong->posX - camera->posX, kong->posY - camera->posY, KONGWIDTH, KONGHEIGHT, bgTextures);
		}
		else {
			glDrawSprite(kong->sprite, kong->posX - camera->posX, kong->posY - camera->posY, KONGWIDTH, KONGHEIGHT);
		}
	}
}

