/*
	smed's Defender
*/
#if defined(_WIN32)
	//compile with Visual Studio
	#define SECTION /**/
#elseif
	//compile with gcc
	#define NKC
    #define SECTION __attribute__ ((section (".data")))
#endif

#include "Defender.h"

#ifndef NKC_SHAMUS
#define NKC_SHAMUS

//#define NULL 0

//#define MAX_NO_CYCLIC_SPRITES 4
#define SHAMUS_SPEED 1
#define FIRE_SPEED (SHAMUS_SPEED * 6)

/* #if defined(NKC)
	
	//NKC libs, original versions
	#include "C:/SysGCC/m68k-elf/bin/libs/NKC_UTIL.h"
	#include "C:/SysGCC/m68k-elf/bin/libs/NKC_GDP64.h"
	//#include "C:/SysGCC/m68k-elf/bin/libs/NKC_SOUND.h"

	//fixes for bugs in libgcc.a wrt relocation, not sure if still needed with gcc 8.2.1
	#include "C:/SysGCC/m68k-elf/bin/libs/libc/divsi3.h"   
	#include "C:/SysGCC/m68k-elf/bin/libs/libc/umodsi3.h"
	#include "C:/SysGCC/m68k-elf/bin/libs/libc/mulsi3.h"

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <math.h>

	//NKC bibl header
	__asm__("dc.b 0x55, 0xAA, 0x01, 0x80");
	__asm__("dc.b 'S', 'H', 'A', 'M', 'U', 'S', 'V', '1' ");
	__asm__("dc.l _start");  	//
	__asm__("dc.l _bss_end");	//from likerscript
	__asm__("dc.b 0x01");
	__asm__("dc.b 0x01");
	__asm__("dc.b 0x00, 0x00");
	__asm__("dc.l 0x00");

#else

	//windows versions of NKC libs
	#include "olcNKC_UTIL.h"
	#include "olcNKC_GDP64.h"
	//#include "olcNKC_SOUND.h"

#endif

#include "console.h"*/
//#include "alogrithms.h"


typedef signed char sbyte;  

typedef enum {

	UNDEF = 0x00,

	KEYHOLE = 1,
	KEY = 2,
	POTION = 3,
	DOOR = 9,

	SHADOW = 4,
	MYSTERY = 5,
	ROBO_DROID = 6,
	SPIRAL_DRONE = 7,
	SNAP_JUMPER = 8,

	SENSOR = 10,
	SHAMUS = 11,
	OUCH = 12,
	PERISH = 13,

	HOT_SQUARE = 30,
	WALL_H = 40, WALL_V = 41,

	FIRE = 50,
	FIRE_NN = 51, FIRE_NE = 52, FIRE_EE = 53, FIRE_SE = 54,   //clockwise starting from NN
	FIRE_SS = 55, FIRE_SW = 56, FIRE_WW = 57, FIRE_NW = 58,

	FIRE_SHAMUS = 60,
	FIRE_SHAMUS_NN = 61, FIRE_SHAMUS_NE = 62, FIRE_SHAMUS_EE = 63, FIRE_SHAMUS_SE = 64,   //clockwise starting from NN
	FIRE_SHAMUS_SS = 65, FIRE_SHAMUS_SW = 66, FIRE_SHAMUS_WW = 67, FIRE_SHAMUS_NW = 68,

} OBJECT_TYPE;

typedef enum {
		GREEN_KEY = 1, BLUE_KEY = 2, RED_KEY = 3, GREEN_KEYHOLE = 4, BLUE_KEYHOLE = 5, RED_KEYHOLE = 6
} KEYSASSETS;

//used as index in doAI()
typedef enum { NN=0, NE=1, EE=2, SE=3, SS=4, SW=5, WW=6, NW=7 } DIRECTION;
								   /*      1  2  3  4  5  6  7  8  9 10   */
sbyte slope2dirLUT_QNE[5][10] = { /*y=0*/{EE, EE, EE, EE, EE, EE, EE, EE, EE, EE},
								  /*y=1*/{NN, NE, NE, NE, NE, NE, NE, NE, EE, EE},
								  /*y=2*/{NN, NN, NE, NE, NE, NE, NE, NE, NE, NE},
								  /*y=3*/{NN, NN, NN, NE, NE, NE, NE, NE, NE, NE},
								  /*y=4*/{NN, NN, NN, NN, NE, NE, NE, NE, NE, NE}};

sbyte slope2dirLUT_QNW[5][10] = { /*y=0*/{WW, WW, WW, WW, WW, WW, WW, WW, WW, WW},
								  /*y=1*/{NN, NW, NW, NW, NW, NW, NW, NW, WW, WW},
								  /*y=2*/{NN, NN, NW, NW, NW, NW, NW, NW, NW, NW},
								  /*y=3*/{NN, NN, NN, NW, NW, NW, NW, NW, NW, NW},
								  /*y=4*/{NN, NN, NN, NN, NW, NW, NW, NW, NW, NW}};

sbyte slope2dirLUT_QSE[5][10] = { /*y=0*/{EE, EE, EE, EE, EE, EE, EE, EE, EE, EE},
								  /*y=1*/{SS, SE, SE, SE, SE, SE, SE, SE, EE, EE},
								  /*y=2*/{SS, SS, SE, SE, SE, SE, SE, SE, SE, SE},
								  /*y=3*/{SS, SS, SS, SE, SE, SE, SE, SE, SE, SE},
								  /*y=4*/{SS, SS, SS, SS, SE, SE, SE, SE, SE, SE}};

sbyte slope2dirLUT_QSW[5][10] = { /*y=0*/{WW, WW, WW, WW, WW, WW, WW, WW, WW, WW},
								  /*y=1*/{SS, SW, SW, SW, SW, SW, SW, SW, WW, WW},
								  /*y=2*/{SS, SS, SW, SW, SW, SW, SW, SW, SW, SW},
								  /*y=3*/{SS, SS, SS, SW, SW, SW, SW, SW, SW, SW},
								  /*y=4*/{SS, SS, SS, SS, SW, SW, SW, SW, SW, SW}};

typedef struct WayPoints {

	byte noOfWayPts;
	byte nextWaypoint;
	word pts[10][2];

} WayPoints;

typedef struct DynVisObj
{
	OBJECT_TYPE type;
	
	int  ID;
	byte score;
	byte isDirty[2];
	byte alive[2];    
	word xPos;
	word yPos;
	
	byte onWay;
	WayPoints* wayPoints;

	byte displayGroup;
	byte isSelfRemoving;  //has limited lifetime if > 0

	byte isCyclic;		
	word cyclicCounter;	 
	word cyclicCounterThreshold;
	byte actCmdListNo;    //index of cmd list (sprite) to draw
	byte* cmdList[6];     //up to 6 ptr to sprites
	
	word xPosPage[2];	  //store draw setup for a page
	word yPosPage[2];
	byte* cmdListPage[2];

} DynVisObj;

#define OPEN TRUE
#define CLOSED FALSE

typedef struct SensorSquare
{
	OBJECT_TYPE type;
	byte isArmed;
	word xmin;
	word xmax;
	word ymin;
	word ymax;
	void (*func)(int, int, void* );
	//void (*func)(int, int, SensorSquare* sq);
	byte arg1;         //arguments for func()
	byte arg2;
	DynVisObj* obj;

} SensorSquare;

typedef struct GameStats
{
	int noOfLives;
	int level;
	int score;
	int highScore;
	
	int monsterShootProb;

	DynVisObj*    greenKeyPtr;					//KEY - will be replaced by asset management system
	SensorSquare* greenKeyTriggerSensorPtr;		//
	
	DynVisObj*    greenKeyHolePtr;				//KEYHOLE
	SensorSquare* greenKeyHoleTriggerSensorPtr;	//
	SensorSquare* greenKeyHoleWallSensorPtr;	//
	SensorSquare* greenKeyHolePassageSensorPtr;	//

	byte greenKey;		
	byte blueKey;
	byte redKey;
	byte greenKeyHole;
	byte blueKeyHole;
	byte redKeyHole;

	byte Keys[3];		//to be implemented
	byte KeyHole[3];	//to be implemented

	byte Potions[3];
	byte Mysteries[3];

	byte isDirty;      //flag if changes happend

} GameStats;



byte text1_botLine[20] = { 'R', 'O', 'O', 'M', ' ', 'L', 'E', 'V', 'E', 'L', 0x00 }; //ROOM LEVEL 0000
byte text2_botLine[20] = { ' ', ' ', '2', '3', ' ', ' ', ' ', ' ', ' ', '9', 0x00 }; // 00   ONE  0000

//byte     text_score[10] = { ' ', ' ', ' ', ' ', ' ', 0x00 };
//byte text_highScore[10] = { ' ', ' ', ' ', ' ', ' ', 0x00 };

//#define MAX_NO_DYN_OBJ 100
#define MAX_NO_SENSORS 20

DynVisObj listDyn[MAX_NO_DYN_OBJ];
SensorSquare listSensor[MAX_NO_SENSORS];

//typedef enum { INTRO = 0, PLAY = 1, MOURN = 2, PAUSE = 3, EXIT = 4} GAMESTATE;


//globals
//int gGameState = INTRO;  //defines how program starts

int gNoOfDynObj = 0;
int gNoOfSensors = 0;

int gNoOfDynUNDEFObj = 0;
int gNoOfBullets = 0;
int gFrameLastBulletPlayer = 0;  //no of frame last bullet was fired from player
int gFrameLastBulletMonster = 0;
byte gFrameBulletDelay = 0;

int gMaxCountsPerHSYNC = 0;
int gTick = 0;
int gFrame = 1;   //must starst at 1, for sprite change all 5 frames with %
int gDrawHotSquares = TRUE;

DynVisObj* shamusPtr;
//int gXPosPlayer = 0;
//int gYPosPlayer = 0;

int gLoadNewRoom = FALSE;
int gRoomNumber = 0;
int gShamusExpires = FALSE;
DIRECTION gRoomEntryFrom;

#if defined(_WIN32)
	static inline int newRoom = 0;
	static inline int roomEntryFrom = (DIRECTION)0;
#else
	int newRoom = 0;
	int roomEntryFrom = (DIRECTION)0;
#endif	

#if defined(_WIN32)
	static inline GameStats gGameStats;
#else
	static GameStats gGameStats;
#endif

//GameStats* gGameStatsPtr = &gGameStats;  //needed wg. R_68K_PC16

static void SECTION doSomething (int arg1, int arg2, void* sq) 

{

	gGameStats.noOfLives++;
	
}


static void SECTION drinkPotion(int bottleNo, int arg2, void *_sq) {

	SensorSquare* sq = (SensorSquare*)_sq;

	if (gGameStats.Potions[bottleNo] == FALSE) {
		
		//unarm sensor and undisplay potion
		sq->isArmed = FALSE;    
		sq->obj->alive[0] = FALSE;
		sq->obj->alive[1] = FALSE;
		sq->obj->isDirty[0] = TRUE;
		sq->obj->isDirty[1] = TRUE;
		
		gGameStats.Potions[bottleNo] = TRUE;
		gGameStats.noOfLives++;
		gGameStats.isDirty = TRUE;
	}
}

byte emptyList[2] = { 0x00, 0x00 };

// 16 x 8 sprites



byte SPRITE_PERISH_0[50] = {
							0x03, v2NN, 0x02, v1EE, v1SE, v3EE, v1SW, v3EE, v1NE, v1EE, v1SE, v1EE, v1NE, v1EE, v1NE, v1EE,
							v1WW, v1NW, v3WW, v1NW, v1WW, v1SW, v1WW, v1SE, v3WW, v1NW, v1WW, v1NE, v1WW, v1NW, v1WW, v1EE,
							v1NE, v3EE, v1SS, v1WW, v2NE, v3EE, v1SE, v1SW, v1WW, v1EE, v1NE, v3EE, v1SE, v1EE, v1WW, v1SW,
							v1WW, 0x00 };

byte SPRITE_PERISH_1[55] = {
							0x03, v3NN, v1NN, 0x02, v1EE, v1SE, v1SS, v1NE, v1SS, v1EE, v1SS, v1EE, v1SE, v3EE, v1NN, v3WW, 
							v1NW, v1NE, v1EE, v1NE, v1EE, v1SE, v1SS, v1EE, v1SE, v1EE, v1NE, v1EE, v1WW, v1NW, v2WW, v1NE, 
							v3EE, v1NN, v1WW, v3WW, 0x02, v2NN, v1WW, v2SS, 0x03, v2NW, v1WW, 0x02, v1WW, 0x03, v2SS, v1WW, 
							0x02, v1WW, v1NW, v1WW, 0x00 };

byte SPRITE_PERISH_2[50] = {
							0x03, v3NE, 0x02, v1EE, v1SE, v3EE, v1SE, v1EE, v1NE, v1EE, v1WW, v1NW, v3WW, v1NW, v1WW, v1NN, 
							v1EE, v1NE, v3EE, v1SW, v1EE, v1SE, v1EE, 0x00 };

byte SPRITE_PERISH_3[50] = {
							0x03, v3EE, v3EE, v3EE, v1NN, 0x02, v3EE, v2WW, v1NN, v1NE, v1EE, v1WW, v1SW, v1WW, v1NW, v1WW,
							0x00 };

byte SPRITE_OUCH[180] = {    //size: 12 x 48
0x03, v2EE, 0x02, v2EE, v1NN, v1EE, v1SS, v2EE, v1NE, v3EE, v1NE, v1EE, v1NE, v3EE, 0x03, v2NW, 0x02, v3NN, v1NN, v1WW, v3SS,
v1SE, v3EE, v3NN, v1NN, v1EE, v3SS, 0x03, v3SS, 0x02, v1WW, v3EE, v3EE, 0x03, v2NN, 0x02, v1NW, v1WW, v1NE, v1WW, v1NN, v1EE, 
v1NE, v1EE, v1SE, v1EE, 0x03, v2SS, 0x02, v1WW, v1SW, 0x03, v2SS, 0x02, v3EE, v2EE, 0x03, v2NN, 0x02, v3NN, v1NN, v1EE, v2SS,
v3EE, v2NN, v1EE, v3SS, v1SS, v1WW, v1NN, 0x03, v3WW, 0x02, v1SS, 0x03, v2SS, 0x02, v3EE, v3EE, v3EE, 0x03, v2NN, 0x02, v1EE, 
0x03, v2SS, 0x02, v2EE, v1NE, v1EE, v1NE, v1EE, v1NW, v1EE, v1NW, v1EE, v1NW, v1EE, v1NN, v1WW, v1NW, v1WW, v1NW, v2WW, 0x03, 
v2SS, 0x02, v2SS, v1WW, v2NN, 0x03, v2NN, 0x02, v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v1WW, 0x03, v2SS, 
0x02, v1SE, v1EE, v1SW, v1EE, v1SS, v1WW, v1SW, v1WW, v1NW, v1WW, v1NE, v1WW, v1NN, v1EE, v1NE, 0x03, v2NN, 0x02, v3WW, v1WW, 
v1SW, v1WW, v1SW, v1WW, v1SE, v1WW, v1SE, v1WW, v1SE, v1WW, v1SS, v1EE, v1SE, v1EE, v1SE, v3EE, v1SS, v1WW, 0x00 };

byte SPRITE_SHAMUS_RIGHT_2[60] = {v3EE, v1NW, v1EE, 0x03, v3EE, 0x02, v1EE, v1SE, v1EE, v1NE, v1EE, 0x03, v3WW, v3WW, v1NW, 0x02,
						  v1EE, v1NW, v3EE, v2EE, v3WW, v1NW, v1WW, v2NW, v1SS, v3EE, v3EE, v1EE, v1NE, v1EE, v3WW, v1NE, 
						  v3WW, v1SW, v2WW, v1NE, v1EE, v3WW, 0x03, v2NW, 0x02, v3EE, v3EE, v3EE, v1WW, v1NW, v3WW, v2WW, 0x00 };

byte SPRITE_SHAMUS_RIGHT_1[60] = { 0x03, v1NN, 0x02, v1EE, v1NE, v2EE, v2SS, v2WW, v3EE, v2EE, v1WW, v1NW, v2NN, v1WW, v3EE, v3EE, v1EE, 
                           v3WW, v3WW, v1NW, v1WW, v2NW, v1SS, v3EE, v3EE, v1EE, v1NE, v1EE, v3WW, v1NE, v3WW, v1SW, v2WW, v1NE,
	                       v1EE, v3WW, 0x03, v2NW, 0x02, v3EE, v3EE, v3EE, v1WW, v1NW, v3WW, v2WW, 0x00 };

byte SPRITE_SHAMUS[60] = { 0x03, v2EE, 0x02, v3EE, 0x03, v3EE, 0x02, v3EE, v1WW, v1NW, v1WW, 0x03, v3WW, 0x02, v1WW,
						   v1NN, v1EE, 0x03, v3EE, 0x02, v1EE, v1NE, v3EE, v3WW, v3WW, v3WW, v3WW, v1WW, v3EE, v3EE,
						   v3NN, v1NN, v3WW, v1WW, v1SE, v1WW, v1SS, v3EE, v2EE, v1SS, v1NE, v3EE, v2NN, v1SW, v1NN,
						   v3WW, v1SS, 0x03, v3NE, 0x02, v1EE, v3WW, v3WW, v3WW, v2EE, v1NN, v3EE, v2EE, 0x00 };

byte SPRITE_SHAMUS_LEFT_1[60] = { 0x03, v1NN, 0x02, v1EE, v1SE, v1EE, v1NE, v1EE, 0x03, v3EE, 0x02, v1SS, v3EE, v1WW, v1NW, v1WW, 
								  v1NW, v1WW, v1NE, v3WW, v2WW, v3EE, v1NE, v1EE, v1NW, v3WW, v1WW, v1NE, v3WW, v1EE, v1NE, v3EE,
								  v1EE, v1SS, v1SE, v2NN, v1EE, v2SS, v1EE, v2NN, 0x03, v2NE, 0x02, v3WW, v3WW, v3WW, v1EE, v1NE, 
								  v3EE, v2EE, 0x00 };
byte SPRITE_SHAMUS_LEFT_2[60] = { 0x03, v3EE, v1EE, 0x02, v3EE, v2EE, v1NE, v1EE, v1WW, v1NW, v2WW, v1SS, v1WW, v2NN, v3WW, v3WW,
								 v3EE, v3EE, v1EE, v1NN, v1WW, v3NN, v3WW, v1WW, v1SW, v1WW, v3EE, v1SW, v3EE, v2EE, v2NN, v1EE,
								 v2SS, v1EE, v2NN, 0x03, v2NE, 0x02, v3WW, v3WW, v3WW, v2EE, v1NN, v3EE, v2EE, 0x00 };

byte SPRITE_SHAMUS_UPDOWN_2[60] = { 0x03, v2EE, 0x02, v3EE, 0x03, v3EE, 0x02, v3EE, v1WW, v1NW, v3WW, v2WW, v1NW, v1WW, v3EE, 
									v3EE, v3EE, v1NE, v1EE, v1WW, v1NW, v3WW, v1WW, v1SS, v1WW, v1NN, v3WW, v1WW, v1SW, v1WW,
									v1EE, v2NE, v1WW, v1NN, v3EE, v1SE, v1NN, v1SE, v1NN, v3EE, v1SS, v1EE, v1NN, 0x03, v2NN,
									0x02, v3WW, v3WW, v3WW, v1EE, v1NE, v3EE, v2EE, 0x00 };
byte SPRITE_SHAMUS_UPDOWN_1[60] = { 0x03, v2EE, 0x02, v3EE, 0x03, v3EE, 0x02, v3EE, v1WW, v1NW, v1WW, 0x03, v3WW, 0x02, v1WW,
						   v1NN, v1EE, 0x03, v3EE, 0x02, v1EE, v1NE, v3EE, v3WW, v3WW, v3WW, v3WW, v1WW, v3EE, v3EE,
						   v3NN, v1NN, v3WW, v1WW, v1SE, v1WW, v1SS, v3EE, v2EE, v1SS, v1NE, v3EE, v2NN, v1SW, v1NN,
						   v3WW, v1SS, 0x03, v3NE, 0x02, v1EE, v3WW, v3WW, v3WW, v2EE, v1NN, v3EE, v2EE, 0x00 };

byte SPRITE_ROBO_DROID_4[56]   = { 0x03, v3EE, v1EE, 0x02, v1EE, 0x03, v2NW, 0x02, v1WW, v1NW, 
								 v1WW, v2NN, v1SE, v1NN, v3EE, v1SS, v2EE, v1SS, v2EE, v1SS, 
								 0x03, v2SE, 0x02, v1EE, 0x03, v2NW, 0x02, v3NN, v1SW, v1NN, 
								 v1SW, v3NN, v3WW, v1SS, v2EE, v1SW, v3EE, v2EE, v2NN, v2WW, 
								 v3EE, v3SS, v1WW, v1NE, v3EE, v1EE, v2SS, 0x03, v2SS, 0x02, 
								 v1WW, 0x03, v2NN, 0x02, v1NN, 0x00 };

byte SPRITE_ROBO_DROID_0[60] = {
0x03, v2EE, 0x02, v1EE, 0x03, v3EE, v2EE, 0x02, v1EE, 0x03, v3EE, v2EE, 0x02, v1EE, 0x03, v3NN, 0x02, v1WW, v1NE, 
v1WW, v1NE, v3WW, v1SW, v2WW, v1SS, v1SE, v1EE, v1WW, v1NW, v3WW, v1SW, v1WW, v1EE, v2NE, v1EE, v1NE, v2EE, v2WW, 
v2NN, v2EE, v3WW, v3WW, v1WW, v1SS, v3EE, v1EE, v1SS, v3WW, v3WW, v2WW, v2SS, v1EE, v1NN, 0x03, v3EE, 0x02, v2EE,
0x00 };

byte SPRITE_ROBO_DROID_1[60] = {
0x03, v3EE, v1EE, 0x02, v1EE, 0x03, v3EE, v2EE, 0x02, v1EE, 0x03, v1NE, v2EE, 0x02, v1EE, 0x03, v2NN, 0x02, v1WW,
v1NE, v1WW, v1NE, v3WW, v1SW, v2WW, v2SS, v1WW, v1NN, v2WW, v1NE, v1EE, v3WW, v1WW, v1NW, v3WW, v2SS, v1EE, v1SE,
v1EE, 0x03, v2NW, 0x02, v1NE, v3EE, v3EE, v3EE, v1NW, v1EE, v1NN, v3WW, v3WW, v1WW, v1SS, v3EE,
0x00 };

byte SPRITE_ROBO_DROID_2[60] = {
0x03, v1NE, v1EE, 0x02, v1EE, 0x03, v1SE, v2EE, 0x02, v1EE, 0x03, v3EE, v2EE, 0x02, v1EE, 0x03, v2NE, 0x02, v1WW,
v1NE, v1WW, v1NE, v1WW, v1NE, v3WW, v1SW, v2WW, v1SS, v2WW, v1SS, v1WW, v2NN, v2WW, v1NW, v3WW, v2SS, v1EE, v1NN,
v1NE, v2EE, v2NN, v1EE, v2SS, v2EE, v1SS, v1EE, v3NN, v2WW, v3EE, v2SS, v2EE, v2NN, v1WW, v1SS,
0x00 };

byte SPRITE_ROBO_DROID_3[60] = {
0x03, v2EE, 0x02, v1EE, 0x03, v3EE, v2EE, 0x02, v1EE, 0x03, v3EE, v2EE, 0x02, v1EE, v2NW, v1WW, v1EE, v1NE, v1EE, 
v1NW, v1EE, v1NN, v3WW, v1SW, v2WW, v1SS, v3WW, v1SW, v1WW, v2NE, v2EE, v3WW, v1WW, v1NW, v3WW, v2SS, v1EE, v2NN, 
v3EE, v3EE, v3EE, v1EE, v2NN, v3WW, v3WW, v1WW, v2EE, v1SS, v3EE, v1EE,
0x00 };

//byte SPRITE_ROBO_DROID_4[2] = { 0x34, 0x00 };
byte SPRITE_ROBO_DROID_5[2] = { 0x35, 0x00 };
byte SPRITE_ROBO_DROID_6[4] = { 0x36, 0x00 };

byte SPRITE_SHADOW_0[55] = {   //legs up
0x03, v1NE, v3EE, 0x02, v1EE, 0x03, v3EE, 0x02, v1EE, v1NW, v3EE, v1NE, v1EE, v1WW, v1NW, v1WW, v1NW, v3WW, v2WW,
v1SW, v1WW, v1SW, v1WW, v1EE, v1SE, v3EE, 0x03, v3NN, v2NN, v1EE, 0x02, v1EE, v1NE, v3EE, v1NW, v1EE, v1NW, v1EE,
v1NN, v3WW, v3WW, v3WW, v1SE, v1WW, v1SE, v1WW, v1SS, v3EE, v2EE, v2NN, v1WW, v1SS,
0x00 };

byte SPRITE_SHADOW_1[50] = {  //legs straight
0x03, v2EE, 0x02, v3EE, 0x03, v3EE, 0x02, v3EE, v1WW, v1NW, v1WW, v1NN, v1EE, v1NN, v1WW, v1NN, v1EE, v1NN, v3WW, v2WW, v3SS, v2SS,
v1NE, v3NN, 0x03, v3NN, v1EE, 0x02, v1EE, v1NE, v3EE, v1NN, v1WW, v1NN, v1EE, v1NN, v3WW, v1WW, v3SS, v1WW, v3NN,
v3WW, v1WW, v1SS, v1EE, v1SS, v1WW, v1SS, v3EE,
0x00 };

byte SPRITE_SPIRAL_DRONE_1[55] = {
0x03, v3EE, v3EE, 0x02, v3EE, v2EE, v1WW, v1NW, v1WW, v1NN, v3EE, v2NN, v3EE, v1SS, v1EE, v2NN, v1WW, v1SW, v1WW, v1NW, v1WW,
v2SS, v1WW, v2NN, v1WW, v2SS, v1SW, v3WW, v1NN, v3WW, v1NN, v1WW, v2SS, v1EE, v1NE, v2EE, v2NN, v1EE, v2SS, v2EE, v1NW, v1EE,
v1NW, v2NN, v2WW, v3EE, v2EE, v1WW, v1SW, v1SS,
0x00 };

byte SPRITE_SPIRAL_DRONE_0[46] = {
0x03, v3EE, v1EE, 0x02, v3EE, v2EE, v1WW, v1NW, v1WW, v1NW, v3EE, v3EE, v1NE, v1EE, v1SE, v1EE, v2NN, v1WW, v1SS, v3WW, v3WW,
v3WW, v1SW, v2NN, v3WW, v1NN, v1WW, v2SS, v1EE, v1NE, v3EE, v3EE, v3EE, v1NN, v3WW, v3WW, v1WW, v3EE, v1NE, v1NW, v1WW, v3EE,
v2EE, v1WW, v1SW,
0x00 };

byte SPRITE_SPIRAL_DRONE_2[55] = {
0x03, v1NN, 0x02, v2NN, v1EE, v2SS, v1NN, v3EE, v3EE, v3EE, v2SS, v2WW, v3EE, v2EE, v1WW, v1NW, v2NN, v3WW, v3WW, v1WW, v1NN,
v3EE, v3EE, v1EE, v1NE, v3EE, v1SW, v1EE, v2NN, v1WW, v1SW, v3WW, v3WW, v3WW, v2NN, v2WW, v3EE, v2EE, v1WW, v1SW,
0x00 };

byte SPRITE_SPIRAL_DRONE_3[55] = {
0x03, v1NN,  0x02, v2EE, v1SS, v3EE, v2WW, v1NN, v1NE, v3EE, v3EE, v1EE, v1SE, v1SS, v1EE, v1NN, v2EE, v1NN, v1WW, 0x03, v1NW, v2WW,
0x02, v3WW, v3WW, v1WW, v1NN, v3EE, v3EE, v1EE, v1NN, v1NE, v1NN, v2WW, v3EE, v1SS, v2EE, v3WW, v1SW, v3WW, v3WW, v1WW, v1NW,
v1NN, v1WW, v1SS, v2WW, v1SS, v1EE,
0x00 };

byte SPRITE_SNAP_JUMPER[35]  = { v3EE, v1EE, v2WW, v3NN, v2WW, v3NN, v1NN, v3EE, v3EE, v3EE, 
								 v3EE, v3EE, v3SS, v1SS, v2WW, v3SS, v2EE, v3WW, v1WW, 0x03, 
								 v3NN, v2NN, 0x02, v1EE, 0x03, v2SS, 0x02, v3WW, v3WW, v3WW, 
								 0x03, v2NN, 0x02, v1EE, 0x00 };

byte SPRITE_WALL_V[30] = { v3NN, v3NN, v3NN, v3NN, v3NN, v3NN,    //18
						  /*vUP,*/ v3EE, v3EE, v3EE, v3EE, v3EE, v3EE, v3EE, v1EE, vDOWN,				//22
						  v3SS, v3SS, v3SS, v3SS, v3SS, v3SS,    //18
						  0x00 };
byte SPRITE_WALL_H[30] = { v3EE, v3EE, v3EE, v3EE, v3EE, v3EE, v3EE, v1EE,  //22
					       /*vUP,*/ v3NN, v3NN, v3NN, v3NN, v3NN, v3NN, vDOWN,				//18
					       v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v3WW, v1WW,  //22
					       0x00 };											
byte SPRITE_GREEN_KEY[40] = { v3NN, v3NN, v3EE, v2EE, v1SW, v1EE, v1SW, v1EE, v1SW, v3EE, v3EE, v3EE, v2EE, v2WW,
							  v2SS, v1WW, v1NN, v3WW, v1SS, v1WW, v1NN, 0x03, v3WW, 0x02, v1WW, v1SE, v1WW, v1SE, 
					          v3WW, v2WW, v1NE, v3NN, v1NN, 0x00 };

byte SPRITE_MYSTERY_0[40] = { 0x03, v3EE, v3EE, 0x02, v3EE, v1NN, v3WW, 0x03, v2NN, 0x02, v3EE, v1NW, v3EE, v1NW, v3EE,
							 v1NW, v2WW, v3EE, v1NW, v3EE, v1WW, v1NW, v3WW, v1NE, v3WW, v3WW, v1WW, v1SE, v3WW, v1SE,
							 v3WW, v1EE, v1SE, v3EE, 0x00 };

byte SPRITE_MYSTERY_1[40] = { 0x03, v1NE, v3EE, v2EE, 0x02, v3EE, v1NN, v3WW, 0x03, v2NN, 0x02, v3EE, v1NW, v3EE, v1NN,
							  v3WW, v1EE, v1NE, v3EE, v1WW, v1NW, v3WW, v3WW, v1WW, v1SE, v3WW, 0x00 };

byte SPRITE_GREEN_KEYHOLE[100] = { 0x03, v3EE, v1EE, 0x02, v3EE, v3EE, v1EE, v1NE, v1EE, v3WW, v3WW, v3WW, v2WW, v1NN, v3EE, v3EE,
v3EE, v2EE, v1NE, v1EE, v3WW, v3WW, v3WW, v3WW, v3WW, v1NN, v3EE, v2EE, v1NN, v3WW, v2WW, v1NN, v3EE, v2EE, v1WW, v1NW, v3WW, v1NN,
v1EE, v1NN, v1WW, v1NN, v3EE, v1NW, v2WW, v1NN, v3EE, v1SS, v3EE, v3EE, v3EE, v1SS, v2EE, v3SS, v2WW, v1SW, v1WW, v2SS, v3EE, v2EE,
v1NN, v3WW, v1WW, v1NE, v3EE, v3NN, v2NN, v2WW, v1NE, v1EE, v3WW, v3WW, v3WW, v2WW, v1NW, v1WW, v3EE, v3EE, v3EE, v2EE, v1NN, v3WW,
v3WW, v3WW, v2WW, v1EE, v1NE, v3EE, v3EE, v1EE, 0x00};

byte SPRITE_POTION_0[50] = { 0x03, v2NN, 0x02, v2NN, v1EE, v2SS, v1SE, v1EE, v1SW, v3EE, v3EE, v3EE, v2EE, v2NW, v1WW, 
							 v1EE, v1SE, v1EE, v1NE, v1EE, v1NW, v1EE, v1NN, v1WW, v1NW, v1WW, v1NW, v3WW, v3WW, v1WW, 
							 v1SW, v1WW, v1EE, 0x03, v2SE, 0x02, v1EE, v1NE, v1EE, 0x03, v2NN, 0x02, v3NN, v2EE, v3WW, 
							 v2WW, v2EE, v2SS, 0x00 };

byte SPRITE_POTION_1[100] = { 0x03, v2NN, 0x02, v2NN, v1EE, v2SS, v1SE, v1EE, v1SW, v3EE, v3EE, v3EE, v2EE, v1NW, v1EE,
								   v1NE, v1EE, v1NW, v1EE, v1NN, v1WW, v1NW, v1WW, v1NW, v3WW, v3WW, v1WW, v1SW, v1WW, v1EE,
								   v1SE, v1EE, 0x03, v2SE, 0x02, v1EE, v1NE, v1EE, 0x03, v3NW, 0x02, v3NN, v2WW, v3EE, v2EE,
								   v2WW, v2SS, 0x00 };

byte SPRITE_BLUE_KEY[25] = { v3NN, v3NN, v2NN,
							  v3EE, v3EE, v3EE, v3EE, v3EE, v3EE,
							  v3SS, v3SS, v3SS,
							  v3WW, v3WW, v3WW, v3WW, v3WW, v3WW,
							  v3EE, 'B', 0x00 };

byte SPRITE_RED_KEY[25] = { v3NN, v3NN, v2NN,
							  v3EE, v3EE, v3EE, v3EE, v3EE, v3EE,
							  v3SS, v3SS, v3SS,
							  v3WW, v3WW, v3WW, v3WW, v3WW, v3WW,
							  v3EE, 'R', 0x00 };

byte SPRITE_FIRE_DOT[2] = { v1NN, 0x00 };
byte SPRITE_FIRE_NN[2] = { v2NN, 0x00 };
byte SPRITE_FIRE_NE[2] = { v2NE, 0x00 };
byte SPRITE_FIRE_EE[2] = { v2EE, 0x00 };
byte SPRITE_FIRE_SE[2] = { v2SE, 0x00 };
byte SPRITE_FIRE_SS[2] = { v2SS, 0x00 };
byte SPRITE_FIRE_SW[2] = { v2SW, 0x00 };
byte SPRITE_FIRE_WW[2] = { v2WW, 0x00 };
byte SPRITE_FIRE_NW[2] = { v2NW, 0x00 };

WayPoints thePath;

/*byte absWayPts[10][2] = { {7,5}, {10,3}, {15,4}, {15,7}, {0,0},
						   {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};*/

typedef struct Vec2D {
	sbyte x;
	sbyte y;
} Vec2D;

//used to advance fire sprites in diff directions
Vec2D fireMovementTable[8] = { { 0,  1}, { 2, 1}, { 2, 0}, { 2, -1},   // NN, NE, EE, SE   clockwise from NN
							   { 0, -1}, {-2,-1}, {-2, 0}, {-2,  1}};  // SS, SW, WW, NW

Vec2D fireShamusMovementTable[8] = { { 0,  FIRE_SPEED}, { FIRE_SPEED * 2, FIRE_SPEED}, { FIRE_SPEED * 2, 0}, { FIRE_SPEED * 2, -FIRE_SPEED},   // NN, NE, EE, SE   clockwise from NN
							         { 0, -FIRE_SPEED}, {-FIRE_SPEED * 2,-FIRE_SPEED}, {-FIRE_SPEED * 2, 0}, {-FIRE_SPEED * 2,  FIRE_SPEED} };  // SS, SW, WW, NW
                                 
//used to define fire sprite start position from lower left refPoint of sprite to avoid self killing by friendly fire
Vec2D fireOffsetTable[8] = { { 8,  9}, { 16, 9}, { 16, 4}, {16, 0},   // NN, NE, EE, SE   clockwise from NN
							 { 8,  0}, { 0,  0}, { 0,  4}, { 0, 8}};  // SS, SW, WW, NW
							 
//same ordering as OBJECT_TYPE enum
byte* SpriteTable[12] = { SPRITE_SHAMUS, SPRITE_GREEN_KEYHOLE, SPRITE_GREEN_KEY, SPRITE_POTION_1, SPRITE_SHADOW_0, SPRITE_MYSTERY_1,
						  SPRITE_ROBO_DROID_0, SPRITE_SPIRAL_DRONE_0, SPRITE_SNAP_JUMPER, 0x00};

byte* NEWSpriteTable[100] = { 0x00 };  //has to be populated by initMemory() at runtime


void printDebug() {

  #if defined(_WIN32)

		std::cout << "     gFrame: " << gFrame      << std::endl;
		std::cout << "      gTick: " << gTick       << std::endl;
		std::cout << "gNoOfDynObj: " << gNoOfDynObj << std::endl;

		for (int i = 0; i < gNoOfDynObj; i++) {

			std::cout << "#" << i << " " << listDynDefender[i].type << " | ";
			std::cout << listDynDefender[i].alive[0]+0 << " ";
			std::cout << listDynDefender[i].alive[1]+0 << " ";
			std::cout << listDynDefender[i].isDirty[0]+0 << " ";
			std::cout << listDynDefender[i].isDirty[1]+0 << " ";
			std::cout << std::endl;
		}

		std::cout << "--- --- --- ---" << std::endl;
		//std::cin.get();
	#endif
}

static void SECTION changeRoom(int arg1, int arg2, void* sq) {
	
	newRoom = arg1;
	roomEntryFrom = (DIRECTION)arg2;
	//playSound;
}

static void SECTION takeKey(int arg1, int arg2, void* sq) {

	if (arg1 == GREEN_KEY) {
		//gGameStats.greenKeyPtr = TRUE; //???
		gGameStats.greenKeyPtr->alive[0] = FALSE;
		gGameStats.greenKeyPtr->alive[1] = FALSE;
		gGameStats.greenKeyPtr->isDirty[0] = TRUE;
		gGameStats.greenKeyPtr->isDirty[1] = TRUE;
		gGameStats.greenKeyTriggerSensorPtr->isArmed = FALSE;
		gGameStats.isDirty = TRUE;
	}

	if (arg1 == 2) { gGameStats.blueKey = TRUE;  gGameStats.isDirty = TRUE;}
	if (arg1 == 3) { gGameStats.redKey = TRUE;   gGameStats.isDirty = TRUE;}

	//play sound
}



static void SECTION tryMystery(int mystNo, int arg2, void* _sq) {

	SensorSquare* sq = (SensorSquare*)_sq;

	if (gGameStats.Mysteries[mystNo] == FALSE) {

		//unarm sensor and undisplay mystery
		sq->isArmed = FALSE;
		sq->obj->alive[0] = FALSE;
		sq->obj->alive[1] = FALSE;
		sq->obj->isDirty[0] = TRUE;
		sq->obj->isDirty[1] = TRUE;

		gGameStats.Mysteries[mystNo] = TRUE;

		//if (rnd() > 50) {
			//gGameStats.noOfLives++;
			//gGameStats.isDirty = TRUE;
		//} else {
			//releaseTheShadow();    //to be implemented
			gGameStats.noOfLives--;
			gGameStats.isDirty = TRUE;
		//}
	}
}

static void useKey(int arg1, int arg2, void* sq) {

	/*if (arg1 == GREEN_KEYHOLE) {
		if (gGameStats.greenKey == TRUE) {
			gGameStats.greenKeyHole = OPEN;
			gGameStats.greenKeyHolePtr->alive[0] = FALSE;
			gGameStats.greenKeyHolePtr->alive[1] = FALSE;
			gGameStats.greenKeyHolePtr->isDirty[0] = TRUE;
			gGameStats.greenKeyHolePtr->isDirty[1] = TRUE;
			gGameStats.greenKeyHoleTriggerSensorPtr->isArmed = FALSE;
			gGameStats.greenKeyHolePassageSensorPtr->isArmed = TRUE;
			gGameStats.isDirty = TRUE;
		}
	}*/
}

void setPath(DynVisObj* obj, WayPoints* path) {

	obj->wayPoints = path;
}

/*void movePlayer(signed char x, signed char y) {

	static byte counter = 0;
	static byte stage = 0;

	//change Shamus SPRITE, left, right, up/down
	shamusPtr->actCmdListNo = 0;
	if ((x == 0) && (y != 0)) {shamusPtr->actCmdListNo = 0+stage;}
	if (x > 0) {shamusPtr->actCmdListNo = 2+stage;}
	if (x < 0) {shamusPtr->actCmdListNo = 4+stage;}
	
	//shamusPtr->isDirty[0] = TRUE;
	//shamusPtr->isDirty[1] = TRUE;
	
	gXPosPlayer += x;
	gYPosPlayer += y;

	counter++;
	if (counter == 3) { //change to increase delay
		stage = !stage;
		counter = 0; 
	}
}*/

void processJoystick() {}

void gameIntro() {

	byte lineTM[10] = { "TM" };
	byte line0[40] = { "  0000" };
	byte line1[10] = { "PLAY" };   					//4		
	byte line2[25] = { "SHAMUS" };			//15    
	byte line3[25] = { "KEYHOLE" };   //23    
	byte line4[20] = { "KEY" };				//10     
	byte line5[20] = { "EXTRA LIFE" };
	byte line6[20] = { "MYSTERY" };
	byte line7[20] = { "SHADOW" };
	byte line8[20] = { "ROBO-DROIDS" };
	byte line9[20] = { "SPIRAL DRONES" };
	byte line10[20] = { "SNAP JUMPER" };

	byte line90[35] = { "(TM) 1982 William Mataga" };  //26
	byte line91[20] = { "(GPL) 2020 smed" };				  //12
	byte lineX[30] = { "                           " };				  //12

	GDP_WAIT();
	GDP_PAGE_REG = 0b11110000;

	GDP_CSIZE(0x11);

	//scroll-in lines
	for (int i = 40; i > 0; i--) {
		CMD(0x00);
		CMDPRINT3(((512 - (26 * 6) + 1) / 2) - 10, 10 - (i / 2), (byte*)&line90);
		CMDPRINT3(((512 - (12 * 6) + 1) / 2) - 10, 0 - (i / 2), (byte*)&line91);
		WAIT_MS(3);

		GDP_SYNC_IT();
		CMD(0x01);
		CMDPRINT3(((512 - (26 * 6) + 1) / 2) - 10, 10 - (i / 2), (byte*)&line90);
		CMDPRINT3(((512 - (12 * 6) + 1) / 2) - 10, 0 - (i / 2), (byte*)&line91);
	}

	CMD(0x00);
	CMDPRINT3(((512 - (26 * 6) + 1) / 2) - 10, 10, (byte*)&line90);
	CMDPRINT3(((512 - (12 * 6) + 1) / 2) - 10, 0, (byte*)&line91);

	WAIT_MS(7 * 10);

	GDP_CSIZE(0x31);
	//CMDPRINT3(0, 247, (byte*)&text_topLine);
	//CMDPRINT3(0, 235, (byte*)&line0);   //score
	//CMDPRINT3(234, 235, (byte*)&text_highScore);

	CMDPRINT3SLOW((512 - (4 * 18) + 1) / 2, 200, (byte*)&line1, 6);

	GDP_CSIZE(0x34);
	CMDPRINT3SLOW((512 - (15 * 18) + 1) / 2, 155, (byte*)&line2, 10);

	GDP_CSIZE(0x11);
	CMDPRINT3SLOW(388, 183, (byte*)&lineTM, 5);
	GDP_CSIZE(0x31);

	CMDPRINT3((512 - (23 * 18) + 1) / 2, 130, (byte*)&line3);

	//CMDPRINT3(((512 - ((10 + 2) * 18) + 1) / 2) - 16, 95 + 15, (byte*)&UFOA);
	//CMDPRINT3(((512 - ((10 + 2) * 18) + 1) / 2) - 8, 80 + 15, (byte*)&monsterA1);
	//CMDPRINT3(((512 - ((10 + 2) * 18) + 1) / 2) - 8, 65 + 15, (byte*)&monsterB1);
	//CMDPRINT3(((512 - ((10 + 2) * 18) + 1) / 2) - 8, 50 + 15, (byte*)&monsterC1);

	WAIT_MS(5 * 10);

	CMDPRINT3SLOW(22 + (512 - ((10 + 2) * 18) + 1) / 2, 95 + 15, (byte*)&line4, 10);
	CMDPRINT3SLOW(22 + (512 - ((10 + 2) * 18) + 1) / 2, 80 + 15, (byte*)&line5, 10);
	CMDPRINT3SLOW(22 + (512 - ((10 + 2) * 18) + 1) / 2, 65 + 15, (byte*)&line6, 10);
	CMDPRINT3SLOW(22 + (512 - ((10 + 2) * 18) + 1) / 2, 50 + 15, (byte*)&line7, 10);

	//waitForJoystickFire();	//will call catchSync()

	//init random generator
	//prev = syncCounter % 100;
	pageFlip_2();
}

SensorSquare* addSensorSquare(OBJECT_TYPE obj, byte _isArmed, word _xmin, word _ymin, word _xmax, word _ymax, void(*func)(int, int, void* sq), byte _arg1, byte _arg2, DynVisObj *_obj) {

	listSensor[gNoOfSensors].type = obj;
	listSensor[gNoOfSensors].isArmed = _isArmed;
	listSensor[gNoOfSensors].xmin = _xmin;
	listSensor[gNoOfSensors].xmax = _xmax;
	listSensor[gNoOfSensors].ymin = _ymin;
	listSensor[gNoOfSensors].ymax = _ymax;

	listSensor[gNoOfSensors].func = func;
	listSensor[gNoOfSensors].arg1 = _arg1;
	listSensor[gNoOfSensors].arg2 = _arg2;
	listSensor[gNoOfSensors].obj = _obj;
	gNoOfSensors++;

	return &listSensor[gNoOfSensors-1];
}

DynVisObj* SECTION addDynObj(OBJECT_TYPE _type, word _xpos, word _ypos, byte* _cmd) {

	//finds first empty slot
	int pos = 0;
	while (pos < MAX_NO_DYN_OBJ) {
		if (listDyn[pos].type == UNDEF) { break; }
		pos++;
	}
	//clean
	memset(&listDyn[pos], 0x00, sizeof(listDyn[0]));

	//update max 
	if (pos > gNoOfDynObj) { gNoOfDynObj = pos;}
	
	//debug
	//if (gNoOfDynObj > MAX_NO_DYN_OBJ) { DebugPrintPos(0, 0, (char*)"ERROR", 0, NONE);};

	//do object settings
	listDyn[pos].type = _type;
	listDyn[pos].xPos = _xpos;
	listDyn[pos].yPos = _ypos;
	listDyn[pos].cmdList[0] = _cmd;

	listDyn[pos].ID = gTick; //for debug

	//listDyn[pos].actCmdListNo = 0;
	//listDyn[pos].isSelfRemoving = FALSE;
	//listDyn[pos].isCyclic = FALSE;

	listDyn[pos].cmdListPage[0] = listDyn[pos].cmdListPage[1] = emptyList;  //(2) and then this not needed
	listDyn[pos].isDirty[0]     = listDyn[pos].isDirty[1]     = TRUE;       //(1) maybe set to FALSE?
	listDyn[pos].alive[0]       = listDyn[pos].alive[1]       = TRUE;

	//for debug, count UNDEFS
	int count = 0;
	for (int i = 0; i < MAX_NO_DYN_OBJ; i++) {
		if (listDyn[i].type == UNDEF) count++;
	}
	gNoOfDynUNDEFObj = count;

	//return pointer to new element for detailed object settings
	return &listDyn[pos];
}

DynVisObj* SECTION addCyclicDynObj(OBJECT_TYPE _type, word _xpos, word _ypos, byte* _cmdArray[]) {

	DynVisObj* ptr = addDynObj(_type, _xpos,  _ypos, _cmdArray[_type]);
	ptr->isCyclic = TRUE;

	//add additional sprites
	for (int i = 0; i < MAX_NO_CYCLIC_SPRITES; i++) {
		ptr->cmdList[i] = _cmdArray[(_type*MAX_NO_CYCLIC_SPRITES)+i];  
	}
	
	return ptr;
}

void SECTION addBullet(OBJECT_TYPE _type, word _xpos, word _ypos, byte* _cmd, OBJECT_TYPE _whoShot) {

	if (gFrameLastBulletPlayer <= gTick-gFrameBulletDelay) {
		DynVisObj* ptr = addDynObj(_type, _xpos, _ypos, _cmd);
		//std::cout << "+:" << ptr->ID << std::endl;
		if (_whoShot == SHAMUS) {
			gFrameLastBulletPlayer = gTick;
		} else {
			gFrameLastBulletMonster = gTick;
		}
		gNoOfBullets++;
	} /*else {
		std::cout << "X:" << gFrameLastBulletPlayer << " " << std::endl;
	}*/
}

void drawWallSegment(byte x, byte y, byte no, OBJECT_TYPE type) {

	static int h = 19;
	static int w = 23;
	static int o = 50;
	
	int xPos = (x-1) * w;
	int yPos = o + (y * h);

	for (int i = 0; i < no; i++) {
		if (type == WALL_H) {
			CMDPRINT3(xPos + (i * w), yPos, SPRITE_WALL_H);
		} else {
			CMDPRINT3(xPos, yPos + (i * h), SPRITE_WALL_V);
		}
	}
}

void r(byte isArmed, int xmin, int ymin, int xmax, int ymax) {

	int h = 19;
	int w = 23;
	int o = 50;

	addSensorSquare(HOT_SQUARE, isArmed, (xmin-1)*w, o+(ymin)*h, (xmax)*w, o+(ymax+1)*h, NULL, 0, 0, NULL);
}

/*void SECTION initObj() {

	int size = sizeof(listDyn[0]);  // 72 byte

	//clean and prep dyn list
	for (int i = 0; i < MAX_NO_DYN_OBJ; i++) {

		listDyn[i].type = UNDEF;
		listDyn[i].score = 0;
		listDyn[i].isDirty[0] = FALSE;
		listDyn[i].isDirty[1] = FALSE;
		listDyn[i].alive[0] = FALSE;
		listDyn[i].alive[1] = FALSE;
		listDyn[i].xPos = 0;
		listDyn[i].yPos = 0;
		listDyn[i].xPosPage[0] = 0;
		listDyn[i].yPosPage[0] = 0;
		listDyn[i].xPosPage[1] = 0;
		listDyn[i].yPosPage[1] = 0;

		listDyn[i].onWay = FALSE;
		listDyn[i].wayPoints = NULL;

		listDyn[i].displayGroup = 0;

		listDyn[i].actCmdListNo = 0;
		listDyn[i].isCyclic = FALSE;
		listDyn[i].cyclicCounter = 0;
		listDyn[i].cyclicCounterThreshold = 0;
		listDyn[i].isSelfRemoving = FALSE;
		for (int i = 0; i < MAX_NO_CYCLIC_SPRITES; i++) {
			listDyn[i].cmdList[i] = (byte*)(&emptyList);
		}
		listDyn[i].cmdListPage[0] = (byte*)(&emptyList);
		listDyn[i].cmdListPage[1] = (byte*)(&emptyList);
	}
	gNoOfDynObj = 0;

	//memset(&listSensor, 0, sizeof(listSensor));  //not tested
	for (int i = 0; i < MAX_NO_SENSORS; i++) {
		listSensor[i].xmin = 0;
		listSensor[i].xmax = 0;
		listSensor[i].ymin = 0;
		listSensor[i].ymax = 0;
		listSensor[i].obj = NULL;
	}
	gNoOfSensors = 0;

	//clean
	gXPosPlayer = 0;
	gYPosPlayer = 0;
}*/

int idx2scrX(int tx) {
	//int h = 23 / 1.2;
	int w = 23;
	//int o = 50;
	int val = ((tx - 1) * w); // -(w / 2);

	return val;
	//ty = o + (((10 - ty) * h) + (h / 2));
}

int idx2scrY(int ty) {
	int h = 23 / 1.2;
	//int w = 23;
	int o = 50;
	int val = o + (((10 - ty) * h)); // +(h / 2));

	//tx = ((tx - 1) * w) - (w / 2);
	return val;
}

void initPath() {

	thePath.noOfWayPts = 7;
	thePath.nextWaypoint = 0;
	thePath.pts[0][0] = idx2scrX(7);   thePath.pts[0][1] = idx2scrY(5);
	thePath.pts[1][0] = idx2scrX(9);   thePath.pts[1][1] = idx2scrY(3);
	thePath.pts[2][0] = idx2scrX(14);  thePath.pts[2][1] = idx2scrY(3);
	thePath.pts[3][0] = idx2scrX(15);  thePath.pts[3][1] = idx2scrY(4);
	thePath.pts[4][0] = idx2scrX(15);  thePath.pts[4][1] = idx2scrY(7);
	thePath.pts[5][0] = idx2scrX(14);  thePath.pts[5][1] = idx2scrY(8);
	thePath.pts[6][0] = idx2scrX(8);   thePath.pts[6][1] = idx2scrY(8);
	thePath.pts[7][0] = idx2scrX(7);   thePath.pts[7][1] = idx2scrY(9);

	setPath(&listDyn[4], &thePath);
	listDyn[4].onWay = TRUE;
}

#include "C:\Users\USER\Documents\GitHub\olcPixelGameEngine\NKCShamusRooms.h"

byte* rooms[32] = { 0x00, 0x00, 0x00, 0x00 };  //NKC: has to be filled at run-time !

void SECTION spawnMonster(int x, int y, int noOfMonster) {
	
	int h = 23 / 1.2;
	int w = 23;
	int o = 50;
	byte speed = 0;

	DynVisObj* ptr;
	DynVisObj tmp;
	tmp.xPos = (x-1) * w;
	tmp.yPos = o + ((10-y) * h);

	//if (checkSensors(&tmp) > 0) {
		for (int i = 0; i < noOfMonster; i++) {

			OBJECT_TYPE type;
			int rnd = getRandom();

			type = SNAP_JUMPER;
			if (rnd > 33) { type = ROBO_DROID;   speed = 4; };
			if (rnd > 66) { type = SPIRAL_DRONE; speed = 2; };

//type = ROBO_DROID;

			if ((type == ROBO_DROID) || (type == SPIRAL_DRONE)) {
				ptr = addCyclicDynObj(type, tmp.xPos, tmp.yPos, NEWSpriteTable);
				ptr->cyclicCounterThreshold = speed;
			} else {
				      addDynObj(type, tmp.xPos, tmp.yPos,    SpriteTable[type]);
			}
		}
	//}
}

/*void SECTION drawRoom(byte *a, DIRECTION fromDir) {

	int i = 0;
	while (a[i] != 0x00) {
		byte x = a[i++]; 
		byte y = a[i++]; 
		byte l = a[i++];
		drawWallSegment(x, y, l, WALL_H);
	}
	i++;

	while (a[i] != 0x00) {
		byte x = a[i++]; 
		byte y = a[i++]; 
		byte l = a[i++];
		drawWallSegment(x, y, l, WALL_V);
	}
	i++;

//NKC: up to here
//DebugPrintPos(250, 100, (char *)"HELLO1", 0, NONE);
//DebugPrintPos(250, 120, (char *)"0x", (int)a, HEX);


	while (a[i] != 0x00) {
		byte xmin = a[i++]; 
		byte ymin = 10 - a[i++]; 
		byte xmax = a[i++]; 
		byte ymax = 10 - a[i++];
		byte isArmed = a[i++];
		r(isArmed, xmin, ymin, xmax, ymax);
	}
	i++;

	static int onlyOnce = 0;

	while (a[i] != 0x00) {
		byte x = a[i++];
		byte y = a[i++];
		byte n = a[i++];
		spawnMonster((int)x, (int)y, (int)n);
	}
	i++;
	
	//sensors 
	while (a[i] != 0x00) {

		OBJECT_TYPE obj = (OBJECT_TYPE)a[i++];
		
		void(*func)(int, int, void*) = NULL;

		byte xminMSB = a[i++];
		byte xminLSB = a[i++];
		word xmin = (xminMSB << 8) + xminLSB;

		byte xmaxMSB = a[i++];
		byte xmaxLSB = a[i++];
		word xmax = (xmaxMSB << 8) + xmaxLSB;

		byte ymin = a[i++];
		byte ymax = a[i++];

		byte arg1 = a[i++]; //KEY/KEYHOLE color
		byte arg2 = a[i++]; //no of sensor square

		switch (obj) {
		
			case DOOR:
				func = changeRoom;
				addSensorSquare(obj, TRUE, xmin, ymin, xmax, ymax, func, arg1, arg2, NULL);
				break;
		
			case KEY:
				func = takeKey;  
				if (arg1 == GREEN_KEY) {
					if (gGameStats.greenKey == FALSE) { //check if key already taken
						gGameStats.greenKeyPtr = addDynObj(obj, xmin, ymin, SpriteTable[obj]);
						gGameStats.greenKeyTriggerSensorPtr = addSensorSquare(obj, TRUE, xmin, ymin, xmax, ymax, func, arg1, 0, gGameStats.greenKeyPtr);
						gGameStats.greenKeyHolePassageSensorPtr = &listSensor[arg2];
					}
				}
				break;

			case POTION:
				func = drinkPotion;
				if (gGameStats.Potions[arg1] == FALSE) {   //check if potion already taken
					DynVisObj* ptr = addCyclicDynObj(obj, xmin, ymin, NEWSpriteTable);
					ptr->cyclicCounterThreshold = 16;
					addSensorSquare(obj, TRUE, xmin, ymin, xmax, ymax, func, arg1, NULL, ptr);
				}
				break;

			case MYSTERY:
				func = tryMystery;
				if (gGameStats.Mysteries[arg1] == FALSE) {   //check if mystery as been already tried
					DynVisObj* ptr = addCyclicDynObj(obj, xmin, ymin, NEWSpriteTable);
					ptr->cyclicCounterThreshold = 7;
					addSensorSquare(obj, TRUE, xmin, ymin, xmax, ymax, func, arg1, NULL, ptr);
				}
				break;

			case KEYHOLE:
				func = useKey;
				if (arg1 == GREEN_KEYHOLE) {
					
					if (gGameStats.greenKeyHole == CLOSED) { //check if key already taken
						gGameStats.greenKeyHolePtr = addDynObj(obj, xmin, ymin, SpriteTable[obj]);
						gGameStats.greenKeyHoleTriggerSensorPtr = addSensorSquare(obj, TRUE, xmin, ymin, xmax, ymax, func, arg1, 0, gGameStats.greenKeyHolePtr); //sensor square for conditional passage
						gGameStats.greenKeyHolePassageSensorPtr = &listSensor[arg2];
						gGameStats.greenKeyHoleWallSensorPtr = &listSensor[arg2 + 1];
					} else {
						gGameStats.greenKeyHoleWallSensorPtr = &listSensor[arg2 + 1];
						func(arg1, arg2, gGameStats.greenKeyHoleWallSensorPtr);  //use key to open
					}
				}
				break;

			default:
				DebugPrintPos(0, 0, (char *)"912: ERROR", 0, NONE);
				break;
		}
	}
	i++;

	//place Shamus
	while (a[i] != 0x00) {

		DIRECTION dir = (DIRECTION)a[i++];

		byte xMSB = a[i++];
		byte xLSB = a[i++];
		if (xMSB == 0xFF) { xMSB = 0; }  
		word x = (xMSB << 8) + xLSB;

		byte y = a[i++];

		if (dir == gRoomEntryFrom) {
			gXPosPlayer = x;
			gYPosPlayer = y;
			shamusPtr = addDynObj(SHAMUS, gXPosPlayer, gYPosPlayer, SPRITE_SHAMUS_UPDOWN_1);
			shamusPtr->cmdList[1] = SPRITE_SHAMUS_UPDOWN_2;
			shamusPtr->cmdList[2] = SPRITE_SHAMUS_RIGHT_1;	//place additional SPRITE set
			shamusPtr->cmdList[3] = SPRITE_SHAMUS_RIGHT_2;  //for movements in diff directions
			shamusPtr->cmdList[4] = SPRITE_SHAMUS_LEFT_1;	//see movePlayer()
			shamusPtr->cmdList[5] = SPRITE_SHAMUS_LEFT_2;
			break;
		}
	}
	
}

void drawSensorSquare() {

	for (int i = 0; i < gNoOfSensors; i++) {
		
		//if (listSensor[i].type != DOOR) {
			
			//FillRect(listSensor[i].xmin, listSensor[i].ymin - (listSensor[i].ymax - listSensor[i].ymin), listSensor[i].xmax - listSensor[i].xmin+1, (listSensor[i].ymax - listSensor[i].ymin), olc::MAGENTA);
		//}
		//else {
			GDP_MOVETO(listSensor[i].xmin, listSensor[i].ymin);
			#if defined(_WIN32) 
				col = olc::BLUE; 
			#endif
			GDP_VLINE(listSensor[i].ymax - listSensor[i].ymin);
			#if defined(_WIN32) 
				col = olc::RED;
			#endif	
			GDP_HLINE(listSensor[i].xmax - listSensor[i].xmin);
			GDP_MOVETO(listSensor[i].xmin, listSensor[i].ymin);
			#if defined(_WIN32) 
				col = olc::YELLOW;
			#endif
			GDP_HLINE(listSensor[i].xmax - listSensor[i].xmin);
			#if defined(_WIN32) 
				col = olc::GREEN;
			#endif
			GDP_VLINE(listSensor[i].ymax - listSensor[i].ymin);
		//}
			#if defined(_WIN32) 
			col = olc::WHITE;
			#endif
	}
}*/

void updateScore(int amount) {

	gGameStats.score += amount;

	//int newHighScore = FALSE;
	if (gGameStats.highScore < gGameStats.score) {
		//newHighScore = TRUE;
		gGameStats.highScore = gGameStats.score;
	}

	for (int i = 0; i < 2; i++) {

		pageFlip_2();
		DebugPrintPos(320, 38, (char*)"     ", 0, NONE);
		DebugPrintPos(320, 38, (char*)"", gGameStats.score, DEC);

		//if (newHighScore) {
			DebugPrintPos(320, 30, (char*)"     ", 0, NONE);
			DebugPrintPos(320, 30, (char*)"", gGameStats.highScore, DEC);
		//}
	}
}

/*void drawStaticText() {

	GDP_CSIZE(0x21);
	text2_botLine[2] = 0x30 + (gRoomNumber / 10);
	text2_botLine[3] = 0x30 + (gRoomNumber % 10);

	text2_botLine[9] = 0x30 + (gGameStats.level);
	
	for (int i = 0; i < 2; i++) {

		pageFlip_2();
		CMDPRINT3(150, 38, text1_botLine); 
		CMDPRINT3(150, 30, text2_botLine); 

		//draw noOfLives at top, two rows!
		for (int p = 0; p < gGameStats.noOfLives; p++) {
			CMDPRINT3(440 + (p * 25), 254-10, SPRITE_SHAMUS_RIGHT_1);
		}
		for (int p = 3; p < gGameStats.noOfLives; p++) {
			CMDPRINT3(440 + ((p-3) * 25), 254-25, SPRITE_SHAMUS_RIGHT_1);
		}

		//draw keys
		if (gGameStats.greenKey == TRUE) { CMDPRINT3(440 + (0 * 25), 254 - 40, SPRITE_GREEN_KEY); }
		if (gGameStats.blueKey == TRUE) { CMDPRINT3(440 + (1 * 25), 254 - 40, SPRITE_BLUE_KEY); }
		if (gGameStats.redKey == TRUE) { CMDPRINT3(440 + (2 * 25), 254 - 40, SPRITE_RED_KEY); }

		//draw keyholes
		

	}

	GDP_CSIZE(0x21);
}*/

/*void SECTION initMemory() {

#if defined (NKC)
	//clear BSS section
	int* text_sta = &_text_sta;
	int* bss_start = &_bss_sta;
	int* bss_end = &_bss_end;

	int size = bss_end - bss_start;
	memset(bss_start, 0x00, size);

#endif

	//NKC: need to assign pointer tables at run-time !
	rooms[0] = 0x00;	
	rooms[1] = room1;
	rooms[2] = room2;
	rooms[3] = room3;
					  
	NEWSpriteTable[ROBO_DROID * MAX_NO_CYCLIC_SPRITES + 0] = SPRITE_ROBO_DROID_0;
	NEWSpriteTable[ROBO_DROID * MAX_NO_CYCLIC_SPRITES + 1] = SPRITE_ROBO_DROID_1;
	NEWSpriteTable[ROBO_DROID * MAX_NO_CYCLIC_SPRITES + 2] = SPRITE_ROBO_DROID_2;
	NEWSpriteTable[ROBO_DROID * MAX_NO_CYCLIC_SPRITES + 3] = SPRITE_ROBO_DROID_3;
	
	NEWSpriteTable[SPIRAL_DRONE * MAX_NO_CYCLIC_SPRITES + 0] = SPRITE_SPIRAL_DRONE_0;
	NEWSpriteTable[SPIRAL_DRONE * MAX_NO_CYCLIC_SPRITES + 1] = SPRITE_SPIRAL_DRONE_1;
	NEWSpriteTable[SPIRAL_DRONE * MAX_NO_CYCLIC_SPRITES + 2] = SPRITE_SPIRAL_DRONE_2;
	NEWSpriteTable[SPIRAL_DRONE * MAX_NO_CYCLIC_SPRITES + 3] = SPRITE_SPIRAL_DRONE_3;
	
	NEWSpriteTable[MYSTERY * MAX_NO_CYCLIC_SPRITES + 0] = SPRITE_MYSTERY_0;
	NEWSpriteTable[MYSTERY * MAX_NO_CYCLIC_SPRITES + 1] = SPRITE_MYSTERY_1;
	NEWSpriteTable[MYSTERY * MAX_NO_CYCLIC_SPRITES + 2] = SPRITE_MYSTERY_0;
	NEWSpriteTable[MYSTERY * MAX_NO_CYCLIC_SPRITES + 3] = SPRITE_MYSTERY_1;
	
	NEWSpriteTable[POTION * MAX_NO_CYCLIC_SPRITES + 0] = SPRITE_POTION_0;
	NEWSpriteTable[POTION * MAX_NO_CYCLIC_SPRITES + 1] = SPRITE_POTION_1;
	NEWSpriteTable[POTION * MAX_NO_CYCLIC_SPRITES + 2] = SPRITE_POTION_0;
	NEWSpriteTable[POTION * MAX_NO_CYCLIC_SPRITES + 3] = SPRITE_POTION_1;

	NEWSpriteTable[SHADOW * MAX_NO_CYCLIC_SPRITES + 0] = SPRITE_SHADOW_0;
	NEWSpriteTable[SHADOW * MAX_NO_CYCLIC_SPRITES + 1] = SPRITE_SHADOW_1;
	NEWSpriteTable[SHADOW * MAX_NO_CYCLIC_SPRITES + 2] = SPRITE_SHADOW_0;
	NEWSpriteTable[SHADOW * MAX_NO_CYCLIC_SPRITES + 3] = SPRITE_SHADOW_1;

	NEWSpriteTable[PERISH * MAX_NO_CYCLIC_SPRITES + 0] = SPRITE_PERISH_0;
	NEWSpriteTable[PERISH * MAX_NO_CYCLIC_SPRITES + 1] = SPRITE_PERISH_1;
	NEWSpriteTable[PERISH * MAX_NO_CYCLIC_SPRITES + 2] = SPRITE_PERISH_2;
	NEWSpriteTable[PERISH * MAX_NO_CYCLIC_SPRITES + 3] = SPRITE_PERISH_3;
}*/

//mid of file

/*void prep() {

	//init_interrupt();
	//interrupt_OFF();
	initMemory();

	initGraphics();
	pageFlip_2();	//should be pageFlip_4 for Shamus

	//initSound();

	consoleArgs.x = 0; 
	consoleArgs.y = 254 - 6; 
	consoleArgs.fontSize = 0;

	gGameStats.noOfLives = 4;
	gGameStats.level = 1;
	gGameStats.score = 0;
	gGameStats.highScore = 0;

	gGameStats.monsterShootProb = 100;	//debug

	gGameStats.greenKey = FALSE;
	gGameStats.blueKey = FALSE;
	gGameStats.redKey = FALSE;

	gGameStats.greenKeyHole = CLOSED;
	gGameStats.blueKeyHole = CLOSED;
	gGameStats.redKeyHole = CLOSED;

	gLoadNewRoom = TRUE;
	gRoomNumber = 1;
	gRoomEntryFrom = WW;
	
	newRoom = gRoomNumber;
	roomEntryFrom = gRoomEntryFrom;

	//zero init variables;	
	gTick = 0;
	gFrame = 1;   //must starst at 1, for sprite change all 5 frames with %

	initObj();
	initPath();

	GDP_CLS(); pageFlip_2();
	GDP_CLS(); pageFlip_2();
	drawStaticText(); pageFlip_2();
	drawStaticText(); pageFlip_2();

	GDP_SYNC_POS();  //sync game loop
}*/


/*void SECTION processKeyboard() {

	byte theKey;
	byte withFire = FALSE;

#if defined(NKC)
	theKey = readKey();

	if (theKey == 0) {
		theKey = readKey();
		theKey = theKey - 128;
#else
		{
			theKey = 0x00;
			withFire = FALSE;

			if (gamepadXAxis > 0.0f) { theKey = 'd'; }
			if (gamepadXAxis < 0.0f) { theKey = 's'; }
			if (gamepadYAxis > 0.0f) { theKey = 'x'; }
			if (gamepadYAxis < 0.0f) { theKey = 'e'; }

			if ((gamepadXAxis > 0.0f) && (gamepadYAxis > 0.0f)) { theKey = 'c'; }
			if ((gamepadXAxis > 0.0f) && (gamepadYAxis < 0.0f)) { theKey = 'r'; }
			if ((gamepadXAxis < 0.0f) && (gamepadYAxis < 0.0f)) { theKey = 'w'; }
			if ((gamepadXAxis < 0.0f) && (gamepadYAxis > 0.0f)) { theKey = 'z'; }

			if (gamepadFire) {withFire = TRUE; }
			
			if (GetKey(olc::Key::ESCAPE).bHeld) { theKey = '#'; }

			if (GetKey(olc::Key::S).bHeld) { theKey = 's'; }
			if (GetKey(olc::Key::D).bHeld) { theKey = 'd'; }
			if (GetKey(olc::Key::E).bHeld) { theKey = 'e'; }
			if (GetKey(olc::Key::X).bHeld) { theKey = 'x'; }

			if (GetKey(olc::Key::LEFT).bHeld)  { theKey = 's'; }
			if (GetKey(olc::Key::RIGHT).bHeld) { theKey = 'd'; }
			if (GetKey(olc::Key::UP).bHeld)    { theKey = 'e'; }
			if (GetKey(olc::Key::DOWN).bHeld)  { theKey = 'x'; }

			//diagonals
			if (GetKey(olc::Key::R).bHeld) { theKey = 'r'; }
			if (GetKey(olc::Key::C).bHeld) { theKey = 'c'; }
		
			if (GetKey(olc::Key::W).bHeld) { theKey = 'w'; }

			if (GetKey(olc::Key::K).bHeld) { theKey = 'k'; }
			if (GetKey(olc::Key::L).bHeld) { theKey = 'l'; }

			if (GetKey(olc::Key::T).bHeld) { theKey = 't'; }	//show hot squares
			if (GetKey(olc::Key::M).bHeld) { theKey = 'm'; }
			if (GetKey(olc::Key::B).bHeld) { theKey = 'b'; }

			if (GetKey(olc::Key::SPACE).bPressed) { withFire = TRUE; }  //avoid Dauerfeuer

			if (GetMouse(0).bPressed) {
				gXPosPlayer = PixelGameEngine::GetMouseX();
				gYPosPlayer = 256 - PixelGameEngine::GetMouseY();
			}

			//DebugPrintPos(400, 0, (char*)"                ", 0, NONE);
			//DebugPrintPos(400,            0, (char*)"", gXPosPlayer, DEC);
			//DebugPrintPos(400 + (4 * 10), 0, (char*)"", gYPosPlayer, DEC);

			//DebugPrintPos(360, 0, (char*)"       ", 0, NONE);
			//DebugPrintPos(360, 0, (char*)"", PixelGameEngine::GetJoystickX(), DEC);

			//DebugPrintPos(360, 10, (char*)"       ", 0, NONE);
			//DebugPrintPos(360, 10, (char*)"", PixelGameEngine::GetJoystickY(), DEC);

			int ox = 8;
			int oy = 4;
			if ((theKey == 'e') && withFire) { addBullet(FIRE_SHAMUS_NN, gXPosPlayer + ox-1, gYPosPlayer + oy+4, SPRITE_FIRE_NN, SHAMUS); theKey = 0x00; }
			if ((theKey == 'r') && withFire) { addBullet(FIRE_SHAMUS_NE, gXPosPlayer + ox+3, gYPosPlayer + oy+4, SPRITE_FIRE_NE, SHAMUS); theKey = 0x00; }
			if ((theKey == 'd') && withFire) { addBullet(FIRE_SHAMUS_EE, gXPosPlayer + ox+3, gYPosPlayer + oy,   SPRITE_FIRE_EE, SHAMUS); theKey = 0x00; }
			if ((theKey == 'c') && withFire) { addBullet(FIRE_SHAMUS_SE, gXPosPlayer + ox+3, gYPosPlayer + oy-2, SPRITE_FIRE_SE, SHAMUS); theKey = 0x00; }
			if ((theKey == 'x') && withFire) { addBullet(FIRE_SHAMUS_SS, gXPosPlayer + ox-1, gYPosPlayer + oy-2, SPRITE_FIRE_SS, SHAMUS); theKey = 0x00; }
			if ((theKey == 'z') && withFire) { addBullet(FIRE_SHAMUS_SW, gXPosPlayer + ox-3, gYPosPlayer + oy-2, SPRITE_FIRE_SW, SHAMUS); theKey = 0x00; }
			if ((theKey == 's') && withFire) { addBullet(FIRE_SHAMUS_WW, gXPosPlayer + ox-3, gYPosPlayer + oy,   SPRITE_FIRE_WW, SHAMUS); theKey = 0x00; }
			if ((theKey == 'w') && withFire) { addBullet(FIRE_SHAMUS_NW, gXPosPlayer + ox-3, gYPosPlayer + oy+4, SPRITE_FIRE_NW, SHAMUS); theKey = 0x00; }

#endif
			DynVisObj* obj;

			if (withFire && (gGameState == INTRO)) { gGameState = PLAY; prep();}
			
			switch (theKey) {

			case 's': movePlayer(-SHAMUS_SPEED * 2,  0); break;
			case 'd': movePlayer( SHAMUS_SPEED * 2,  0); break;
			case 'k': movePlayer(-SHAMUS_SPEED * 2,  0); break;
			case 'l': movePlayer( SHAMUS_SPEED * 2,  0); break;
			case 'x': movePlayer(                0, -SHAMUS_SPEED); break;
			case 'e': movePlayer(                0,  SHAMUS_SPEED); break;
			case 'r': movePlayer( SHAMUS_SPEED * 2,  SHAMUS_SPEED); break;
			case 'c': movePlayer( SHAMUS_SPEED * 2, -SHAMUS_SPEED); break;
			case 'z': movePlayer(-SHAMUS_SPEED * 2, -SHAMUS_SPEED); break;
			case 'w': movePlayer(-SHAMUS_SPEED * 2,  SHAMUS_SPEED); break;

			case 't': gDrawHotSquares = !gDrawHotSquares;	break;
			case '#': gGameState = EXIT;					break;   //ESC
			case 'm': printDebug();							break;
			case 'b': theKey = 0x00;						break;

			default:
				movePlayer(0, 0);

			}

			/*if (gXPosPlayer < 0)   gXPosPlayer = 0;
			if (gXPosPlayer > 512) gXPosPlayer = 512;

			if (gYPosPlayer < 0)   gYPosPlayer = 0;
			if (gYPosPlayer > 255) gYPosPlayer = 255;*/
	//	}
//}






/*void drawDynObj() {

	GDP_CSIZE(0x21);

	for (int i = 0; i <= gNoOfDynObj; i++) {

		//catchSync();

		DynVisObj* e = &listDyn[i];

		if (e->type != UNDEF) {
			if (e->alive[NKClib_page] == TRUE) {
				if ((e->isDirty[NKClib_page] == TRUE) || (e->type == KEY) || (e->type == KEYHOLE)) {
					CMDPRINT3(e->xPos, e->yPos, e->cmdList[e->actCmdListNo]);
					e->xPosPage[NKClib_page] = e->xPos;
					e->yPosPage[NKClib_page] = e->yPos;
					e->cmdListPage[NKClib_page] = e->cmdList[e->actCmdListNo];
					e->isDirty[NKClib_page] = FALSE;
				}
			}
		}
	}
}

void clearDynObj() {

	CMD(0x01);
	GDP_CSIZE(0x41);

	for (int i = 0; i <= gNoOfDynObj; i++) {

		DynVisObj* e = &listDyn[i];

		if (e->type != UNDEF) {
			if (e->isDirty[NKClib_page] == TRUE) {
				//GDP_BLANK(e->xPosPage[NKClib_page], e->yPosPage[NKClib_page]);
				CMDPRINT3(e->xPosPage[NKClib_page], e->yPosPage[NKClib_page], e->cmdListPage[NKClib_page]);
				if ((e->alive[NKClib_page] == FALSE) && (e->alive[!NKClib_page] == FALSE)) {
					if (e->type > FIRE) { 
						//std::cout << "-:" << e->ID << std::endl;
						gNoOfBullets--; 
					}
					memset(&listDyn[i], 0x00, sizeof(listDyn[0]));
				}
				if (e->alive[NKClib_page] == FALSE) e->alive[!NKClib_page] = FALSE;
			}
		}
	}
	CMD(0x00);
}*/

int isInside(DynVisObj* obj, SensorSquare* sen) {  //used by checkSensors();

	int result = FALSE;

	if (obj->xPos >= sen->xmin) {
		if (obj->xPos+14 <= sen->xmax) {
			if (obj->yPos >= sen->ymin) {
				if (obj->yPos + 8 <= sen->ymax) {
					result = TRUE;
				//} else {
					//return obj->yPos + 8 <= sen->ymax;
				}
			//} else {
				//return obj->yPos - 1 >= sen->ymin;
			}
		//} else {
			//return obj->xPos + 18 <= sen->xmax;
		}
	//} else {
		//return obj->xPos >= sen->xmin;
	}
	
	return result;
}

int checkSensors(DynVisObj* obj) {

	int result = 0;
	
	for (int i = 0; i < gNoOfSensors; i++) {
		if (listSensor[i].isArmed == TRUE) {
			if (isInside(obj, &listSensor[i])) {
				result = i+1;
			}
		}
	}

	return result; //returns the square+1 obj is in
}

/*void SECTION doMovements(int dg) {

	int rnd = getRandom();
	
	if (gGameState == MOURN) goto END_DO_MOVEMENTS;  

	for (int i = 0; i <= gNoOfDynObj; i++) {

			DynVisObj* e = &listDyn[i];

			int oldX = e->xPos;
			int oldY = e->yPos;

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

			//advance fire sprites in any direction
			if ((e->type > FIRE) && (e->type != UNDEF)) {  

				if (e->type > FIRE_SHAMUS) {
					e->xPos += fireShamusMovementTable[e->type - 61].x;
					e->yPos += fireShamusMovementTable[e->type - 61].y;
				} else {
					e->xPos += fireMovementTable[e->type - 51].x;
					e->yPos += fireMovementTable[e->type - 51].y;
				}

				//check if hit wall   
				if (!checkSensors(e)) {
					e->alive[NKClib_page] = FALSE;  
				}
				e->isDirty[0] = TRUE;
				e->isDirty[1] = TRUE;
			}

			switch (e->type) {

				case SHAMUS:
					e->xPos = gXPosPlayer;
					e->yPos = gYPosPlayer;

					if (!checkSensors(e)) {
						gShamusExpires = TRUE;	//Shamus touched electric wall
					} 
					e->isDirty[NKClib_page] = TRUE;
					break;

				case POTION:
					e->isDirty[NKClib_page] = TRUE;
					break;

				case MYSTERY:
					e->isDirty[NKClib_page] = TRUE;
					break;

				case SHADOW:
					if (gGameState == INTRO) {
						if (e->isCyclic) e->xPos++;  //move Shadow following Shamus in Intro
						if (e->xPos > 511) e->xPos = 0;
					}

					e->isDirty[NKClib_page] = TRUE;
					break;

				case SNAP_JUMPER:
				
					if (getRandom() > 90) {

						int xJump = 10 - (getRandom() / 5);
						int yJump = 10 - (getRandom() / 5);

						e->xPos += xJump;
						e->yPos += yJump;

						if (!checkSensors(e)) {
							e->xPos = oldX;
							e->yPos = oldY;
						}
						e->isDirty[NKClib_page] = TRUE;
					}
					e->isDirty[NKClib_page] = TRUE;  //WIN: has to be dirty, otherwise pixel debris
					break;

				case SPIRAL_DRONE:

					//follow Shamus, but dont get stuck
					if (rnd > 80) {
						if (e->xPos > gXPosPlayer) e->xPos--;
						if (e->xPos < gXPosPlayer) e->xPos++;
						if (e->yPos > gYPosPlayer) e->yPos--;
						if (e->yPos < gYPosPlayer) e->yPos++;
						
						//add some random movement
						if (rnd >= 95) {
							if (rnd == 95)  e->xPos--;
							if (rnd == 96)  e->xPos++;
							if (rnd == 97)  e->yPos--;
							if (rnd == 98)  e->yPos++;
						}
					}

					if (!checkSensors(e)) {
						e->xPos = oldX;
						e->yPos = oldY;
					}
					e->isDirty[NKClib_page] = TRUE;
					break;

				case ROBO_DROID:

					//random path
					if (getRandom() > 50) {                     
						if (getRandom() > 50) { 
							e->xPos += 1; }
						else {
							e->xPos -= 1;
						}
					} else {
						if (getRandom() > 50) {
							e->yPos += 1;
						}
						else {
							e->yPos -= 1;
						}
					}

					int x1, x2, y1, y2;

					if ((e->onWay == TRUE) && (e->wayPoints != NULL)) {

					#if defined(_WIN32)
						//draw full path if first waypoint
						if (e->wayPoints->nextWaypoint == 0) {
								
							//GDP_MOVETO(e->wayPoints->pts[0][0], e->wayPoints->pts[0][1]);
							for (int i = 0; i < e->wayPoints->noOfWayPts-1; i++) {
								//GDP_DRAWTO(e->wayPoints->pts[i][0], e->wayPoints->pts[i][1]);
								x1 = e->wayPoints->pts[i][0];
								y1 = e->wayPoints->pts[i][1];
									
								x2 = e->wayPoints->pts[i+1][0];
								y2 = e->wayPoints->pts[i+1][1];

								DrawLine(x1, 256-y1, x2, 256-y2);
							}
						}
					#endif
					
					int tx = e->wayPoints->pts[e->wayPoints->nextWaypoint][0];
					int ty = e->wayPoints->pts[e->wayPoints->nextWaypoint][1];

					//check if current waypoint is reached
					if ((e->xPos == tx) && (e->yPos == ty)) {
						//more waypoints to reach?
						if (e->wayPoints->nextWaypoint < e->wayPoints->noOfWayPts - 1) {
							e->wayPoints->nextWaypoint++;
							tx = e->wayPoints->pts[e->wayPoints->nextWaypoint][0];
							ty = e->wayPoints->pts[e->wayPoints->nextWaypoint][1];
						} else {
							e->onWay = FALSE;
						}
					} else {
						//simply proceed to waypoint
						if (e->xPos > tx) e->xPos--; 
						if (e->xPos < tx) e->xPos++; 
						if (e->yPos > ty) e->yPos--;
						if (e->yPos < ty) e->yPos++;
					}
				}

				if (!checkSensors(e)) {  
					e->xPos = oldX;
					e->yPos = oldY;
				}

				e->isDirty[0] = TRUE;
				e->isDirty[1] = TRUE;
				break;
			}
	}
	END_DO_MOVEMENTS:;
}*/

/*double bearing(double a1, double a2, double b1, double b2) {
	
	static const double TWOPI = 6.2831853071795865;
	static const double RAD2DEG = 57.2957795130823209;
	// if (a1 = b1 and a2 = b2) throw an error 
	double theta = atan2(b1 - a1, a2 - b2);
	if (theta < 0.0)
		theta += TWOPI;
	return RAD2DEG * theta;
}*/

void doAI() {

	for (int i = 0; i <= gNoOfDynObj; i++) {

		if (getRandom() > gGameStats.monsterShootProb) {
			
			DynVisObj* e = &listDyn[i];

			int quadrantToShamus = 0;

			if ((e->type == ROBO_DROID) || (e->type == SPIRAL_DRONE)) {

				int LUTval = 0;
				int xdiff = (gXPosPlayer - e->xPos) >> 4;
				int ydiff = (gYPosPlayer - e->yPos) >> 4;

				//check if outside range
				if ((NKClib_abs(xdiff) <= 9) && (NKClib_abs(ydiff) <= 4)) {

					if (xdiff < 0) {
						if (ydiff < 0) {
							LUTval = slope2dirLUT_QSW[-ydiff][-xdiff];  //SW
						}
						else {
							LUTval = slope2dirLUT_QNW[ydiff][-xdiff];   //NW
						}
					}
					else {
						if (ydiff < 0) {
							LUTval = slope2dirLUT_QSE[-ydiff][xdiff];  //SE
						}
						else {
							LUTval = slope2dirLUT_QNE[ydiff][xdiff];   //NE
						}
					}

					/*DebugPrintPos(50, 0, (char*)"        ", 0, NONE);
					DebugPrintPos(50, 0, (char*)"xr=", xdiff, DEC);

					DebugPrintPos(100, 0, (char*)"        ", 0, NONE);
					DebugPrintPos(100, 0, (char*)"yr=", ydiff, DEC);

					DebugPrintPos(150, 0, (char*)"        ", 0, NONE);
					DebugPrintPos(150, 0, (char*)"aLUT=", LUTval, DEC);*/

					//0 points down, counterclockwise
					/*double t = 360 - bearing(gXPosPlayer, gYPosPlayer, e->xPos, e->yPos);

					DebugPrintPos(400, 0, (char*)"        ", t, DEC);
					DebugPrintPos(400, 0, (char*)"a=", t, DEC);

					int d = 0;
					if ((t >=   0.0) && (t <  22.5)) { d = NN; }
					if ((t >=  22.5) && (t <  67.5)) { d = NE; }
					if ((t >=  67.5) && (t < 112.5)) { d = EE; }
					if ((t >= 112.5) && (t < 157.5)) { d = SE; }
					if ((t >= 157.5) && (t < 202.5)) { d = SS; }
					if ((t >= 202.5) && (t < 247.5)) { d = SW; }
					if ((t >= 247.5) && (t < 292.5)) { d = WW; }
					if ((t >= 292.5) && (t < 337.5)) { d = NW; }
					if ((t >= 337.5) && (t < 361)) { d = NN; } */

					int x = fireOffsetTable[LUTval].x;
					int y = fireOffsetTable[LUTval].y;

					addDynObj((OBJECT_TYPE)(FIRE_NN + LUTval), e->xPos + x, e->yPos + y, (byte*)(&SPRITE_FIRE_NN + LUTval));
				}	
			}
		}
	}
}

int pointInSquare(int x, int y, SensorSquare* sen) {

	int result = FALSE;

	if ((x >= sen->xmin) && (x <= sen->xmax)) {
		if ((y >= sen->ymin) && (y <= sen->ymax)) {
			result = TRUE;
		}
	}

	return result;
}

int pointInDynOby(int x, int y, DynVisObj* o) {

	int result = FALSE;

	int xsize = 8*2;
	int ysize = 8;

	if ((x >= o->xPos) && (x <= o->xPos + xsize)) {
		if ((y >= o->yPos) && (y <= o->yPos + ysize)) {
			result = TRUE;
		}
	}
	return result;
}

int isTouching(DynVisObj* obj, SensorSquare* sen) {

	//check if any of the obj corners is inside the sensor square
	//assumption: object is smaller than SensorSquare

	int result = FALSE;

	int xO = obj->xPos;
	int yO = obj->yPos;

	if (pointInSquare(xO, yO, sen) ||
		pointInSquare(xO+18, yO+8, sen) ||
		pointInSquare(xO+18, yO, sen) ||
		pointInSquare(xO, yO+8, sen)) {

		result = TRUE;
	}

	return result;
}

/*void doCollisions() {

	for (int i = 0; i <= gNoOfDynObj; i++) {

		DynVisObj* e = &listDyn[i];

		if (e->type == SHAMUS) {
			
			//Shamus vs sensor
			for (int p = 0; p <= gNoOfSensors; p++) {

				SensorSquare* s = &listSensor[p];

				if ((s->type == DOOR) || (s->type == KEY) || (s->type == KEYHOLE) || (s->type == POTION) || (s->type == MYSTERY)) {
					if (s->isArmed == TRUE) {
						if (isTouching(e, s) == TRUE) {
							s->func(s->arg1, s->arg2, s);  //trigger func
						}
					}
				}
			}

			//Shamus vs monster
			for (int p = 0; p <= gNoOfDynObj; p++) {

				DynVisObj* o = &listDyn[p];

				if ((o->type == SNAP_JUMPER) || (o->type == ROBO_DROID) || (o->type == SPIRAL_DRONE) || (o->type > FIRE)) {
				
					if (pointInDynOby(o->xPos, o->yPos, e)) {
					//if (isTouching(e, o) == TRUE) {
						gShamusExpires = TRUE;
					}
				}
			}
		}

		//monster vs fire
		if ((e->type == SNAP_JUMPER) || (e->type == ROBO_DROID) || (e->type == SPIRAL_DRONE)) {
			
			for (int p = 0; p <= gNoOfDynObj; p++) {

				DynVisObj* o = &listDyn[p];

				if (o->type > FIRE_SHAMUS) {
					if (pointInDynOby(o->xPos, o->yPos, e)) {   
						
						o->alive[NKClib_page] = FALSE;	//Fire
						e->alive[NKClib_page] = FALSE;  //Monster
						
						//perish graphics
						DynVisObj* ptr = addCyclicDynObj(PERISH, e->xPos, e->yPos, NEWSpriteTable);
						ptr->cyclicCounterThreshold = 2;
						ptr->isSelfRemoving = (4 * ptr->cyclicCounterThreshold);

						updateScore(25);

						//gGameStats.isDirty = TRUE;
					}
				}
			}
		}
	}
}*/

/*void updateWallSegments() {

	if (gGameStats.greenKeyHole == OPEN) {
		SensorSquare *sq = gGameStats.greenKeyHoleWallSensorPtr;
		for (int x = sq->xmin; x < sq->xmax; x++) {
			CMD(0x01);
			GDP_MOVETO(x, sq->ymin);
			GDP_VLINE(sq->ymax - sq->ymin);
			CMD(0x00);
		}
	}
}*/

/*void prep2() {

	initMemory();
	initObj();
	initGraphics();

	int x = 200;
	int yStep = 12;
	int offset = 75;

	DynVisObj* ptr;

	addDynObj(SNAP_JUMPER,  x, offset + (1 * yStep),   SPRITE_SNAP_JUMPER);
	
	ptr = addCyclicDynObj(SPIRAL_DRONE, x, offset + (2 * yStep), NEWSpriteTable);
	ptr->cyclicCounterThreshold = 4;

	ptr = addCyclicDynObj(ROBO_DROID,   x, offset + (3 * yStep), NEWSpriteTable);
	ptr->cyclicCounterThreshold = 4;
	
	addDynObj(SHADOW,                   x+1, offset + (4 * yStep)-2, SPRITE_SHADOW_0);
	
	ptr = addCyclicDynObj(MYSTERY,      x, offset + (5 * yStep),   NEWSpriteTable);
	ptr->cyclicCounterThreshold = 4;
	
	ptr = addCyclicDynObj(POTION,       x, offset + (6 * yStep),   NEWSpriteTable);
	//ptr->isSelfRemoving = 12;
	ptr->cyclicCounterThreshold = 3;

	addDynObj(KEY,                      x, offset + (7 * yStep),   SPRITE_GREEN_KEY);
	addDynObj(KEYHOLE,                  x, offset + (8 * yStep),   SPRITE_GREEN_KEYHOLE);
	
	offset += 0;
	DebugPrintPos(250, offset + (1 * yStep), (char*)"Snap Jumpers",  0, NONE);
	DebugPrintPos(250, offset + (2 * yStep), (char*)"Spiral Drones", 0, NONE);
	DebugPrintPos(250, offset + (3 * yStep), (char*)"Robo-Droids",   0, NONE);
	DebugPrintPos(250, offset + (4 * yStep), (char*)"Shadow",        0, NONE);
	DebugPrintPos(250, offset + (5 * yStep), (char*)"Mystery",       0, NONE);
	DebugPrintPos(250, offset + (6 * yStep), (char*)"Extra Life",    0, NONE);
	DebugPrintPos(250, offset + (7 * yStep), (char*)"Key",           0, NONE);
	DebugPrintPos(250, offset + (8 * yStep), (char*)"Keyhole",       0, NONE);

	DebugPrintPos(250-60, offset + (11 * yStep), (char*)"S H A M U S", 0, NONE);

	gXPosPlayer = 256/2;
	gYPosPlayer = 128/2;
	
	shamusPtr = addDynObj(SHAMUS, gXPosPlayer, gYPosPlayer, SPRITE_SHAMUS_UPDOWN_1);
	shamusPtr->cmdList[1] = SPRITE_SHAMUS_UPDOWN_2;
	shamusPtr->cmdList[2] = SPRITE_SHAMUS_RIGHT_1;	//place additional SPRITE set
	shamusPtr->cmdList[3] = SPRITE_SHAMUS_RIGHT_2;  //for movements in diff directions
	shamusPtr->cmdList[4] = SPRITE_SHAMUS_LEFT_1;	//see movePlayer()
	shamusPtr->cmdList[5] = SPRITE_SHAMUS_LEFT_2;

	ptr = addCyclicDynObj(SHADOW, gXPosPlayer - 18, gYPosPlayer, NEWSpriteTable);
	ptr->cyclicCounterThreshold = 3;
}*/

/*void iterate2() {

	processKeyboard();
	doMovements(0);
	//doCollisions();
	//doAI();
	clearDynObj();       	
	drawDynObj();        	
	movePlayer(1, 0);
	pageFlip_2();   						
	//gTick++;
	//gFrame++;
	
	

	if (gXPosPlayer > 511) gXPosPlayer = 0;
}*/

/*void iterate() {

	//debug
	DebugPrintPos(380, 0, (char*)"       ", 0, NONE);
	DebugPrintPos(380, 0, (char*)"", gXPosPlayer, DEC);
	DebugPrintPos(380 + (4 * 10), 0, (char*)"", gYPosPlayer, DEC);

	DebugPrintPos(200, 0, (char*)"M:    ", 0, NONE);
	DebugPrintPos(200, 0, (char*)"M:", gNoOfDynObj, DEC);
	DebugPrintPos(240, 0, (char*)"", gNoOfDynUNDEFObj, DEC);

	if (gLoadNewRoom == TRUE) { 
		initObj();
		initGraphics();
		drawStaticText();
		updateScore(0);
		//GDP_PAGE_REG = 0b00000000;
		//drawRoom(room1, gRoomEntryFrom);  //NKC - up to here
		drawRoom(rooms[gRoomNumber], gRoomEntryFrom);
		gLoadNewRoom = FALSE;
	}

	processKeyboard(); 								//asm("move.b 0x00, 0x8007");
	//processJoystick(); 							//asm("move.b 0x00, 0x8008");
	doMovements(gFrame); 							//asm("move.b 0x00, 0x8001");
	doCollisions();     catchSync(); 				//asm("move.b 0x00, 0x8003");
	doAI();											//asm("move.b 0x00, 0x8004");
	clearDynObj();      catchSync(); 				//asm("move.b 0x00, 0x8005");
	drawDynObj();       catchSync(); 				//asm("move.b 0x00, 0x800F");	
	pageFlip_2();   								
	gTick++;
	gFrame++;

	//DebugPrintPos(10, 245, (char*)"     ", 0, NONE);
	//DebugPrintPos(10, 245, (char*)"", gTick, DEC);

	if (gShamusExpires == TRUE) {
		
		static int counter;

		if (counter == 0) {
			//clear space for sprite, szie 12 X 48   
			CMD(PEN_ERASE);
			GDP_CSIZE_REG = 0xC3;   //3x12
			GDP_MOVETO(gXPosPlayer + 12, gYPosPlayer + 10);
			CMD(0x0b);				//4x4 block = 12 x 48
			GDP_CSIZE_REG = 0x21;  
			CMD(PEN_DRAW);
			DynVisObj* ptr = addDynObj(OUCH, gXPosPlayer + 12, gYPosPlayer + 10, SPRITE_OUCH);
			gGameState = MOURN;
			counter++;
		} else {
			
			if (counter > 25) {  //25 frame mourning delay when Shamus expires
				gGameStats.noOfLives--;
				gGameStats.isDirty = TRUE;
				gShamusExpires= FALSE;
				gLoadNewRoom = TRUE;
				gNoOfBullets = 0;
				gFrameLastBulletMonster = 0;
				gFrameLastBulletPlayer = 0;
				gGameState = PLAY;
				counter = 0;
			} else {
				counter++;
			}
		}
	}

	if (newRoom != gRoomNumber) {
		gRoomNumber = newRoom;
		gRoomEntryFrom = (DIRECTION)roomEntryFrom;
		gLoadNewRoom = TRUE;
	}

	if (gGameStats.isDirty == TRUE) {
		
		if (gGameStats.noOfLives < 1) {
			DebugPrintPos(30, 30, (char*)"Game over", 0, DEC); //re-start game
			//wait 10sec
			gGameState = INTRO;
			gGameStats.level = 1;
			gGameStats.score = 0;
			prep2();
		}
		gGameStats.isDirty = FALSE;
	}

	//DEBUG
	if (gDrawHotSquares == TRUE) {
		drawSensorSquare();
	}

}*/

void _start() {

	//start routine for NKC

	prep();
	while (gGameState != EXIT) {
		iterate();		
	}
}

#endif