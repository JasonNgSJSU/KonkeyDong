#include "Enemy.h"
#include "Definitions.h"

/* Draw Enemy*/
void enemyDraw(Enemy* enemy, Camera * camera, GLuint* bgTextures){
	//if (enemy->posX + ENEMYSIZE >= camera->posX&&enemy->posX <= camera->posX + CAMERAWIDTH && enemy->posY + ENEMYSIZE >= camera->posY&&enemy->posY <= camera->posY + CAMERAHEIGHT){
		if (enemy->anim.isPlaying){
			//animDraw(&enemy->anim, enemy->posX - camera->posX, enemy->posY - camera->posY, 32, 32);
			animDraw(&enemy->anim, enemy->posX - camera->posX, enemy->posY - camera->posY, ENEMYSIZE, ENEMYSIZE, bgTextures);
		}
		else {
			glDrawSprite(enemy->sprite, enemy->posX - camera->posX, enemy->posY - camera->posY, ENEMYSIZE, ENEMYSIZE);
		}
	}
//}