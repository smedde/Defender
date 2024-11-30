#pragma once

#define CPU 1   //1=68008 2=68000 4=68020

typedef enum { INTRO = 0, PLAY = 1, MOURN = 2, PAUSE = 3, EXIT = 4 } GAMESTATE;

typedef enum { RIGHT = 0, LEFT = 1, BOTH = 2} DIRECTION;

typedef unsigned short word; // .w

#if defined(NKC)

  	//#define SECTION __attribute__ ((section (".data")))

void GDP_CLS();
void GDP_SYNC_POS(); 
void GDP_SYNC_NEG();
void WAIT_MS();

	//NKC libs, original versions
	#include "C:/SysGCC/m68k-elf/bin/libs/NKC_UTIL.h"
	#include "C:/SysGCC/m68k-elf/bin/libs/NKC_SOUND.h"
	#include "C:/SysGCC/m68k-elf/bin/libs/NKC_GDP64.h"
	
	//fixes for bugs in libgcc.a wrt relocation, not sure if still needed with gcc 8.2.1 (yes, when compiling relocativ)
	#include "C:/SysGCC/m68k-elf/bin/libs/libc/divsi3.h"   
	#include "C:/SysGCC/m68k-elf/bin/libs/libc/umodsi3.h"
	#include "C:/SysGCC/m68k-elf/bin/libs/libc/mulsi3.h"

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <math.h>

	//NKC bibl header
	__asm__("dc.b 0x55, 0xAA, 0x01, 0x80");
	__asm__("dc.b 'D', 'E', 'F', 'E', 'N', 'D', 'E', 'R' ");
	__asm__("dc.l _start");  	//
	__asm__("dc.l _bss_end");	//from likerscript
	__asm__("dc.b 0x01");
	__asm__("dc.b 0x01");
	__asm__("dc.b 0x00, 0x00");
	__asm__("dc.l 0x00");

#else

	//windows versions of NKC libs
	#include "olcNKC_UTIL.h"
	#include "olcNKC_GDP64.h"    //emulates (essenial) GDP64 comands in WINDOWS
	#include "C:/SysGCC/m68k-elf/bin/libs/NKC_SOUND.h"  //#include "olcNKC_SOUND.h"

#endif




#include "console.h"

#define MAX_NO_DYN_OBJ 150
#define MAX_NO_CYCLIC_SPRITES 2


word FPMM = 10;  //fixed-point math multiplier, 10 bits = 1024

typedef enum {

	UNDEFF    = 0x00,
	SPACESHIP = 0x11,
	THRUST    = 0x22,
	LASERBEAM = 0x33,
	ENEMYSHIP = 0x44,
	//ENEMYFIRE = 0x03,
	DEBRIS    = 0x55
	
} OBJECT_TYPE_DEFENDER;

typedef enum {

	SMALL_STAR = 0x00,
	MID_STAR   = 0x01,
	LARGE_STAR = 0x02

} STAR_TYPE;

typedef struct GameStats
{
	int noOfLives;
	int level;
	int score;
	int highScore;
	byte isDirty;      //flag if changes happend

} GameStats;



//globals
int gGameState = INTRO;  //defines how program starts

int gXPosPlayer = 0;
int gYPosPlayer = 0;

int gDirection = RIGHT;

int gSpaceshipSpeed = 0;
int gLaserbeamSpeed = 50;

int gTick = 0;
int gFrame = 1;   //must starst at 1, for sprite change all 5 frames with %

int gNoOfDynObj = 0;
int gNoOfDynUNDEFObj = 0;

#if defined(_WIN32)
	static inline GameStats gGameStats;
#else
	static GameStats gGameStats;
	//forward declerations
	void laser();
	void prep();
#endif

int gXPosWorld = 10000;  //ref for visible world  
int gXPosWorldPrev[2] = { 10000, 10000};

int gXPosStep = 0;

int gTerrainIsDirty = FALSE;

byte text_score[6]        = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x00 };
byte text_highScore[6]    = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x00 };
byte text_score_blanks[6] = { 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x00 };

byte numbers[22] = { 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x40, 0x00 };
byte numbers2[22] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x00 };

byte SPRITE_SPACESHIP_RIGHT[80] = { v3WW, v3WW, v3WW, v3WW, v2WW, v1NE, v1NW, v1NE, v1SS, v1NE, v2SS, v1EE, v3NN, v1EE, v3SS, v1EE, v3NN, v1EE,
									v3SS, v1EE, v2NN, v1EE, v2SS, v1EE, v1NN, v1EE, v1SS, v3EE, v3EE, v2EE, 0x03, v1EE, 0x02, v1EE, 0x03, v1EE,
									0x02, v2EE, v1SE, 0x03, v1EE, 0x02, v1EE, 0x03, v1EE, 0x02, v2EE, v2WW, 0x03, v1WW, 0x02, v1WW, 0x03, v1WW, 
									0x02, v3WW, v3WW, v3WW, v1WW, 0x00};

byte SPRITE_SPACESHIP_LEFT[80] = {	v3EE, v3EE, v3EE, v3EE, v3EE, v1NW, v1NE, v1WW, v1NN, v1WW, v2SS, v1WW, v3NN, v1WW, v3SS, v1WW, v3NN, v1WW, 
									v3SS, v1WW, v2NN, v1WW, v2SS, v1WW, v1NN, v1WW, v1SS, v3WW, v3WW, v2WW, v1SW, v1NW, v1SW, v1NW, v1WW, v1SW, 
									v1SW, v1NW, v1SW, v1NW, v1WW, 0x03, v3EE, v3EE, 0x02, v3EE, v3EE, v3EE, 0x00};

byte SPRITE_THRUST_RIGHT[5] = { v3EE, v3EE, v3EE, v2NW, 0x00 };
byte SPRITE_THRUST_LEFT[5] =  { v3WW, v3WW, v3WW, v2NE, 0x00 };

byte SPRITE_DEBRIS_SMALL[2] = { v1EE, 0x00 };
byte SPRITE_DEBRIS_LARGE[4] = { v2EE, v1SS, v2WW, 0x00 };

byte SPRITE_STAR_LARGE[4] = { v2EE, v1SS, v2WW, 0x00 };
byte SPRITE_STAR_MID[2]   = { v2SE, 0x00 };
byte SPRITE_STAR_SMALL[2] = { 0x31, 0x00 };

byte SPRITE_EMPTY[2] = { 0x00, 0x00 };  //needed?

byte emptyList[2] = { 0x00, 0x00 };  //use this


byte SPRITE_ENEMYSHIP_1[51]   = {vUP, v1NN, vDOWN, v1SE, v2EE, v1WW, v1NW, v1NE, v1EE, v1NN, v3WW, v1NN, v3EE, v1NW, v2EE, 
						v1NN, v2EE, v1NN, v3EE, v1SS, v2WW, v1SE, v3WW, v1EE, v2SS, v2WW, v3EE, v1SW, v3EE, v2NW, 
						v2EE, v1SS, v3EE, v1SS, v1EE, v1SE, v1EE, v1SW, v2WW, vUP, v3NN, vDOWN, v3EE, v1NN, v3WW, 
						v1NE, v3WW, v1WW, v1NE, v1EE, 0x00}; 

byte SPRITE_ENEMYSHIP_2[51]   = {v1EE, v1NE, v1EE, v1SE, v1EE, v1NE, v3EE, v1SE, v1EE, v1NE, v1EE, v1SE, v1EE, vUP, v3NN, 
						vDOWN, v3WW, v1SW, v1WW, v1NE, v3WW, v3WW, v1SS, v1WW, v1NN, v3WW, v1WW, v1NN, v3EE, v1NW, 
						v2EE, v1NN, v1EE, v1SS, v1SE, v3EE, v1NE, v3EE, v1SE, v1EE, v3WW, v1NW, v1NN, v2WW, v1NN, 
						v3WW, v2SS, v3EE, v1NW, v1WW, 0x00};



//byte SPRITE_ENEMYSHIP[16] = { v3NE, v3SS, v3SS, v3WW, v3WW, v3WW, v3WW, v3NN, v3NN, v3EE, v3EE, v3EE, v3EE, 0x00 };
//byte SPRITE_ENEMYSHIP[16] = { 0x41, 0x42, 0x43, 0x44, 0x00 }; PEN_UP, v3NE, PEN_DOWN,


int gMaxSpaceshipSpeed = 8;
int gMaxGravity = 5;


typedef struct DynVisObjDefender
{
	OBJECT_TYPE_DEFENDER type;

	int  ID;
	byte score;
	byte isDirty[2];
	byte alive[2];
	word xPos;
	word yPos;

	word dXPos;  //movement vector
	word dYPos;
	
	byte direction;   //for laserbeams

	byte isSelfRemoving;  //has limited lifetime if > 0

	byte isCyclic;
	word cyclicCounter;
	word cyclicCounterThreshold;
	byte actCmdListNo;    //index of cmd list (sprite) to draw
	byte* cmdList[6];     //up to 6 ptr to sprites

	word xPosPage[2];	  //store draw setup for a page
	word yPosPage[2];
	byte* cmdListPage[2];

} DynVisObjDefender;

DynVisObjDefender listDynDefender[MAX_NO_DYN_OBJ];

//convinience pointer to spaceship and thrusts
DynVisObjDefender* gPlayer; 
DynVisObjDefender* gPlayerThrustRight;
DynVisObjDefender* gPlayerThrustLeft;

/*
//https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
function ptInTriangle(p, p0, p1, p2) {
	var dX = p.x - p2.x;
	var dY = p.y - p2.y;
	var dX21 = p2.x - p1.x;
	var dY12 = p1.y - p2.y;
	var D = dY12 * (p0.x - p2.x) + dX21 * (p0.y - p2.y);
	var s = dY12 * dX + dX21 * dY;
	var t = (p2.y - p0.y) * dX + (p0.x - p2.x) * dY;
	if (D < 0) return s <= 0 && t <= 0 && s + t >= D;
	return s >= 0 && t >= 0 && s + t <= D;
}
*/

void printDebug() {

#if defined(_WIN32)

	std::cout << "     gFrame: " << gFrame << std::endl;
	std::cout << "      gTick: " << gTick << std::endl;
	std::cout << "gNoOfDynObj: " << gNoOfDynObj << std::endl;

	for (int i = 0; i < gNoOfDynObj; i++) {

		std::cout << "#" << i << " " << listDynDefender[i].type << " | ";
		std::cout << listDynDefender[i].alive[0] + 0 << " ";
		std::cout << listDynDefender[i].alive[1] + 0 << " ";
		std::cout << listDynDefender[i].isDirty[0] + 0 << " ";
		std::cout << listDynDefender[i].isDirty[1] + 0 << " ";
		std::cout << std::endl;
	}

	std::cout << "--- --- --- ---" << std::endl;
	//std::cin.get();
#endif
}

int pointInDynOby(int x, int y, DynVisObjDefender* o) {

	int result = FALSE;

	int xsize = 8 * 2;
	int ysize = 10;

	if ((x >= o->xPos) && (x <= o->xPos + xsize)) {
		if ((y >= o->yPos) && (y <= o->yPos + ysize)) {
			result = TRUE;
		}
	}
	return result;
}

void movePlayer(signed char x, signed char y) {

	//static byte counter = 0;
	//static byte gravityCounter = 0;

	//if ((gXPosWorld < 0) || (gXPosWorld > 2000)) {
		
	//}

	gXPosWorld += x;  //for terrain
	gTerrainIsDirty = TRUE;

	gXPosStep = -x;

	gXPosPlayer -= x;
	gYPosPlayer += y;
	
	//thrust graphics
	if (gDirection == RIGHT) {
		gPlayer->actCmdListNo = 0; 
		gPlayerThrustRight->actCmdListNo = 0;
		gPlayerThrustRight->isDirty[NKClib_page] = TRUE;
		gPlayerThrustLeft->actCmdListNo = 1;
		gPlayerThrustLeft->isDirty[NKClib_page] = TRUE;
	} else {
		gPlayer->actCmdListNo = 1; 
		gPlayerThrustRight->actCmdListNo = 1;
		gPlayerThrustRight->isDirty[NKClib_page] = TRUE;
		gPlayerThrustLeft->actCmdListNo = 0;
		gPlayerThrustLeft->isDirty[NKClib_page] = TRUE;
	}

	//clip spaceship movement to screen
	if (gXPosPlayer <= 150) gXPosPlayer = 150;
	if (gXPosPlayer >= 512-150) gXPosPlayer = 512-150;
	
	if (gYPosPlayer <= 20) gYPosPlayer = 20;
	if (gYPosPlayer >= 256-40) gYPosPlayer = 256-40;

	gPlayerThrustRight->xPos = gXPosPlayer - 25;
	gPlayerThrustLeft->xPos = gXPosPlayer + 25;
	
	gPlayerThrustRight->yPos = gYPosPlayer;
	gPlayerThrustLeft->yPos = gYPosPlayer;

	if ((gTick % 20) == 0) {
		if (gSpaceshipSpeed < gMaxSpaceshipSpeed) {
			gSpaceshipSpeed += 1;
		}
	}
	
}

int gLastTickFired = -1;

int joystickAccel = 0;
int joystickAccelInc = 1;

int minJoystickAccel = 1;
int maxJoystickAccel = 1;
int joystickFireAllowed = TRUE;
int joystickRamp = 0;			//min iterations before joystick acc kicks in
int withFire = 0;		

int doneJoystickMove = FALSE;
int dauerfeuerprevent = 5;

//void doJoystickFire()      { laser(); 	addToSoundQueue(CHANNEL_C, sound_bing, sizeof(sound_bing) / sizeof(sound_bing[0]));  
//void doJoystickFire()      { laser(); 	addToSoundQueue(CHANNEL_B, captain, sizeof(captain) / sizeof(captain[0]));  }

void doJoystickFire()      { laser(); 	addToSoundQueue(CHANNEL_B, sonic, sizeof(sonic) / sizeof(sonic[0]));  }

void doJoystickUp()        { movePlayer(0,  gSpaceshipSpeed * 1);  	addToSoundQueue(CHANNEL_A, sound_bang1, sizeof(sound_bang1) / sizeof(sound_bang1[0])); }
	
void doJoystickDown()      { movePlayer(0, -gSpaceshipSpeed * 1);   addToSoundQueue(CHANNEL_A, sound_bang2, sizeof(sound_bang2) / sizeof(sound_bang2[0])); }

void doJoystickRight()     { gDirection = RIGHT; movePlayer( gSpaceshipSpeed * 2, 0); addToSoundQueue(CHANNEL_A, sound_bang0, sizeof(sound_bang0) / sizeof(sound_bang0[0])); }

void doJoystickLeft()      { gDirection = LEFT;  movePlayer(-gSpaceshipSpeed * 2, 0); addToSoundQueue(CHANNEL_A, sound_bang0, sizeof(sound_bang0) / sizeof(sound_bang0[0])); }

void doJoystickUpLeft()    { gDirection = LEFT;  movePlayer(-gSpaceshipSpeed * 2,  gSpaceshipSpeed * 1); addToSoundQueue(CHANNEL_A, sound_bang1, sizeof(sound_bang1) / sizeof(sound_bang1[0]));}
	
void doJoystickUpRight()   { gDirection = RIGHT; movePlayer( gSpaceshipSpeed * 2,  gSpaceshipSpeed * 1); addToSoundQueue(CHANNEL_A, sound_bang1, sizeof(sound_bang1) / sizeof(sound_bang1[0]));}
	
void doJoystickDownLeft()  { gDirection = LEFT;  movePlayer(-gSpaceshipSpeed * 2, -gSpaceshipSpeed * 1); addToSoundQueue(CHANNEL_A, sound_bang2, sizeof(sound_bang2) / sizeof(sound_bang2[0])); }

void doJoystickDownRight() { gDirection = RIGHT; movePlayer( gSpaceshipSpeed * 2, -gSpaceshipSpeed * 1); addToSoundQueue(CHANNEL_A, sound_bang2, sizeof(sound_bang2) / sizeof(sound_bang2[0])); }

void doJoystickNothing() {

	if ((gTick % 50) == 0) {
		if (gSpaceshipSpeed > 0) {
			//gSpaceshipSpeed--;
		}
	}	

	//gravity
	if ((gTick % 5) == 0) { movePlayer(0, -1);}

   //movePlayer(1, 0);
   //movePlayer(-1, 0);

	gXPosStep = 0;
}

#include "C:/SysGCC/m68k-elf/bin/libs/NKC_JOYSTICK.h"


void processKeyboard() {

	byte theKey = 0x00;      //init added
	
#if defined(NKC)

	//byte keyd = KEY_REG;
	//byte keys = KEY_PORT_REG;  

	//pageFlip_2(); DebugPrintPos(10, 200, "   ", 0, NONE); 
	//pageFlip_2(); DebugPrintPos(10, 200, "   ", 0, NONE); 

	//pageFlip_2(); DebugPrintPos(10, 200, "", keyd, HEX); 
	//pageFlip_2(); DebugPrintPos(10, 200, "", keyd, HEX); 


	//theKey = keyd;
	theKey = readKey();

	if (theKey == 0) {
		theKey = readKey();
		theKey = theKey - 128;  //  && 0b01111111;  //mask STROBE
#else

	theKey = 0x00;

	if (TRUE) {   	

	    if (GetKey(olc::Key::S).bHeld) { theKey = 's'; }
		if (GetKey(olc::Key::D).bHeld) { theKey = 'd'; }
		if (GetKey(olc::Key::E).bHeld) { theKey = 'e'; }
		if (GetKey(olc::Key::X).bHeld) { theKey = 'x'; }
		
		if (GetKey(olc::Key::Z).bHeld) { theKey = 'z'; }
		if (GetKey(olc::Key::R).bHeld) { theKey = 'r'; }
		if (GetKey(olc::Key::C).bHeld) { theKey = 'c'; }
		if (GetKey(olc::Key::W).bHeld) { theKey = 'w'; }

		if (GetKey(olc::Key::Q).bHeld) { theKey = 'q'; }
		if (GetKey(olc::Key::M).bHeld) { theKey = 'm'; }

		if (GetKey(olc::Key::B).bHeld) { theKey = 'b'; }
		if (GetKey(olc::Key::P).bHeld) { theKey = 'p'; }

		if (GetKey(olc::Key::SPACE).bPressed) { theKey = ' '; } //avoid Dauerfeuer

#endif

	    if ((theKey == ' ') && (gGameState == INTRO)) {
			gGameState = PLAY; 
			prep();
		}

		byte done = FALSE;

		if (theKey == 'd') {doJoystickRight(); done = TRUE;}
		if (theKey == 's') {doJoystickLeft();  done = TRUE;}
		if (theKey == 'e') {doJoystickUp();    done = TRUE;}
		if (theKey == 'x') {doJoystickDown();  done = TRUE;}

		if (theKey == ' ') {doJoystickFire();  done = TRUE;}
		if (theKey == 'q') {
			gGameState = EXIT; 
			done = TRUE;
		}

		if (theKey == 'p') { 
			done = TRUE; 
		}

		if (done == FALSE) {doJoystickNothing();}

	}
	/*pageFlip_2(); DebugPrintPos(10, 150, (char*)"speed:   ", 0, NONE);
	pageFlip_2(); DebugPrintPos(10, 150, (char*)"speed:   ", 0, NONE);

	pageFlip_2(); DebugPrintPos(10, 150, (char*)"speed:", gSpaceshipSpeed, DEC); 
	pageFlip_2(); DebugPrintPos(10, 150, (char*)"speed:", gSpaceshipSpeed, DEC);*/

}


void initMemory() {

#if defined (NKC)
	
	//clear BSS section
	int* text_sta = &_text_sta;
	int* bss_start = &_bss_sta;
	int* bss_end = &_bss_end;

	int size = bss_end - bss_start;
	memset(bss_start, 0x00, size);

	/*DebugPrintPos(10, 100, "start:", *bss_start, HEX); pageFlip_2();
	DebugPrintPos(10, 100, "start:", *bss_start, HEX); pageFlip_2();

	DebugPrintPos(10, 120, "size:", size, HEX); pageFlip_2();
	DebugPrintPos(10, 120, "size:", size, HEX); pageFlip_2();

	WAIT_MS(2000);*/
#endif
}

void updateGameScore(int val) {

	gGameStats.score += val;
}

void drawStaticText() {

	//GDP_CSIZE(0x21);

	text_score[0] = numbers[((gGameStats.score / 1000) % 10)];
	text_score[1] = numbers[((gGameStats.score / 100) % 10)];
	text_score[2] = numbers[((gGameStats.score / 10) % 10)];
	text_score[3] = numbers[((gGameStats.score / 1) % 10)];
	text_score[4] = 0x00;

	for (int i = 0; i < 2; i++) {

		pageFlip_2();
		CMDPRINT3(10, 230, text_score);

		//DebugPrintPos(10, 20, (char*)"rel:", gXPosWorld-32768, DEC);
		//DebugPrintPos(10, 10, (char*)"abs:", gXPosWorld , DEC);

		//draw noOfLives at top, two rows
		//for (int p = 0; p < gGameStats.noOfLives; p++) {
		//	CMDPRINT3(400 + (p * 40), 254 - 10, SPRITE_SPACESHIP_RIGHT);
		//}
	}
	//GDP_CSIZE(0x21);
}

void initObj() {

	int size = sizeof(listDynDefender[0]);  // 76 byte

	//clean and prep dyn list
	for (int i = 0; i < MAX_NO_DYN_OBJ; i++) {

		listDynDefender[i].type = UNDEFF;
		listDynDefender[i].score = 0;
		listDynDefender[i].isDirty[0] = FALSE;
		listDynDefender[i].isDirty[1] = FALSE;
		listDynDefender[i].alive[0] = FALSE;
		listDynDefender[i].alive[1] = FALSE;
		listDynDefender[i].xPos = 0;
		listDynDefender[i].yPos = 0;
		listDynDefender[i].xPosPage[0] = 0;
		listDynDefender[i].yPosPage[0] = 0;
		listDynDefender[i].xPosPage[1] = 0;
		listDynDefender[i].yPosPage[1] = 0;

		listDynDefender[i].dXPos = 0;
		listDynDefender[i].dYPos = 0;
	
		listDynDefender[i].isCyclic = FALSE;
		listDynDefender[i].cyclicCounter = 0;
		listDynDefender[i].cyclicCounterThreshold = 0;
		listDynDefender[i].isSelfRemoving = FALSE;
		
		listDynDefender[i].actCmdListNo = 0;
		for (int i = 0; i < MAX_NO_CYCLIC_SPRITES; i++) {
			listDynDefender[i].cmdList[i] = (byte*)(&emptyList);
		}
		listDynDefender[i].cmdListPage[0] = (byte*)(&emptyList);
		listDynDefender[i].cmdListPage[1] = (byte*)(&emptyList);
	}
	gNoOfDynObj = 0;
}

DynVisObjDefender* addDynObj(OBJECT_TYPE_DEFENDER _type, word _xpos, word _ypos, word _dX, word _dY, byte* _cmd) {

	
	//finds first empty slot
	int pos = 0;
	while (pos < MAX_NO_DYN_OBJ) {
		if (listDynDefender[pos].type == UNDEFF) { break; }
		pos++;
	}
	//clean
	memset(&listDynDefender[pos], 0x00, sizeof(listDynDefender[0]));

	//update max 
	if (pos > gNoOfDynObj) { gNoOfDynObj = pos; }

	//debug
	if (gNoOfDynObj == MAX_NO_DYN_OBJ) { DebugPrintPos(200, 100, (char*)"ERROR", 0, NONE);};

	//do object settings
	listDynDefender[pos].type = _type;
	listDynDefender[pos].xPos = _xpos;
	listDynDefender[pos].yPos = _ypos;
	listDynDefender[pos].cmdList[0] = _cmd;

	listDynDefender[pos].dXPos = _dX;
	listDynDefender[pos].dYPos = _dY;

	listDynDefender[pos].ID = gTick; //for debug

	listDynDefender[pos].actCmdListNo = 0;
	listDynDefender[pos].isSelfRemoving = FALSE;
	listDynDefender[pos].isCyclic = FALSE;

	listDynDefender[pos].cmdListPage[0] = listDynDefender[pos].cmdListPage[1] = emptyList;  //(2) and then this not needed
	listDynDefender[pos].isDirty[0] = listDynDefender[pos].isDirty[1] = TRUE;       //(1) maybe set to FALSE?
	listDynDefender[pos].alive[0] = listDynDefender[pos].alive[1] = TRUE;

	//for debug, count UNDEFS
	int count = 0;
	for (int i = 0; i < MAX_NO_DYN_OBJ; i++) {
		if (listDynDefender[i].type == UNDEFF) count++;
	}
	gNoOfDynUNDEFObj = count;

	//return pointer to new element for detailed object settings
	return &listDynDefender[pos];
}

void doMovements() {

	//int rnd = getRandom();

	for (int i = 0; i <= gNoOfDynObj; i++) {

		DynVisObjDefender* e = &listDynDefender[i];

		//int oldX = e->xPos;
		//int oldY = e->yPos;

		//check for auto death
		if (e->isSelfRemoving > 0) {
			e->isSelfRemoving--;
			if (e->isSelfRemoving == 0) {
				e->alive[NKClib_page] = FALSE;
				e->isDirty[0] = TRUE;
				e->isDirty[1] = TRUE;
			}
		}

		//alternate cyclic objects
		if (e->isCyclic == TRUE) {
			e->cyclicCounter++;
			if (e->cyclicCounter > e->cyclicCounterThreshold) {
				e->cyclicCounter = 0;
				e->actCmdListNo++;
				e->isDirty[NKClib_page] = TRUE;
				if (e->actCmdListNo >= MAX_NO_CYCLIC_SPRITES) {
					e->actCmdListNo = 0;
				}
			}
		}

		switch (e->type) {
	
			case SPACESHIP:
				e->xPos = gXPosPlayer;
				e->yPos = gYPosPlayer;
				e->isDirty[NKClib_page] = TRUE;
				break;
			 
			case THRUST:
				e->xPos = gXPosPlayer;
				e->yPos = gYPosPlayer;
				e->isDirty[NKClib_page] = TRUE;
				break;

			case LASERBEAM:
				e->xPos += e->dXPos;
				e->isDirty[NKClib_page] = TRUE;
				break;

			case ENEMYSHIP:
				//move towards player
				if (getRandom() > 30) {

					int speed = getRandom() / 20;

					if ((gXPosPlayer + gXPosWorld) > e->xPos) {
						e->xPos += speed;
					}
					else {
						e->xPos -= speed;
					}
					if (gYPosPlayer > e->yPos) {
						e->yPos += speed;
					}
					else {
						e->yPos -= speed;
					}
				}
				e->isDirty[NKClib_page] = TRUE;
				break;

			case DEBRIS:
				e->xPos += e->dXPos;
				e->yPos += e->dYPos;
				e->isDirty[NKClib_page] = TRUE;
				break;
	
		}
	}

	//if (gNoOfDynObj == 1) {
	//	int test = 0;
	//}
}

void drawDynObj() {

	GDP_CSIZE(0x21);

	for (int i = 0; i <= gNoOfDynObj; i++) {

		catchSync();

		DynVisObjDefender* e = &listDynDefender[i];

		if (e->type != UNDEFF) {
			if (e->alive[NKClib_page] == TRUE) {
				if ((e->isDirty[NKClib_page] == TRUE)) {
					if (e->type != LASERBEAM) {

						//spaceship or enemy or debris or thrust
						if (e->type == ENEMYSHIP) {
							if ((e->xPos > gXPosWorld) && (e->xPos < gXPosWorld + 512)) {
								CMDPRINT3(e->xPos - gXPosWorld, e->yPos, e->cmdList[e->actCmdListNo]);
								e->xPosPage[NKClib_page] = e->xPos - gXPosWorld;
								e->yPosPage[NKClib_page] = e->yPos;
							}
						}
						else {
							CMDPRINT3(e->xPos, e->yPos, e->cmdList[e->actCmdListNo]);
							e->xPosPage[NKClib_page] = e->xPos;
							e->yPosPage[NKClib_page] = e->yPos;
						}

						e->cmdListPage[NKClib_page] = e->cmdList[e->actCmdListNo];
						e->isDirty[NKClib_page] = FALSE;

						//}

					} else {
						
						//vector object, laserbeam
						int sign = 1;
						if (e->direction == LEFT) { sign = -1; }

						GDP_MOVETO(e->xPos - (15 * sign), e->yPos);
						GDP_SET_VECTOR_TYPE(VECTOR_TYPE_DOTTED);
						GDP_DRAWTO(e->xPos + (15 * sign), e->yPos);       //GDP_HLINE(e->xPos + (15 * sign));
						GDP_SET_VECTOR_TYPE(VECTOR_TYPE_DOTTED_DASHED);
						GDP_DRAWTO(e->xPos + (45 * sign), e->yPos);       //GDP_HLINE(e->xPos + (45 * sign));
						GDP_SET_VECTOR_TYPE(VECTOR_TYPE_DASHED);
						GDP_DRAWTO(e->xPos + (65 * sign), e->yPos);       //GDP_HLINE(e->xPos + (65 * sign));
						GDP_SET_VECTOR_TYPE(VECTOR_TYPE_CONTINOUS);
						GDP_DRAWTO(e->xPos + (85 * sign), e->yPos);       //GDP_HLINE(e->xPos + (85 * sign));   

						e->xPosPage[NKClib_page] = e->xPos;
						e->yPosPage[NKClib_page] = e->yPos;
						e->isDirty[NKClib_page] = FALSE;
						
					}
				}
			}
		}
	}
}

void clearDynObj() {

	CMD(0x01);
	GDP_CSIZE(0x41);

	for (int i = 0; i <= gNoOfDynObj; i++) {

		catchSync();

		DynVisObjDefender* e = &listDynDefender[i];

		if (e->type != UNDEFF) {
			if (e->isDirty[NKClib_page] == TRUE) {
				//GDP_BLANK(e->xPosPage[NKClib_page], e->yPosPage[NKClib_page]);
				if (e->type != LASERBEAM) { //spaceship or enemy or debris or thrust
					CMDPRINT3(e->xPosPage[NKClib_page], e->yPosPage[NKClib_page], e->cmdListPage[NKClib_page]);
					if ((e->alive[NKClib_page] == FALSE) && (e->alive[!NKClib_page] == FALSE)) {
						memset(&listDynDefender[i], 0x00, sizeof(listDynDefender[0]));
					}
				} else {

					//if ((e->xPos > (gXPosPlayer-256)) && (e->xPos < (gXPosPlayer+256))) {

						//vector object, laserbeam 
						int sign = 1;
						if (e->direction == LEFT) { sign = -1; }
						GDP_MOVETO(e->xPosPage[NKClib_page] - (15 * sign), e->yPosPage[NKClib_page]);
						GDP_DRAWTO(e->xPosPage[NKClib_page] + (100 * sign), e->yPosPage[NKClib_page]);       //GDP_HLINE(e->xPosPage[NKClib_page] + (100 * sign));
						if ((e->alive[NKClib_page] == FALSE) && (e->alive[!NKClib_page] == FALSE)) {
							memset(&listDynDefender[i], 0x00, sizeof(listDynDefender[0]));
						}
					//}
				}

				if (e->alive[NKClib_page] == FALSE) e->alive[!NKClib_page] = FALSE;
			}
		}
	}
	CMD(0x00);
}

void laser() {

	//add laser 
	if (gDirection == RIGHT) {
		DynVisObjDefender* ptr = addDynObj(LASERBEAM, gXPosPlayer, gYPosPlayer,  gLaserbeamSpeed, 0, NULL);
		ptr->direction = RIGHT;
	} else {
		DynVisObjDefender* ptr = addDynObj(LASERBEAM, gXPosPlayer, gYPosPlayer, -gLaserbeamSpeed, 0, NULL);
		ptr->direction = LEFT;
	}

	//addToSoundBuffer(&soundBuffer, sound_upFire_ChannelA, sizeof(sound_upFire_ChannelA));
	//addToSoundBuffer(&soundBuffer, sound_Noise, sizeof(sound_Noise));

  	


	//addToSoundQueue(CHANNEL_A, sound_bing, 1);
	
	//addToSoundQueue(CHANNEL_A, sound_bing, 1); 
	

	//addToSoundQueue(CHANNEL_B, sound_bang, 312);
	//addToSoundQueue(CHANNEL_C, sound_bang, 12);

	
	

	/*playBeep(440, 1000);  
	WAIT_MS(50);

	playBeep(880, 500);  
	WAIT_MS(50);

	playBeep(220, 250);  
	WAIT_MS(50);*/

	//debug_printSoundQueue();

	//updateGameScore(10);
}

void doCollisions() {

	static int done = FALSE;

	//clean debris out of screen
	for (int i = 0; i <= gNoOfDynObj; i++) {

		DynVisObjDefender* e = &listDynDefender[i];

		if (e->type == DEBRIS) {
			if ((e->xPos >= 512) || (e->yPos >= 256-0) || (e->xPos <= 0) || (e->yPos <= 0)) {
				e->alive[NKClib_page] = FALSE;
			}
		}

		if (e->type == LASERBEAM) {
			
			//remove if out of screen
			if (e->direction == RIGHT) {
				if (e->xPos > 500) {
					e->alive[NKClib_page] = FALSE;
				}
			} else {
				if (e->xPos < 90) {
					e->alive[NKClib_page] = FALSE;
				}
			}

			//laser vs enemyship
			for (int p = 0; p <= gNoOfDynObj; p++) {

				DynVisObjDefender* o = &listDynDefender[p];

				if (o->type == ENEMYSHIP) {

					int result = FALSE;
					int ysize = 6;    //up and down 

					//check if object within lengh of leaserbeam
					if ((o->xPos - gXPosWorld >= e->xPos) && (o->xPos - gXPosWorld <= e->xPos + 100)) {    //laser beam length = 100
						if ((e->yPos >= o->yPos-ysize) && (e->yPos <= o->yPos + ysize)) {
							result = TRUE;
						}
					}

					if (result == TRUE) {

						o->alive[NKClib_page] = FALSE;	//Enemyship
						e->alive[NKClib_page] = FALSE;  //Laserbeam

						//perish graphics
						for (int i = 0; i < 10; i++) {

							int veloX = ((getRandom() - 50));
							int veloY = ((getRandom() - 50) / 2);

							if ((veloX != 0) && (veloY != 0)) {
								DynVisObjDefender* ptr = addDynObj(DEBRIS, o->xPos - gXPosWorld, o->yPos, veloX, veloY, SPRITE_DEBRIS_SMALL);
								//ptr->isSelfRemoving = 10; 
							}
						}

						//updateScore(25);
						//gGameStats.isDirty = TRUE;
					}
				}
			}
		}
	}
}

void drawTerrainOverview() {

	int step = 512 / 8;
	int count = 175;

	GDP_MOVETO(count, 220);

	for (int x = gXPosWorld-1024; x < gXPosWorld + 1024; x = x + step) {
		
		int number = getRandomS(x) / 8;
		GDP_DRAWTO(count, 220 + number);
		count = count + 5;
	}
}

void prep() {

	initMemory();

	initGraphics();
	pageFlip_2();	

	//initSound();

	consoleArgs.x = 0;
	consoleArgs.y = 254 - 6;
	consoleArgs.fontSize = 0;

	gGameStats.noOfLives = 3;
	gGameStats.level = 1;
	gGameStats.score = 0;
	gGameStats.highScore = 0;

	text_score[0] = 0x30;
	text_score[1] = 0x30;
	text_score[2] = 0x30;
	text_score[3] = 0x30;
	text_score[4] = 0x00;

	//zero init variables;	
	gTick = 0;
	gFrame = 1;   //must starst at 1, for sprite change all 5 frames with %

	//initObj();

	GDP_CLS(); pageFlip_2();
	GDP_CLS(); pageFlip_2();
	drawStaticText(); pageFlip_2();
	drawStaticText(); pageFlip_2();

	GDP_SYNC_POS();  //sync game loop
}

void prep2() {

	initMemory();
	initSound();
	initObj();
	
	//playBeep(880, 50);
	//silence();


	//initGraphics();

	gXPosPlayer = 56;
	gYPosPlayer = 128;

	gDirection = RIGHT;

	//place spaceship
	gPlayer = addDynObj(SPACESHIP, gXPosPlayer, gYPosPlayer, 0, 0, SPRITE_SPACESHIP_RIGHT);
	gPlayer->cmdList[0] = SPRITE_SPACESHIP_RIGHT;
	gPlayer->cmdList[1] = SPRITE_SPACESHIP_LEFT;
	gPlayer->actCmdListNo = 0; //start with spceship facing to the right

	gPlayerThrustRight = addDynObj(THRUST, gXPosPlayer-50, gYPosPlayer, 0, 0, SPRITE_THRUST_RIGHT);
	gPlayerThrustRight->cmdList[0] = SPRITE_THRUST_RIGHT;
	gPlayerThrustRight->cmdList[1] = SPRITE_EMPTY;
	gPlayerThrustRight->actCmdListNo = 0;
	
	gPlayerThrustLeft = addDynObj(THRUST, gXPosPlayer+50, gYPosPlayer, 0, 0, SPRITE_THRUST_LEFT);
	gPlayerThrustLeft->cmdList[0] = SPRITE_THRUST_LEFT;
	gPlayerThrustLeft->cmdList[1] = SPRITE_EMPTY;
	gPlayerThrustLeft->actCmdListNo = 1;

	//add enemy
	for (int i = 0; i < 50; i++) {
		 
		int offsetX = 10000 + ((getRandom() - 50) * 30);
		int offsetY = 50 + getRandom() * 2;

		//if ((offsetX > 1000) && (offsetX < 2000)) {
			if ((offsetY > 100) && (offsetY < 220)) {
				DynVisObjDefender* ptr = addDynObj(ENEMYSHIP, offsetX, offsetY, 0, 0, SPRITE_ENEMYSHIP_1);
				ptr->cmdList[1] = SPRITE_ENEMYSHIP_2;
				ptr->isCyclic = TRUE;
				ptr->cyclicCounterThreshold = 5;
			}
		//}	
	}

	//radar area

	/*for (int i=0; i < 2; i++) {

		
		//GDP_SET_VECTOR_TYPE(VECTOR_TYPE_DOTTED);
		GDP_MOVETO(  0, 220);
		GDP_DRAWTO(512, 220);
		//GDP_SET_VECTOR_TYPE(VECTOR_TYPE_CONTINOUS);

		drawTerrainOverview();
		pageFlip_2();
	}*/	

	//DebugPrintPos(256 - 80, 128, (char*)"S O L A R I S", 0, NONE);
}

void iterate2() {

	//while splash screen is showing
	processKeyboard();
	//int fake = getRandom(); //cycle trough random numbers
	
}



void drawTerrain(int refXPos) {

	int step = 512 / 8;
	
	//find first vertex offscreen left, in negative space
	int distToVertex = refXPos % step;
	int vertexPosOffScreen = refXPos - distToVertex;
	int last = getRandomS(vertexPosOffScreen);

	//find y coordinate of intersect with x axis
	int x_dist = step;
	int y_dist = last - getRandomS(vertexPosOffScreen + step);
	
	int slopeFFPM = (y_dist << FPMM) / x_dist;				//float slope = y_dist / (float)x_dist;
    int ysFFPM = (distToVertex * slopeFFPM) >> FPMM;		//int ys = distToVertex * slope;
	int intersec_y = last - ysFFPM;

	GDP_MOVETO(0, 50 + (intersec_y/2));						//GDP_MOVETO(-distToVertex, last); //GDP
	
	//draw rest of terrain until right edge
	for (int x = -distToVertex + step; x <= 511+step; x=x+step) {
			int number = getRandomS(refXPos + x);
			GDP_DRAWTO(x, 50 + (number/2));  //successive DRAWTO's
	}

	//add stars
	for (int x = -distToVertex + step; x <= 511 + step; x = x + step) {
		
		int number = getRandomS(refXPos + x); //sets the seed
		int yLevel = 50 + (number / 2);
			
		for (int i = 0; i < 1; i++) {  //number of stars here

			int xStar = (step / 2) - (((step << FPMM) * getRandom() >> FPMM) / 100);
			int yStar = 50 + (getRandom() << 1);  //scaled to 50...250

			if (yStar > yLevel + 10) {
				GDP_MOVETO(x + xStar, yStar);
				CMD(v1EE); 
			}
		}
	}

}


void iterate() {

#if defined(NKC)
	processJoystick();catchSync();
	asm("move.b 0x00, 0x1FFFF");  //NEG flank on A16 for benchmarking
#else
	processKeyboard();

	//std::cout << "gXPosWorld: " << gXPosWorld << std::endl;
#endif

	doMovements(); catchSync();
	doCollisions();  catchSync();

	//drawTerrainOverview();

	//terrain
	if (gTerrainIsDirty == TRUE) {
		CMD(0x01);//catchSync();
		drawTerrain(gXPosWorldPrev[NKClib_page]);
		CMD(0x00);//catchSync();
		drawTerrain(gXPosWorld);
		gXPosWorldPrev[NKClib_page] = gXPosWorld;
		gTerrainIsDirty = FALSE;
	}

    processJoystick(); catchSync();

	//objects
	clearDynObj();  catchSync();
	drawDynObj();   catchSync();



	//drawStaticText();

	pageFlip_2(); catchSync();
	gTick++;
	gFrame++;

	/*DebugPrintPos(10, 180, (char*)"           ", 0, NONE); pageFlip_2();
	DebugPrintPos(10, 180, (char*)"           ", 0, NONE); pageFlip_2();

	DebugPrintPos(10, 180, (char*)"", gSpaceshipSpeed, DEC); pageFlip_2();
	DebugPrintPos(10, 180, (char*)"", gSpaceshipSpeed, DEC); pageFlip_2();*/	

	doneJoystickMove = FALSE;


	//GDP_SYNC_POS(); 
    //GDP_SYNC_NEG();
}

void _start() {

	//start routine for NKC

	prep();
	prep2();

	
	//while (gGameState == INTRO) {
	//	iterate2();
	//}
//waitForJoystickFire();

	while (gGameState != EXIT) {
		
		iterate();		

	

		//if (gTick >= 100) gGameState = EXIT;

		//DebugPrintPos(10, 100, "inView:", inView, DEC); 
		//DebugPrintPos(10, 110, "t:",      t,      DEC); 
		//DebugPrintPos(10, 120, "",  gTick,  DEC); 
		//pageFlip_2();

		//WAIT_MS(5);

	}
}