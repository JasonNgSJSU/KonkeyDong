#include "Player.h"
#include "SDL.h"
#include "DrawUtils.h"
#include "Animation.h"



/* Draw Player*/
void playerDraw(Player* player, Camera * camera, GLuint* bgTextures){

	//if (player->posX + 32 >= camera->posX&&player->posX <= camera->posX + CAMERAWIDTH && player->posY + 32 >= camera->posY&&player->posY <= camera->posY + CAMERAHEIGHT){
		if (player->anim.isPlaying){				
			animDraw(&player->anim, player->posX - camera->posX, player->posY - camera->posY, 32, 32, bgTextures);
		}
		else {
			glDrawSprite(player->sprite, player->posX - camera->posX, player->posY - camera->posY, 32, 32);
		}
	}
//}

void playerReset(Player* player){
	player->posX = player->posXStart;
	player->posY = player->posYStart;
}
