/* #includes left out */
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <vector>
#include "SDL.h"
#include "SDL_scancode.h"
#include "GL/glew.h"
#include "DrawUtils.h"
#include "Definitions.h"
#include "Player.h"
#include "Enemy.h"
#include "Kong.h"
#include "Camera.h"
#include "Tile.h"
#include "Map.h"
#include "Animation.h"
#include "Draw.h"

Player player;
Camera camera;
Kong dong;
float gameTime = 0;
int lastSpawn = 0;
int gameState = 0;

std::vector<Enemy*> enemyList;

static void collision(Player *player, float deltaTime);
static void collision(Enemy *enemy, float deltaTime);
char shouldExit = 0;
int enemyIndex = 1;


GLuint bgTextures[15];
GLuint man, man1, man2, man3, enemy1, enemy2, enemy3, enemy,enemy5,enemy6,enemy7,kong,kong1,kong2,kong3;
GLuint wall, background, block, ladder, win,start;


/* The previous frame's keyboard state */
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };
/* The current frame's keyboard state */
const unsigned char* kbState = NULL;

Tile bg[MAPWIDTH][MAPHEIGHT];

int iSecret;

static void stateUpdate();
static void gameOver();

int main(void)
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return 1;
	}

	/* Create the window, OpenGL context */
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow(
		"Konkey Dong by Jason Ng And No. There is no spelling errors.",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		CAMERAWIDTH, CAMERAHEIGHT,
		SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window.ErrorCode = %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_GL_CreateContext(window);

	/* Make sure we have a recent version of OpenGL */
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Colud not initialize glew.ErrorCode = %s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (!GLEW_VERSION_3_0) {
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}
	/* Setup OpenGL state */
	glViewport(0, 0, CAMERAWIDTH, CAMERAHEIGHT);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, CAMERAWIDTH, CAMERAHEIGHT, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);


	/* load the textures */
	wall = glTexImageTGAFile("Wall.tga", NULL, NULL);
	block = glTexImageTGAFile("Block.tga", NULL, NULL);
	background = glTexImageTGAFile("background.tga", NULL, NULL);	
	win = glTexImageTGAFile("win.tga", NULL, NULL);
	ladder = glTexImageTGAFile("ladder.tga", NULL, NULL);
	start = glTexImageTGAFile("start.tga", NULL, NULL);

	man = glTexImageTGAFile("walk1.tga", NULL, NULL);
	man1 = glTexImageTGAFile("walk2.tga", NULL, NULL);
	man2 = glTexImageTGAFile("walk3.tga", NULL, NULL);
	man3 = glTexImageTGAFile("walk4.tga", NULL, NULL);
	enemy = glTexImageTGAFile("Enemy1.tga", NULL, NULL);
	enemy1 = glTexImageTGAFile("Enemy2.tga", NULL, NULL);
	enemy2 = glTexImageTGAFile("Enemy3.tga", NULL, NULL);
	enemy3 = glTexImageTGAFile("Enemy4.tga", NULL, NULL);
	enemy5 = glTexImageTGAFile("Enemy5.tga", NULL, NULL);
	enemy6 = glTexImageTGAFile("Enemy6.tga", NULL, NULL);
	enemy7 = glTexImageTGAFile("Enemy7.tga", NULL, NULL);
	kong = glTexImageTGAFile("kong.tga", NULL, NULL);
	kong1 = glTexImageTGAFile("kong1.tga", NULL, NULL);
	kong2 = glTexImageTGAFile("kong2.tga", NULL, NULL);
	kong3 = glTexImageTGAFile("kong3.tga", NULL, NULL);

	bgTextures[0] = man;
	bgTextures[1] = man1;
	bgTextures[2] = man2;
	bgTextures[3] = man3;
	bgTextures[4] = enemy;
	bgTextures[5] = enemy1;
	bgTextures[6] = enemy2;
	bgTextures[7] = enemy3;
	bgTextures[8] = kong;
	bgTextures[9] = kong1;
	bgTextures[10] = kong2;
	bgTextures[11] = kong3;
	bgTextures[12] = enemy5;
	bgTextures[13] = enemy6;
	bgTextures[14] = enemy7;

	/////////////////////////////////////////////////////////////////////////
	//////// Animation Section
	////////////////////////////////////////////////////////////////////////

	AnimFrameDef tempMan, tempMan1, tempMan2, tempMan3;
	tempMan.frameNum = 0;
	tempMan.frameTime = FRAMESPEED;

	tempMan1.frameNum = 1;
	tempMan1.frameTime = FRAMESPEED;

	tempMan2.frameNum = 2;
	tempMan2.frameTime = FRAMESPEED;

	tempMan3.frameNum = 3;
	tempMan3.frameTime = FRAMESPEED;

	AnimDef tempDef;
	tempDef.name = "Walking";
	tempDef.frames[0] = &tempMan;
	tempDef.frames[1] = &tempMan1;
	tempDef.frames[2] = &tempMan2;
	tempDef.frames[3] = &tempMan3;
	tempDef.frames[4] = &tempMan3;
	tempDef.frames[5] = &tempMan2;
	tempDef.frames[6] = &tempMan1;
	tempDef.frames[7] = &tempMan;

	tempDef.numFrames = 8;

	AnimData tempData;
	tempData.def = &tempDef;
	tempData.curFrame = 0;
	tempData.timeToNextFrame = 1;
	tempData.isPlaying = true;

	player.anim = tempData;
	player.sprite = man;
	///////////	
	AnimFrameDef dongWalk, dongWalk1, dongWalk2, dongWalk3;
	dongWalk.frameNum = 8;
	dongWalk.frameTime = FRAMESPEED;

	dongWalk1.frameNum = 9;
	dongWalk1.frameTime = FRAMESPEED;

	dongWalk2.frameNum = 10;
	dongWalk2.frameTime = FRAMESPEED;

	dongWalk3.frameNum = 11;
	dongWalk3.frameTime = FRAMESPEED;

	AnimDef dongDef;
	dongDef.name = "DongWalking";
	dongDef.frames[0] = &dongWalk;
	dongDef.frames[1] = &dongWalk1;
	dongDef.frames[2] = &dongWalk2;
	dongDef.frames[3] = &dongWalk3;
	dongDef.frames[4] = &dongWalk3;
	dongDef.frames[5] = &dongWalk2;
	dongDef.frames[6] = &dongWalk1;
	dongDef.frames[7] = &dongWalk;
	dongDef.numFrames = 8;

	AnimData dongAnimData;
	dongAnimData.def = &dongDef;
	dongAnimData.curFrame = 0;
	dongAnimData.timeToNextFrame = 1;
	dongAnimData.isPlaying = true;

	dong.anim = dongAnimData;
	dong.sprite = kong1;

	
	/* Convert char background in to tile arrays*/
	for (int i = 0; i < MAPWIDTH; i++){
		for (int j = 0; j < MAPHEIGHT; j++){
			if (map[j][i] == 'w'){
				bg[i][j].image = wall;
				bg[i][j].coll = true;
			}
			else if (map[j][i] == 'l'){
				bg[i][j].image = ladder;
				bg[i][j].coll = true;
				bg[i][j].isLadder = true;
			}
			else if (map[j][i] == 'b'){
				bg[i][j].image = block;
				bg[i][j].coll = true;
			}	
			else if (map[j][i] == 'd'){				
				
				bg[i][j].image = wall;
				bg[i][j].coll = true;
				bg[i][j].destroyZone = true;
			}
			else if (map[j][i] == 'e'){				
				
				bg[i][j].coll = true;
				bg[i][j].destroyZone = true;
			}
			else {
				bg[i][j].image = background;
				bg[i][j].coll = false;
				bg[i][j].air = true;
			}
		}
	}

	/* Keep a pointer to SDL's internal keyboard state */
	kbState = SDL_GetKeyboardState(NULL);

	/* Physics runs at 100fps, or 10ms / physics frame */
	float physicsDeltaTime = 1 / 100.0f;
	int physicsDeltaMs = 10;
	Uint32 lastPhysicsFrameMs = 0;

	/* delta time*/
	float deltaTime = 0;
	Uint32 lastFrameMs;
	Uint32 currentFrameMs = SDL_GetTicks();

	/* The game loop */
	while (!shouldExit) {
		
		// Handle OS message pump
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				shouldExit = 1;
			}
		}

		if (gameState == 1){
			lastFrameMs = currentFrameMs;
			/* kbState is updated by the message pump. Copy over the old state before the pump! */
			memcpy(kbPrevState, kbState, sizeof(kbPrevState));		

			currentFrameMs = SDL_GetTicks();
			float deltaTime = (currentFrameMs - lastFrameMs) / 1000.0f;		

			if (!dong.dead){
				//deltaTime++;
				playerUpdate(&player, deltaTime);
				cameraUpdate(&camera, deltaTime);
				kongUpdate(&dong, deltaTime);

				playerDraw(&player, &camera, bgTextures);
				kongDraw(&dong, &camera, bgTextures);
				for (int i = 0; i < enemyList.size(); i++){
					if (!enemyList[i]->dead){
						if (destroyEnemy(enemyList[i])){
							delete(enemyList[i]); //free memory
							enemyList.erase(enemyList.begin() + i);
						}
						else {
							enemyUpdate(enemyList[i], deltaTime);
							enemyDraw(enemyList[i], &camera, bgTextures);
						}
					}
				}
			}
			/* Check for collision */
			do {
				collision(&player, physicsDeltaTime);
				lastPhysicsFrameMs += physicsDeltaMs;
			} while (lastPhysicsFrameMs + physicsDeltaMs < currentFrameMs);						
		}
		else if(gameState == 0){
			stateUpdate();
			glDrawSprite(start, camera.posX, camera.posY, 1024, 768);
		}
		else if (gameState == 2){			
			//wining condition
			gameOver();
			glDrawSprite(win, camera.posX, camera.posY, 1024, 768);
		}
		
		/* Present to the player */
		SDL_GL_SwapWindow(window);
	}
	SDL_Quit();
	return 0;
}

static void gameOver(){
	for (int i = 0; i < enemyList.size(); i++){
		if (destroyEnemy(enemyList[i])){
			delete(enemyList[i]); //free memory
			enemyList.erase(enemyList.begin() + i);
		}
	}

	if (kbState[SDL_SCANCODE_KP_ENTER] || kbState[SDL_SCANCODE_RETURN]){		
		dong.dead = false;
		playerReset(&player);
		gameState = 1;
	}
}

//update state
static void stateUpdate(){	
	if (kbState[SDL_SCANCODE_KP_ENTER] || kbState[SDL_SCANCODE_RETURN]){		
		gameState = 1;
	}
}

/* Updates Player*/
static void playerUpdate(Player* player, float dt){
	if (kbState[SDL_SCANCODE_LEFT]){
		if (!player->grounded){
			player->posX -= (MIDJUMPSPEED*dt);

		}
		else{
			player->posX -= (SPEED*dt);
		}
	}  if (kbState[SDL_SCANCODE_RIGHT]){
		if (!player->grounded){
			player->posX += (MIDJUMPSPEED*dt);
		}
		else{
			player->posX += (SPEED*dt);
		}
	}
	if (kbState[SDL_SCANCODE_UP]){
		if (player->onLadder){
			player->posY -= (CLIMBSPEED*dt);
		}
		else if (player->grounded){
			player->posY -= (JUMPSPEED);
			player->grounded = false; //he jumped so hes no longer grounded let collision handle this
		}
	}
	if (kbState[SDL_SCANCODE_DOWN]){
		if (!player->grounded){
			player->posY += (SPEED*dt);
		}
	}
	if (!player->grounded){
		if (!player->onLadder){
			player->posY += (GRAVITY*dt);
		}
	}
	animTick(&player->anim, dt);
}

/*Updates Enemy*/
static void enemyUpdate(Enemy* enemy, float dt){
	animTick(&enemy->anim, dt);	
	collision(enemy, dt);
}

static void kongUpdate(Kong *dong, float dt){
	gameTime += dt;	
	int temp = (int)gameTime;	
	if (((temp % MSECONDS) == 0)&&(temp!=lastSpawn)){
		iSecret = rand() % 2 + 1;	
		
		lastSpawn = temp;
		Enemy * enemyObj = new Enemy();

		AnimFrameDef* enemyWalk = new AnimFrameDef();
		AnimFrameDef* enemyWalk1 = new AnimFrameDef();
		AnimFrameDef* enemyWalk2 = new AnimFrameDef();
		AnimFrameDef* enemyWalk3 = new AnimFrameDef();

		AnimDef* enemyDef = new AnimDef();
		enemyDef->name = "EnemyWalking";

		AnimData* tempData1 = new AnimData;

		if (iSecret == 1){
		
		enemyWalk->frameNum = 4;
		enemyWalk->frameTime = FRAMESPEED;

		enemyWalk1->frameNum = 5;
		enemyWalk1->frameTime = FRAMESPEED;

		enemyWalk2->frameNum = 6;
		enemyWalk2->frameTime = FRAMESPEED;

		enemyWalk3->frameNum = 7;
		enemyWalk3->frameTime = FRAMESPEED;
		enemyObj->sprite = enemy;
	
			enemyObj->type = 1;
		}
		else if (iSecret == 2){

			enemyWalk->frameNum = 12;
			enemyWalk->frameTime = FRAMESPEED;

			enemyWalk1->frameNum = 13;
			enemyWalk1->frameTime = FRAMESPEED;

			enemyWalk2->frameNum = 14;
			enemyWalk2->frameTime = FRAMESPEED;

			enemyWalk3->frameNum = 12;
			enemyWalk3->frameTime = FRAMESPEED;
			enemyObj->sprite = enemy;

			enemyObj->type = 2;
		}

		enemyDef->frames[0] = enemyWalk;
		enemyDef->frames[1] = enemyWalk1;
		enemyDef->frames[2] = enemyWalk2;
		enemyDef->frames[3] = enemyWalk3;
		enemyDef->frames[4] = enemyWalk;
		enemyDef->frames[5] = enemyWalk1;
		enemyDef->frames[6] = enemyWalk2;
		enemyDef->frames[7] = enemyWalk3;
		enemyDef->numFrames = 8;


		tempData1->def = enemyDef;
		tempData1->curFrame = 0;
		tempData1->timeToNextFrame = 1;
		tempData1->isPlaying = true;

		enemyObj->anim = *tempData1;		

		enemyList.push_back(enemyObj);
	}
	animTick(&dong->anim, dt);
}

static void cameraUpdate(Camera * camera, float dt){	
	/* draw background*/
	for (int i = 0; i < MAPWIDTH; i++){
		for (int j = 0; j < MAPHEIGHT; j++){			
				glDrawSprite(bg[i][j].image, (i * 32) - camera->posX, j * 32 - camera->posY, 32, 32);			
		}
	}
}

static void collision(Player *player, float dt){
	int bgL = player->posX / TILESIZE;
	int bgT = player->posY / TILESIZE;
	int bgR = bgL + 1;
	int bgB = bgT + 1;

	if (!dong.dead){

		if (player->posX >= dong.posX && player->posX <= dong.posX + KONGWIDTH && player->posY >= dong.posY&&player->posY <= dong.posY + KONGHEIGHT){
			dong.dead = true;
		}
		else if (player->posX + KONGWIDTH >= dong.posX && player->posX + KONGWIDTH <= dong.posX + KONGWIDTH && player->posY >= dong.posY&&player->posY <= dong.posY + KONGHEIGHT){
			dong.dead = true;
		}
		else if (player->posX >= dong.posX && player->posX <= dong.posX + KONGWIDTH && player->posY + KONGHEIGHT >= dong.posY&&player->posY + KONGHEIGHT <= dong.posY + KONGHEIGHT){
			dong.dead = true;
		}
		else if (player->posX + KONGWIDTH >= dong.posX && player->posX + KONGWIDTH <= dong.posX + KONGWIDTH && player->posY + KONGHEIGHT >= dong.posY&&player->posY + KONGHEIGHT <= dong.posY + KONGHEIGHT){
			dong.dead = true;
		}

		if (dong.dead == true){			
			//delete all enemies
			for (int i = 0; i < enemyList.size(); i++){
				delete(enemyList[i]); //free memory
				enemyList.erase(enemyList.begin() + i);
			}
			gameState = 2;//change game state
		}

	}

	for (int i = 0; i < enemyList.size(); i++){
		if (!enemyList[i]->dead){
			if (player->posX >= enemyList[i]->posX + PLAYERBUFFER && player->posX <= enemyList[i]->posX + TILESIZE - PLAYERBUFFER && player->posY >= enemyList[i]->posY + PLAYERBUFFER&&player->posY <= enemyList[i]->posY + TILESIZE - PLAYERBUFFER){
				enemyList[i]->dead = true;
				playerReset(player);
			}
			else if (player->posX + TILESIZE >= enemyList[i]->posX + PLAYERBUFFER && player->posX + TILESIZE <= enemyList[i]->posX + TILESIZE - PLAYERBUFFER && player->posY >= enemyList[i]->posY + PLAYERBUFFER &&player->posY <= enemyList[i]->posY + TILESIZE - PLAYERBUFFER){
				enemyList[i]->dead = true;
				playerReset(player);
			}
			else if (player->posX >= enemyList[i]->posX + PLAYERBUFFER && player->posX <= enemyList[i]->posX + TILESIZE - PLAYERBUFFER && player->posY + TILESIZE >= enemyList[i]->posY + PLAYERBUFFER&&player->posY + TILESIZE <= enemyList[i]->posY + TILESIZE - PLAYERBUFFER){
				enemyList[i]->dead = true;
				playerReset(player);
			}
			else if (player->posX + TILESIZE >= enemyList[i]->posX + PLAYERBUFFER && player->posX + TILESIZE <= enemyList[i]->posX + TILESIZE - PLAYERBUFFER && player->posY + TILESIZE >= enemyList[i]->posY + PLAYERBUFFER&&player->posY + TILESIZE <= enemyList[i]->posY + TILESIZE + PLAYERBUFFER){
				enemyList[i]->dead = true;
				playerReset(player);
			}
		}
	}
	
	if (bg[bgL][bgB].coll && bg[bgR][bgB].coll){
		player->grounded = true; //he is touchign the ground
		if (bg[bgL][bgB].isLadder || bg[bgR][bgB].isLadder){
			player->onLadder = true;
			player->grounded = false;
		}
		else {
			int dist = (((int)player->posY) % 32);
			player->posY -= dist - 1;
			player->onLadder = false;
		}
	}

	if (bg[bgR][bgT].coll && bg[bgR][bgB].coll){
		if (bg[bgR][bgT].isLadder || bg[bgR][bgB].isLadder){
			player->onLadder = true;
			player->grounded = false;
		}
		else {
			int dist = (((int)player->posX + 32) % 32);			
			player->posX -= dist;
			player->onLadder = false;
		}
	}
	

	if (bg[bgL][bgT].coll && bg[bgL][bgB].coll){
		if (bg[bgL][bgT].isLadder || bg[bgL][bgB].isLadder){
			player->onLadder = true;
			player->grounded = false;
		}
		else {
			int dist = 32-(((int)player->posX) % 32);			
			player->posX += dist+1;
			player->onLadder = false;
		}
	}	

	if (!(bg[bgL][bgB].coll && bg[bgR][bgB].coll)){

		if (bg[bgL][bgT].isLadder || bg[bgL][bgB].isLadder || bg[bgR][bgT].isLadder || bg[bgR][bgB].isLadder){
			player->onLadder = true;
		}
		else{			
			player->grounded = false;
			player->onLadder = false;
		}
	}

	//if player is below ground
	if (player->posY + 32 >= MAPHEIGHT * 32){
		playerReset(player);
	}
}

//enemy collision
static void collision(Enemy *enemy, float dt){
	int bgL = enemy->posX / TILESIZE;
	int bgT = enemy->posY / TILESIZE;
	int bgR = bgL + 1;
	int bgB = bgT + 1;
	
	//enemy that goes row by row
	if (enemy->type == 1){
		if (bg[bgL][bgT].isLadder || bg[bgL][bgB].isLadder || bg[bgR][bgT].isLadder || bg[bgR][bgB].isLadder){
			enemy->posX += enemy->enemySpeed*dt;
		}
		else {
			//hit left wall
			if (enemy->enemySpeed < 0 && bg[bgL][bgT].coll && bg[bgL][bgB].coll){
				int dist = (32 - (((int)enemy->posX) % 32));
				enemy->posX += dist;
				enemy->enemySpeed = -1 * enemy->enemySpeed;
				enemy->move = false;
			}
			//hit right wall
			else if (enemy->enemySpeed > 0 && bg[bgR][bgT].coll && bg[bgR][bgB].coll){
				int dist = (((int)enemy->posX) % 32);
				enemy->posX -= dist;
				enemy->enemySpeed = -1 * enemy->enemySpeed;
				enemy->move = false;
			}
			else{
				//not hiting wall so move sideways
				if (enemy->move){
					enemy->posX += enemy->enemySpeed*dt;
				}
			}
		}
		//hitting the ground
		if (bg[bgL][bgB].coll && bg[bgR][bgB].coll){
			int dist = (((int)enemy->posY) % 32);
			enemy->posY -= dist - 1;
			enemy->move = true;
		}
		//mid air
		if (!(bg[bgL][bgB].coll && bg[bgR][bgB].coll)){
			enemy->posX += enemy->enemySpeed*0.1*dt;
			enemy->posY += (ROLLGRAVITY*dt);
		}
		////////////////////////////////////////////////////////////////////////
		///////////
		////////////
		///////////////////////////////////////////
	}
	else if (enemy->type == 2){ //goes down every ladder
		if (bg[bgL][bgB].isLadder || bg[bgR][bgB].isLadder){
			enemy->posY += (CLIMBSPEED*dt);
			enemy->move = false;			
		}
		else{
			//pick a direction for enemy to go
			if (bg[bgL][bgB].coll && bg[bgR][bgB].coll){
				if (!enemy->move){
					int ran = rand() % 2 + 1;
					if (ran == 1){
						enemy->enemySpeed = -ROLLSPEED;
					}
					else {
						enemy->enemySpeed = ROLLSPEED;
					}
					enemy->move = true;
				}
			}


			//in the ground but not on top of any ladder
			if (bg[bgL][bgB].coll && bg[bgR][bgB].coll&&!(bg[bgL][bgT].isLadder && bg[bgR][bgT].isLadder)){

				int dist = (((int)enemy->posY) % 32);
				enemy->posY -= dist - 1;
			}
			else if (!(bg[bgL][bgB].coll && bg[bgR][bgB].coll)){
				//mid air
				if (enemy->move){
					enemy->posX += enemy->enemySpeed*0.1*dt;
				}
				enemy->posY += (ROLLGRAVITY*dt);
			}

			//hit left wall
			if (enemy->enemySpeed < 0 && bg[bgL][bgT].coll && bg[bgL][bgB].coll&&!bg[bgL][bgT].isLadder&&!bg[bgL][bgB].isLadder){
				int dist = (32 - (((int)enemy->posX) % 32));
				enemy->posX += dist;
				enemy->enemySpeed = -1 * enemy->enemySpeed;
				enemy->move = false;
				
			}
			//hit right wall
			else if (enemy->enemySpeed > 0 && bg[bgR][bgT].coll && bg[bgR][bgB].coll&&!bg[bgR][bgT].isLadder&&!bg[bgR][bgB].isLadder){
				int dist = (((int)enemy->posX) % 32);
				enemy->posX -= dist;
				enemy->enemySpeed = -1 * enemy->enemySpeed;
				enemy->move = false;				
			}

			if (enemy->move){
				enemy->posX += enemy->enemySpeed*dt;				
			}
		}
	}
}

static bool destroyEnemy(Enemy* enemy){
	int bgL = enemy->posX / TILESIZE;
	int bgT = enemy->posY / TILESIZE;
	int bgR = bgL + 1;
	int bgB = bgT + 1;
	if (enemy->dead){
		return true;
	}
	if (bg[bgL][bgT].destroyZone || bg[bgL][bgB].destroyZone || bg[bgR][bgT].destroyZone || bg[bgR][bgB].destroyZone){		
		return true;
	}
	else if (bgL<0 || bgR>MAPWIDTH || bgB > MAPWIDTH){
		return true;
	}
	return false;
}