
#include "titlescreen.h"
#include "playerImage.h"
#include "enemy.h"
#include "boss.h"
#include "gameover.h"
#include "shoot.h"
#include <xinu.h>

typedef unsigned short u16;
#define RGB(r, g, b) (r | (g << 5) | (b << 10))
// #define REG_DISPCNT *(u16 *)0x4000000
#define extern videoBuffer
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define WHITE RGB(31, 31, 31)
#define BLACK RGB(0, 0, 0)

/*
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
*/
//#define BUTTONS *(volatile unsigned int *)0x4000130
unsigned char *tecla_actual;
#define BUTTON_A	0x24
#define BUTTON_B	0x25 
#define BUTTON_EXIT 0x10 //Presionar 'q' para salir
#define BUTTON_SELECT	0x03
#define BUTTON_START	0x2c
#define BUTTON_RIGHT	0x20 //'d'
#define BUTTON_LEFT	0x1e	
#define BUTTON_UP	0x11 //'w'
#define BUTTON_DOWN 	0x1f //'s'
#define BUTTON_R	'1'
#define BUTTON_L	'2'
#define KEY_DOWN_NOW(key)  ((*tecla_actual) == key)

//variable definitions
#define playerspeed 2
#define enemyspeed 1
#define fastXSpeed 3
#define fastYSpeed 2


void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void delay_galaga();
void waitForVBlank();

//helpers
void initialize();
void drawEnemies();
void endGame();
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY);
int collisionShoot(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY);

//objects
struct Players {
	volatile u16 playerX;
	volatile u16 playerY;
};
struct Enemy {
	volatile u16 enemyX;
	volatile u16 enemyY;
};
struct FastEnemy {
	volatile u16 fastX;
	volatile u16 fastY;
};

int shoots[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int curr_shot = 0;
//Máximo hay 10 balas
#define N_SHOOTS 10
/*void ubicarAlPrincipio(struct Enemy enemies[9]){
	for (int a = 0; a < 9; a++) {
		enemies[a].enemyX = (28*a);
		enemies[a].enemyY = 0;
	}
}*/

int juego(pid32 pid_main,pid32 pid_vidas_puntaje, unsigned char *tecla) {
	//easy enemy wave set setup
	struct Enemy easyEnemies[9];
	tecla_actual=tecla;
	for (int a = 0; a < 9; a++) {
		easyEnemies[a].enemyX = (28*a);
		easyEnemies[a].enemyY = 0;
	}
	easyEnemies[1].enemyX = 240;
	easyEnemies[4].enemyX = 240;
	easyEnemies[8].enemyX = 240;
	//difficult enemies setup
	struct Enemy hardEnemies[9];
	for (int a = 0; a < 9; a++) {
		hardEnemies[a].enemyX = (28*a);
		hardEnemies[a].enemyY = 160;
	}
	hardEnemies[3].enemyX = 240;
	hardEnemies[6].enemyX = 240;
	//player setup
	struct Players player;
	player.playerX = 120;
	player.playerY = 136;
	//fast enemy "boss" setup
	/*struct FastEnemy fast;
	fast.fastX = 0;
	fast.fastY = 30;*/

	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	//initalize title screen
	print_text_on_vga(10, 20, "GALAGA ");
	drawImage3(0, 0, 240, 160, titlescreen);
	while(1) {
		//tecla_actual=(*tecla);
		if (KEY_DOWN_NOW(BUTTON_START)) {
			break;
		}
	}
	//start black screen for drawing
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 160; j++) {
			setPixel(i, j, BLACK);
		}
	}
	while(1) {
    	//tecla_actual=(*tecla);
		//go back to title screen if select button is pressed
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			//initialize();
			galaga();
		}
		//player shots 
		if (KEY_DOWN_NOW(BUTTON_A)) {
			if (shoots[curr_shot] == 0) {
				shoots[curr_shot] = player.playerY*240 + player.playerX+9; /* 24 widht player */
				curr_shot++;
				if (curr_shot >= N_SHOOTS)
					curr_shot = 0;
			};
		}
		//player movement input
		if (KEY_DOWN_NOW(BUTTON_LEFT) && (player.playerX > 0)) {
			player.playerX -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_RIGHT) && (player.playerX <= 216)) {
			player.playerX += playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_UP) && (player.playerY > 25)) {
			player.playerY -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN) && (player.playerY <= 136)) {
			player.playerY += playerspeed;
		}
		//
		if (KEY_DOWN_NOW(BUTTON_EXIT)){
			//Salir
			send(pid_main,1);
		}
		//
		waitForVBlank();
		sleepms(50);
		//draw player
		drawImage3(player.playerX, player.playerY, 24, 24, playerImage);
		drawHollowRect(player.playerX - 1, player.playerY - 1, 26, 26, BLACK);
		drawHollowRect(player.playerX - 2, player.playerY - 2, 28, 28, BLACK);
		//draw easy enemies with downward movement
		for (int a = 0; a < 9; a++) {
			easyEnemies[a].enemyY += enemyspeed;
			drawImage3(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, enemy);
			if (collision(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, player.playerX, player.playerY)) {
				send(pid_vidas_puntaje,'v');//---------------------
				endGame();
				easyEnemies[a].enemyY = 0;
			}
			if (easyEnemies[a].enemyY >= 160) {
				easyEnemies[a].enemyY = 0;
			}
		}

		//draw shots
		for (int i = 0; i < N_SHOOTS; i++) {
			if (shoots[i] != 0) {
				drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
				drawImage3((shoots[i] % 240), (shoots[i] / 240), 5, 5, shoot);
				shoots[i] = shoots[i]-240*4;
				if (shoots[i] <=0)   shoots[i]=0;
			}

			// check hits of shoots
			for (int j = 0; j < 9; j++) {
				if (collisionShoot(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 20, 20, shoots[i] % 240, shoots[i] / 240)) {
					if (shoots[i] != 0) {
						//Avisar al proceso contador que sume un punto
						send(pid_vidas_puntaje,'p');
					}
					drawRect(easyEnemies[j].enemyX, easyEnemies[j].enemyY,  20, 20, BLACK);
					drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
					easyEnemies[j].enemyY = 0;
					shoots[i] = 0;

				}
				if (collisionShoot(hardEnemies[j].enemyX, hardEnemies[j].enemyY, 15, 15, shoots[i] % 240, shoots[i] / 240)){
					if (shoots[i] != 0) {
						//Avisar al proceso contador que sume un punto
						send(pid_vidas_puntaje,'p');
					}
					drawRect(hardEnemies[j].enemyX, hardEnemies[j].enemyY,  20, 20, BLACK);
					drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
					hardEnemies[j].enemyY = 0;
					shoots[i] = 0;
				}
			}
		}
		
		//draw hard enemies
		for (int a = 0; a < 9; a++) {
			hardEnemies[a].enemyY += enemyspeed;
			drawImage3(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 15, 15, boss);
			if (collision(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 15, 15, player.playerX, player.playerY)) {
				send(pid_vidas_puntaje,'v');
				endGame();
			}	
			if (hardEnemies[a].enemyY >= 228) {
				hardEnemies[a].enemyY = 0;
			}
			if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
				hardEnemies[a].enemyY = 160;
			}	
			//space enemies apart
			if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
				hardEnemies[a].enemyY = 160;
			}		
			if ((easyEnemies[a].enemyY >= 120) && (hardEnemies[a].enemyY >=170)) {
				hardEnemies[a].enemyY = 160;
			}							
		}
		//draw fast enemy
		/*drawImage3(fast.fastX, fast.fastY, 15, 15, boss);
		drawHollowRect(fast.fastX - 1, fast.fastY - 1, 17, 17, BLACK);
		drawHollowRect(fast.fastX - 2, fast.fastY - 2, 19, 19, BLACK);
		
		if(collision(fast.fastX, fast.fastY, 15, 15, player.playerX, player.playerY)) {
			send(pid_vidas_puntaje,'v');
		}
		fast.fastX += fastXSpeed;
		fast.fastY += fastYSpeed;
		if (fast.fastX >= 240) {
			fast.fastX = 0;
		}
		if (fast.fastY >= 200){
			fast.fastY = player.playerY - 20;
		}*/
	}	
	return 0;
}

int collisionShoot(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY) {
	//check if bottom right corner of enemy is within player
	if (((enemyX + enemyWidth) > playerX) && ( (enemyY + enemyHeight)
		> playerY ) &&  ((enemyX + enemyWidth) < (playerX + 10))
		&& ((enemyY + enemyWidth) < (playerY + 10))) {
		return 1;
	}
	//check bottom left corner of enemy
	if ( (enemyX < (playerX + 10))
		&& (enemyX > playerX)
		&& ((enemyY + enemyHeight) > playerY)
		&& ((enemyY + enemyHeight) < (playerY + 10))
		) {
		return 1;
	}
	//check top left corner
	if ( (enemyX < (playerX + 10))
		&& (enemyX > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 10))
		) {
		return 1;
	}
	//check top right corner
	if ( ((enemyX + enemyWidth) < (playerX + 10))
		&& ((enemyX + enemyWidth) > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 10))
		) {
		return 1;
	}
	return 0;
}
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY) {
	//check if bottom right corner of enemy is within player
	if (((enemyX + enemyWidth) > playerX) && ( (enemyY + enemyHeight) 
		> playerY ) &&  ((enemyX + enemyWidth) < (playerX + 24)) 
		&& ((enemyY + enemyWidth) < (playerY + 24))) {
		return 1;
	} 
	//check bottom left corner of enemy
	if ( (enemyX < (playerX + 24)) 
		&& (enemyX > playerX)
		&& ((enemyY + enemyHeight) > playerY)
		&& ((enemyY + enemyHeight) < (playerY + 24))
		) {
		return 1;
	}
	//check top left corner
	if ( (enemyX < (playerX + 24)) 
		&& (enemyX > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 24))
		) {
		return 1;
	}	
	//check top right corner
	if ( ((enemyX + enemyWidth) < (playerX + 24)) 
		&& ((enemyX + enemyWidth) > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 24))
		) {
		return 1;
	}	
	return 0;
}

void endGame() {
	//start Game Over State
	drawImage3(0, 0, 240, 160, gameover);
	drawHollowRect(0, 0, 240, 160, WHITE);
	while(1) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			galaga();
		}
		if (KEY_DOWN_NOW(BUTTON_START))	{
			galaga();
		}
	}
}
int puntaje_vidas(void){
	short int puntaje=0;
	short int cantVidas=1;
	char buffer[50];
	umsg32 msg;
	sprintf(buffer,"Puntaje: %d, Vidas: %d",puntaje,cantVidas);
	print_text_on_vga(250, 100, buffer);
	while(1){
		msg=recvclr();
		msg=receive();
		switch(msg){
			case 'p':
				//sumar puntaje
				puntaje++;
				break;
			case 'v':
				//Restar vidas
				cantVidas--;
				break;
		}
		sprintf(buffer,"Puntaje: %d, Vidas: %d",puntaje,cantVidas);
		print_text_on_vga(250, 100, buffer);
	}
}
void temporizador(){
	while(1){
		sleepms(200);

	}
}
void teclado_in(unsigned char *tecla){
	if(open(KEYBOARD,"","")!=-1){
	    while(1){
            read(KEYBOARD,tecla,1);
        }
	}
}

int galaga(){
    unsigned char tecla_actual;
	pid32 vidas_puntaje_pid=create(puntaje_vidas,8000,20,"Vidas y puntaje",0);
	pid32 juego_pid=create(juego,8000,20,"Galaga juego",3,getpid(),vidas_puntaje_pid,&tecla_actual);
	pid32 teclado_pid=create(teclado_in,8000,20,"Teclado",1,&tecla_actual);
    resume(teclado_pid);
    resume(juego_pid);
    resume(vidas_puntaje_pid);

    //terminar el juego
    umsg32 msg=recvclr();
    msg=receive();
    kill(vidas_puntaje_pid);
    kill(juego_pid);
    close(KEYBOARD);
    kill(teclado_pid);
    printf("FIN DEL JUEGO %d\n",msg);	
}
