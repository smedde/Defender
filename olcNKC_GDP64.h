#ifndef NKC_GDP
#define NKC_GDP

//EF9366 hardware layer
byte GDP_CMD_REG = 0x00;    //#define GDP_CMD_REG BYTE_AT(0xFFFFFF70)
byte GDP_CTRL1_REG = 0x00;  //#define GDP_CTRL1_REG BYTE_AT(0xFFFFFF71)
byte GDP_CTRL2_REG = 0x00;  //#define GDP_CTRL2_REG BYTE_AT(0xFFFFFF72)
byte GDP_CSIZE_REG = 0x00;  //#define GDP_CSIZE_REG BYTE_AT(0xFFFFFF73)   0bxxxxyyyy

byte GDP_DELTAX_REG = 0x00; //#define GDP_DELTAX_REG BYTE_AT(0xFFFFFF75)
byte GDP_DELTAY_REG = 0x00; //#define GDP_DELTAY_REG BYTE_AT(0xFFFFFF77)
  
byte GDP_X_MSB_REG = 0x00;  //#define GDP_X_MSB_REG BYTE_AT(0xFFFFFF78)
byte GDP_X_LSB_REG = 0x00;  //#define GDP_X_LSB_REG BYTE_AT(0xFFFFFF79)

byte GDP_Y_MSB_REG = 0x00;  //#define GDP_Y_MSB_REG BYTE_AT(0xFFFFFF7A)
byte GDP_Y_LSB_REG = 0x00;  //#define GDP_Y_LSB_REG BYTE_AT(0xFFFFFF7B)

byte GDP_PAGE_REG = 0x00;   //#define GDP_PAGE_REG BYTE_AT(0xFFFFFF60)

byte GDP_STATE_PEN_UPDOWN = 0x00;   //TRUE=UP    FALSE=DOWN
byte GDP_STATE_PEN_DRAWERASE = 0x00; //TRUE=DRAW  FALSE=ERASE

#if CPU == 1

	#if defined(NKC)
		#define GDP_WAIT() __asm__ volatile ("dc.w 0x0839, 0x0002, 0xFFFF, 0xFF70, 0x67F6"); /* wait: btst.b #$02, $FFFFFF70.l  beq.s wait */
		//#define GDP_WAIT() __asm__ volatile ("dc.w 0x0839, 0x0002, 0xFFFF, 0xFF70, 0x4E71"); /* wait: btst.b #$02, $FFFFFF70.l  NOP */

		//#define GDP_SYNC_POS() ____asm____ volatile ("dc.w 0x0839, 0x0001, 0xFFFF, 0xFF70, 0x67F6"); /* wait: btst.b #$01, $FFFFFF70.l  beq.s wait */
		#define GDP_SYNC_NEG() __asm__ volatile ("dc.w 0x0839, 0x0001, 0xFFFF, 0xFF70, 0x66F6"); /* wait: btst.b #$01, $FFFFFF70.l  bne.s wait */
	#else 
		#define GDP_WAIT()
		#define GDP_SYNC_NEG() 
	#endif

#endif

//GDP Kurzvektoren, wenn stift up: 512x512 sonst 512x256
#define v3EE 0b11111000 
#define v3NE 0b11111001
#define v3NN 0b11111010  
#define v3NW 0b11111011
#define v3WW 0b11111110   
#define v3SW 0b11111111
#define v3SS 0b11111100   
#define v3SE 0b11111101

#define v2EE 0b11010000   
#define v2NE 0b11010001
#define v2NN 0b11010010   
#define v2NW 0b11010011
#define v2WW 0b11010110   
#define v2SW 0b11110111
#define v2SS 0b11010100   
#define v2SE 0b11010101

#define v1EE 0b10101000   
#define v1NE 0b10101001
#define v1NN 0b10101010   
#define v1NW 0b10101011
#define v1WW 0b10101110   
#define v1SW 0b10101111
#define v1SS 0b10101100
#define v1SE 0b10101101

#define v0EE 0b10000000 
#define v0NE 0b10000001
#define v0NN 0b10000010
#define v0NW 0b10000011
#define v0WW 0b10000110 
#define v0SW 0b10000111
#define v0SS 0b10000100
#define v0SE 0b10000101

#define vDRAW  0x00
#define vERASE 0x01
#define vDOWN  0x02
#define vUP    0x03

#define PEN_DRAW  0x00
#define PEN_ERASE 0x01
#define PEN_DOWN  0x01  //???
#define PEN_UP    0x00  //???

#define square 0x0b
#define cursor 0x0a

#define VECTOR_TYPE_CONTINOUS     0x00
#define VECTOR_TYPE_DOTTED        0x01
#define VECTOR_TYPE_DASHED        0x02
#define VECTOR_TYPE_DOTTED_DASHED 0x03

olc::Pixel col = olc::WHITE;


#if defined(NKC)
	//void playSound(int skip);
#endif

byte font_array[10][13] ={{v3NN, v1NN,  v2EE,  v3SS, v1SS,  v1WW,  v1EE,  vUP, v2EE, vDOWN,  0x00,  0x00, 0x00},
							{v3NN, v1NN,  v3SS,  v1SS,  vUP,  v2EE, vDOWN, 0x00, 0x00,  0x00,  0x00,  0x00, 0x00},
							{v2NN, v3EE,  v2NN,  v3WW,  vUP,  v3SS, vDOWN, v1SE, v2EE,   vUP,  v2EE, vDOWN, 0x00},
							{v3EE, v2NN,  v3WW,   vUP, v2NN, vDOWN,  v3EE, v2SS,  vUP,  v2SE, vDOWN,  0x00, 0x00},
							{ vUP, v2NN, vDOWN,  v2NN, v2SS,  v3EE,  v2NN, v1SS, v3SS,   vUP,  v2EE, vDOWN, 0x00},
							{v3EE, v2NN,  v3WW,  v2NN, v3EE,   vUP,  v3SS, v1SE, v1EE,  0x00,  0x00,  0x00, 0x00},
							{v3NN, v1NN,  v3EE,  v3WW, v2SS,  v3EE,  v2SS, v2WW,  vUP,  v3EE,  v1EE,  0x00, 0x00},
							{ vUP, v3NN,  v1NN, vDOWN, v3EE,  v1SS,  v2SW, v1SS,  vUP,  v2EE,  v3EE,  0x00, 0x00},
							{v3NN, v1NN,  v3EE,  v2SS, v2WW,  v2EE,  v2SS, v2WW,  vUP,  v2EE,  v2EE,  0x00, 0x00},
							{ vUP, v2NN, vDOWN,  v2NN, v3EE,  v2SS,  v2WW, v2EE, v2SS,   vUP,  v2EE,  0x00, 0x00}};


byte  volatile NKClib_page = 1;  //must be volatile
int NKClib_frame = 0;

int syncCounter = 0;
//int syncTested = 0;
int syncIsUp = 0;

#if defined(NKC)
//forward decleration
void DebugPrint(char *text, int value, DATA_TYPE t);
void WAIT_MS(int count);
//void __attribute__ ((inline)) GDP_SYNC_POS();
//void GDP_SYNC_POS();
#endif

#define olcSCREENsizeY 255


void moveX(int val) {int x = ((GDP_X_MSB_REG * 0x100) + GDP_X_LSB_REG) + val; GDP_X_LSB_REG = (byte)x; GDP_X_MSB_REG = (byte)(x >> 8);}
void moveY(int val) {GDP_Y_LSB_REG += (byte)val;}
int translateY(int val) {return (olcSCREENsizeY - val) ;}
int getX() { return ((GDP_X_MSB_REG * olcSCREENsizeY) + GDP_X_LSB_REG); };
int getY() { return translateY(GDP_Y_LSB_REG); }

//void __attribute__ ((inline)) CMD(byte val) { 
void CMD(byte val) {
	GDP_WAIT();
	GDP_CMD_REG = val; 
	
	//printable character
	if ((val >= 0x20) && (val <= 0x7F)) {
		const std::string ch(1, val);
		int x = getX();
		int y = getY();
		olc::PixelGameEngine::DrawString(x, y-3, ch, col);
		moveX(7 * (GDP_CSIZE_REG >> 4));
		return;
	}

	switch (val) {
	case (0x00): GDP_STATE_PEN_DRAWERASE = PEN_DRAW;  col = olc::WHITE; break;
	case (0x01): GDP_STATE_PEN_DRAWERASE = PEN_ERASE; col = olc::BLACK; break;
	case (0x02): GDP_STATE_PEN_UPDOWN = PEN_DOWN; break;
	case (0x03): GDP_STATE_PEN_UPDOWN = PEN_UP;   break;

	case (0x04): {FillRect(0, 0, 512, 256, olc::BLACK); break; } //clear screen
	case (0x05): GDP_X_LSB_REG = 0; GDP_X_MSB_REG = 0; GDP_Y_LSB_REG = 0; break; //x and y 0
	case (0x06): GDP_X_LSB_REG = 0; GDP_X_MSB_REG = 0; GDP_Y_LSB_REG = 0; FillRect(0, 0, 511, 255, olc::BLACK); break; //x any y 0  and cls
	case (0x07): GDP_X_LSB_REG = 0; GDP_X_MSB_REG = 0; GDP_Y_LSB_REG = 0; GDP_CSIZE_REG = 0x11; FillRect(0, 0, 511, 255, olc::BLACK); break; //x any y and czise and cls
	case (0x08): break; //light pen
	case (0x09): break; //light pen
	case (0x0a): {  //5x8 block
		int width = 6 * (GDP_CSIZE_REG >> 4);
		int height = 8 * (GDP_CSIZE_REG && 0b00001111);
		FillRect(getX(), getY() - height + 1, width, height , col);
		moveX(6 * (GDP_CSIZE_REG >> 4));  //??? need to check with real hardware
		break; }
	case (0x0b): { //4x4 block
		int width = 4 * (GDP_CSIZE_REG >> 4);
		int height = 4 * (GDP_CSIZE_REG & 0b00001111);
		FillRect(getX(), getY() - height + 1, width, height, col);
		moveX(7 * (GDP_CSIZE_REG >> 4));  //??? need to check with real hardware
		break; }
	case (0x0c): break; //inverse
	case (0x0d): GDP_X_LSB_REG = 0; GDP_X_MSB_REG = 0; break; //x =0 
	case (0x0e): GDP_Y_LSB_REG = 0; break; //y =0
	case (0x0f): break; //DMA

	case (0x10): {DrawLine(getX(), getY(), getX() + GDP_DELTAX_REG, getY(), col);
				  moveX(GDP_DELTAX_REG);
				  break; }  //HZ line
	case (0x14): {DrawLine(getX(), getY(), getX(), getY() - GDP_DELTAY_REG, col);
				  moveY(GDP_DELTAY_REG);
				  break; }  //VT line

	//Kurzvektoren
	case(v1EE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 1, getY(), col); } moveX(+1); moveY(0); break; }
	case(v1WW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 1, getY(), col); } moveX(-1); moveY(0); break; }
	case(v1SS): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX(), getY() + 1, col); } moveX(0); moveY(-1); break; }
	case(v1NN): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX(), getY() - 1, col); } moveX(0); moveY(+1); break; }
	case(v1NE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 1, getY() - 1, col); } moveX(+1); moveY(+1); break; }
	case(v1NW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 1, getY() - 1, col); }moveX(-1); moveY(+1); break; }
	case(v1SE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 1, getY() + 1, col); }moveX(+1); moveY(-1); break; }
	case(v1SW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 1, getY() + 1, col); }moveX(-1); moveY(-1); break; }

	case(v2EE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 2, getY(), col); }moveX(+2); moveY(0); break; }
	case(v2WW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 2, getY(), col); }moveX(-2); moveY(0); break; }
	case(v2SS): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX(), getY() + 2, col); }moveX(0); moveY(-2); break; }
	case(v2NN): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX(), getY() - 2, col); }moveX(0); moveY(+2); break; }
	case(v2NE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 2, getY() - 2, col); } moveX(+2); moveY(+2); break; }
	case(v2NW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 2, getY() - 2, col); } moveX(-2); moveY(+2); break; }
	case(v2SE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 2, getY() + 2, col); } moveX(+2); moveY(-2); break; }
	case(v2SW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 2, getY() + 2, col); } moveX(-2); moveY(-2); break; }

	case(v3EE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 3, getY(), col); } moveX(+3); moveY(0); break; }
	case(v3WW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 3, getY(), col); } moveX(-3); moveY(0); break; }
	case(v3SS): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX(), getY() + 3, col); } moveX(0); moveY(-3); break; }
	case(v3NN): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX(), getY() - 3, col); } moveX(0); moveY(+3); break; }
	case(v3NE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 3, getY() - 3, col); } moveX(+3); moveY(+3); break; }
	case(v3NW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 3, getY() - 3, col); } moveX(-3); moveY(+3); break; }
	case(v3SE): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() + 3, getY() + 3, col); } moveX(+3); moveY(-3); break; }
	case(v3SW): {if (GDP_STATE_PEN_UPDOWN == PEN_DOWN) { DrawLine(getX(), getY(), getX() - 3, getY() + 3, col); } moveX(-3); moveY(-3); break; }

				//default: {
				//	const std::string ch(1, val);
				//	olc::PixelGameEngine::DrawString(240, 125, ch, RED);
				//}
	}
}

/*void __attribute__ ((inline)) newCMD(byte val) {   
  
  if ((val <= 0x2F) || (val >= 0x3A)) {
      GDP_WAIT();
      GDP_CMD_REG = val;
  } else {
      //draw numbers in small font
      int i=0;
      while (font_array[val-0x30][i] != 0x00) {
        GDP_WAIT();
        GDP_CMD_REG = font_array[val-0x30][i++];
      }
  }
}*/

//void __attribute__ ((inline)) GDP_SYNC_POS() {
void  GDP_SYNC_POS() {
	int count = 0;
	register word c;

#if defined(NKC)
	__asm__ __volatile__(
		"1:addi #1, %1\n\t"
		"btst.b #1, 0xFFFFFF70\n\t"  /* VSYNC ==  0 ? */
		"bne.s 1b\n\t"

		"2:addi #1, %1\n\t"
		"btst.b #1, 0xFFFFFF70\n\t"
		"beq.s 2b\n\t"

		"3:addi #1, %1\n\t"
		"btst.b #1, 0xFFFFFF70\n\t"
		"bne.s 3b\n\t"

		"movel %1, %0\n\t"
		: "=m"(count)  /* OUTPUTS */
		: "r"(c)  /* INPUTS */
		:  /* CLOBBERS */
	);
#endif
}




//void __attribute__ ((inline)) WAIT(int count) {
void WAIT_MS(int count) {

	for (word i = 0; i < count; i++) {
		//GDP_SYNC_POS();
		Sleep(50);
	}
}

//byte __attribute__ ((inline)) getSyncSample() {
byte getSyncSample() {

  if ((GDP_CMD_REG & 0b00000010) == 0b00000010) {
    return TRUE;
  } else {
    return FALSE;
  }
}

//void __attribute__ ((inline)) GDP_CSIZE(byte val) {
void GDP_CSIZE(byte val) {
  GDP_WAIT();   
  GDP_CSIZE_REG =  val;
}

//void __attribute__ ((inline)) GDP_CLS() {  
void GDP_CLS() {
  CMD(0x06);
}

//void __attribute__ ((inline)) GDP_MOVETO(word x, word y) {
void GDP_MOVETO(word x, word y) {
  GDP_WAIT();
  GDP_X_LSB_REG = (byte)x;
  GDP_WAIT();  //needed?
  GDP_X_MSB_REG = (x >> 8);
  GDP_WAIT();          //needed?
  GDP_Y_LSB_REG = (byte)y;  
  GDP_WAIT();          //needed?
  GDP_Y_MSB_REG = (y >> 8);   //needed?
}

void GDP_DRAWTO(word x, word y) {

	word x1 = getX();
	word y1 = 255 - getY(); 

	DrawLine(x1, 255-y1, x, 255-y, col);
	GDP_MOVETO(x, y);
}

//void __attribute__ ((inline)) GDP_BLANK(word x, word y) {
void GDP_BLANK(word x, word y) {
  GDP_MOVETO(x, y);
  CMD(cursor);  //5x8 block
}

#ifdef GDPALL

void CRITCAL() {

    //GDP_PAGE_REG = 0b00000000; 
    //GDP_WAIT(); 
    //consoleArgs.x=0;
    //consoleArgs.y=247;
    //DebugPrint("CRITICAL!", 0, DEC);
    getStack();
    DebugPrint("SP: ", CRITICAL_ADR, HEX);
    DebugPrint("  : ", CRITICAL_VAL, HEX);
   
    //#define BYTE_AT(adr) (*(unsigned char volatile *) adr)
    //#define GDP_PAGE_REG BYTE_AT(0xFFFFFF60)

    //WAIT(50000);

}


/*void PRINT2(word x, word y, word val) {

  GDP_WAIT(); byte a = GDP_X_LSB_REG;
  GDP_WAIT(); byte b = GDP_X_MSB_REG;
  GDP_WAIT(); byte c = GDP_Y_LSB_REG;
  GDP_WAIT(); byte d = GDP_Y_MSB_REG;

  GDP_WAIT(); 
  GDP_CSIZE_REG = 0xA1;
  
  GDP_MOVETO(x, y);
  CMD(0x01); 
  CMD(0x0a); //print big inverse space
  CMD(0x00); 
  
  GDP_WAIT(); 
  GDP_CSIZE_REG =  0x21;
  
  GDP_MOVETO(x, y);
  CMD(toASCII((val >> 12) & 0xf));  //1st digit
  CMD(toASCII((val >>  8) & 0xf));  //2nd digit
  CMD(toASCII((val >>  4) & 0xf));  //3rd digit
  CMD(toASCII((val      ) & 0xf));  //4th digit

  GDP_WAIT(); GDP_X_LSB_REG = a;
  GDP_WAIT(); GDP_X_MSB_REG = b;
  GDP_WAIT(); GDP_Y_LSB_REG = c;
  GDP_WAIT(); GDP_Y_MSB_REG = d;
}*/

/*void __attribute__ ((inline)) GDP_MOVERELY(byte dy) {

  GDP_WAIT();
  word y = GDP_Y_LSB_REG;// + (GDP_Y_MSB_REG << 8);  //needed?
  GDP_WAIT();  //needed?
  GDP_Y_LSB_REG = y - dy;
  //GDP_WAIT();  //needed?
  //GDP_Y_MSB_REG = (y >> 8);
}
void __attribute__ ((inline)) GDP_MOVERELX(byte dx) {

  GDP_WAIT();
  byte x = GDP_X_LSB_REG;
  GDP_WAIT();  //needed?
  GDP_X_LSB_REG = x - dx;
  //GDP_WAIT();  //needed?
  //GDP_X_MSB_REG = (x >> 8);
}*/

/*static void __attribute__ ((noinline)) GDP_DRAWBITMAP(word x, word y) {

  int i = 0;
  int xpos = x;
  int ypos = y;

    GDP_MOVETO(xpos++, ypos); 
  
  do {
    while ((data[i] != 0xfe) ) {
      GDP_MOVERELY(data[i++]);
      GDP_VLINE(data[i++]);
    } 
    i++;
    GDP_MOVETO(xpos++, ypos);
  } while (data[i] != 0xff);
}*/

/*void PRINT1(word x, word y, word val) {

  GDP_WAIT(); 
  GDP_CSIZE_REG = 0xa1;
  
  GDP_MOVETO(x, y);
  CMD(0x01); 
  CMD(0x0a); //print big inverse space
  CMD(0x00); 
  
  GDP_WAIT(); 
  GDP_CSIZE_REG =  0x11;
  
  GDP_MOVETO(x, y);
  CMD(0x30); //'0'
  CMD(0x78); //'x'
  CMD(toASCII((val >> 12) & 0xf));  //1st digit
  CMD(toASCII((val >>  8) & 0xf));  //2nd digit
  CMD(toASCII((val >>  4) & 0xf));  //3rd digit
  CMD(toASCII((val      ) & 0xf));  //4th digit
}*/

#endif

void CMDPRINT3(word x, word y, byte *arrayPtr) {

  int i=0;

  //if (arrayPtr == 0x00) return;

  GDP_MOVETO(x, y);
  while (arrayPtr[i] != 0x00) {  
	CMD(arrayPtr[i++]);
  }
}

void CMDPRINT3SLOW(word x, word y, const byte *arrayPtr, int ms) {

  volatile int i=0;

  GDP_MOVETO(x, y);
  
  while (arrayPtr[i] != 0x00) {  
    CMD(arrayPtr[i++]);
    WAIT_MS(ms);
  }
}

//void __attribute__ ((inline)) GDP_HLINE(byte dx) {
void GDP_HLINE(byte dx) {

  GDP_WAIT();
  GDP_DELTAX_REG = dx;
  CMD(0b00010000);      //horizontal line
}

//void __attribute__ ((inline)) GDP_VLINE(byte dy) {
void GDP_VLINE(byte dy) {

  GDP_WAIT();
  GDP_DELTAY_REG = dy;
  CMD(0b00010100);      //vertical line
}


/*void clockedLoop() {

  //(18 cycles * iteration) + 6
  //  10 =   202
  // 100 =  1822
  //1000 = 18022
  
  //7996 cycles = approx 1ms
  for (word i=0; i < 443 * 5; i++) {
    __asm__("NOP");
  }

}*/

//int __attribute__ ((inline)) GDP_SYNC_IT() {
int GDP_SYNC_IT() {

#if defined(NKC)
   //wait to raising flank

   int count = 0;

    __asm__ __volatile__(
    "move.w #0, %d0\n\t"
    "1:addi #1, %d0\n\t"
    "btst.b #1, 0xFFFFFF70\n\t"  // 
    "bne.s 1b\n\t"
    
    //"2:addi #1, %1\n\t"
    //"btst.b #1, 0xFFFFFF70\n\t"
    //"beq.s 2b\n\t"

    //"3:addi #1, %1\n\t"
    //"btst.b #1, 0xFFFFFF70\n\t"
    //"bne.s 3b\n\t"

    //"movel %1, %0\n\t"
    : "=m"(count)  /* OUTPUTS */
    :   /* INPUTS */
    : "%d0" /* CLOBBERS */
  );
#else
	return 1;
#endif
}

void GDP_SET_VECTOR_TYPE(byte type) {
	//GDP_WAIT();  //KEEP
	GDP_CTRL2_REG = type;
}

/*void GDP_BEAM_ON() {
  //overrides other settings in CTRL1
  GDP_CTRL1_REG = 0b00000011;

}

void GDP_BEAM_OFF() {
  //overrides other settings in CTRL1
  
  GDP_CTRL1_REG = 0b00000111;
}*/

//void __attribute__ ((inline)) GDP_FONT_UP() {
void GDP_FONT_UP() {
  GDP_WAIT();
  GDP_CTRL2_REG = 0b00001000;
}

//void __attribute__ ((inline)) GDP_FONT_NORMAL() {
void GDP_FONT_NORMAL() {
  GDP_WAIT();
  GDP_CTRL2_REG = 0b00000000;
}

/*void prepPageNumbersForDebug() {

 GDP_WAIT();  
 GDP_PAGE_REG = 0b00000000; //0 0
 DebugPrintPos(0, 0, "", 0, DEC); 
 
 GDP_WAIT(); 
 GDP_PAGE_REG = 0b01010000; //0 0
 DebugPrintPos(8, 0, "", 1, DEC); 
 
 GDP_WAIT(); 
 GDP_PAGE_REG = 0b10100000; //0 0
 DebugPrintPos(18, 0, "", 2, DEC); 
 
 GDP_WAIT(); 
 GDP_PAGE_REG = 0b11110000; //0 0
 DebugPrintPos(24, 0, "", 3, DEC); 
}*/

//void __attribute__((inline)) pageFlip_4() {
void  pageFlip_4() {

  NKClib_frame++;
  NKClib_page++;
  if (NKClib_page >= 4) NKClib_page = 0;
   
  GDP_WAIT();  //must be there            //R W
       if (NKClib_page==0) GDP_PAGE_REG = 0b01000000; //0 1
  else if (NKClib_page==1) GDP_PAGE_REG = 0b00100000; //1 2
  else if (NKClib_page==2) GDP_PAGE_REG = 0b10110000; //2 3
  else if (NKClib_page==3) GDP_PAGE_REG = 0b11010000; //3 4

  //faster
  //place in init
  //NKClib_page_array[0] = 0b01000000;
  //NKClib_page_array[1] = 0b00100000;
  //NKClib_page_array[2] = 0b10110000;
  //NKClib_page_array[3] = 0b11010000;
  //faster
  //GDP_PAGE_REG = NKClib_page_array[page];

  //int volatile it = GDP_SYNC_IT(); //must be volatile
}


//void __attribute__ ((inline)) pageFlip_2() {
void pageFlip_2() {

  //page = !page;
  int it = 0;
  //byte volatile reg = 0;
  
  if (NKClib_page == 0) {

    GDP_WAIT();
    GDP_PAGE_REG = 0b01000000;
    NKClib_page = 1;
    //reg = GDP_PAGE_REG;
  
    //it = GDP_SYNC_IT(); 

  } else {
  
    GDP_WAIT();
    GDP_PAGE_REG = 0b00010000;
    NKClib_page = 0;
    //reg = GDP_PAGE_REG;
    

    //it = GDP_SYNC_IT();
  }

  //return it;
}

//void __attribute__ ((inline)) initGraphics() {
void initGraphics() {
  
	/*GDP_X_LSB_REG = 0x00;
	GDP_X_MSB_REG = 0x00;

	GDP_Y_LSB_REG = 0x00;
	GDP_Y_MSB_REG = 0x00;*/
	
	CMD(0x00);
	CMD(0x02);
	CMD(0x07);

	//PAGE INIT
	NKClib_page = 0;
	GDP_PAGE_REG = 0b01000000;
	GDP_CSIZE_REG =  0x21;
	for (int i=0; i < 4; i++) {
		CMD(0x06);  //cls
		pageFlip_4();
	}
}





void catchSync() {

	//syncTested++;

	byte status = getSyncSample();

	if (syncIsUp == TRUE) {
		if (status == FALSE) {
			//falling edge
			syncCounter++;
			syncIsUp = FALSE;

			//NKCplaySound(1);    

			//syncCaughtFallingEdge++;
		}
	}
	else {
		if (status == TRUE) {
			//raising edge
			syncIsUp = TRUE;
			//syncCaughtRaisingEdge++;
		}
	}

#if defined(_WIN32)
	playSound(1);
#endif

}


//int __attribute__ ((inline)) countToVSYNC() {
int countToVSYNC() {

  int count = 0;

  while ((GDP_CMD_REG & 0b00000010) != 0b00000010) {
    count++;
  }

  return count;
}

//int __attribute__ ((inline)) getMaxCountsToVSYNC() {
int getMaxCountsToVSYNC() {

  GDP_SYNC_POS(); 
  return countToVSYNC();
}



void PRINTSTRING(ConsoleArgs *consoleArgs, char *string1, char *string2) {

  int csize = GDP_CSIZE_REG;

  GDP_WAIT(); 
  GDP_CSIZE_REG = 0x11;

#if defined(NKC)
	  //function pointer, fancy
	  void (*CMDfunc)(int);

  
	  if (consoleArgs->fontSize == 0) {
		CMDfunc = (void*)&CMD;
	  } else {
		//CMDfunc = (void*)&newCMD;  //HEY HEY HEY
	  }
#endif

  //clean space, need modification for small font
  GDP_MOVETO(consoleArgs->x, consoleArgs->y);
  CMD(0x01); 
  for (int x=0; x < strlen(string1); x++) {
    CMD(0x0a); 
  }
  CMD(0x0a); 
  for (int x=0; x < strlen(string1); x++) {
    CMD(0x0a); 
  }
  CMD(0x00);

  GDP_MOVETO(consoleArgs->x, consoleArgs->y);
  for (int x=0; x < strlen(string1); x++) {
    
    byte val = string1[x];

    if ((val >= 0x20) && (val <= 0x7F)) {
#if defined(NKC)
      CMDfunc(val); 
#else 
	  CMD(val);
#endif

    } else {
      CMD(0x08); //flash for error
    }
  }

  CMD(0x20); //' ' between strings

  for (int x=0; x < strlen(string2); x++) {
    
    byte val = string2[x];

    if ((val >= 0x20) && (val <= 0x7F)) {
#if defined(NKC)
		CMDfunc(val);
#else 
		CMD(val);
#endif
    } else {
      CMD(0x08); //flash for error
    }
  }
  GDP_CSIZE(csize);

  //columns
  consoleArgs->y -= 8;
  if (consoleArgs->y <= -1) {
    consoleArgs->y = 254-6; 
    consoleArgs->x += 6 * 6;
  }
}

void DebugPrint(char *text, int value, DATA_TYPE t) {

  unsigned char base = 10;
  const char digits[] = "0123456789ABCDEF";
  char str[100];

  consoleArgs.fontSize = 0;

  if (t == SMALL) {
    consoleArgs.fontSize = 1;
    t = DEC;
  }

  //clean str array
  for (int i=0; i<10; i++) {
    str[i]=0x00;
  }

  if (t==HEX) base = 16;

  if (t != NONE) {

//if (value<0) value=0;
//if (value>1000) value=1000;
    /* Convert to string. Digits are in reverse order.  */
    int i, j;
    char c;

    i=0;

    if (value < 0) {
      str[i++] = '-';
      value = abs(value);
    }

    do {
      unsigned char remainder = value % base;
      str[i++] = digits[remainder];
      value = value / base;
    } while (value != 0);  
    str[i] = 0x00; 

    // Reverse string.  
    for (j = 0, i--; j < i; j++, i--)
    {
        c = str[j];
        str[j] = str[i];
        str[i] = c; 
    } 
  }

  //if (t == SLOWTEXT) {
  //  int i=0;
  //  do {
  //    PRINTSTRING(&consoleArgs, text[i], str);
  //    WAIT(10);
  //  } while (text[i] != 0x00); 
  //} else {
    PRINTSTRING(&consoleArgs, text, str);
  //}

}

void DebugPrintPos(int x, int y, char *text, int value, DATA_TYPE t) {

    consoleArgs.x = x;
    consoleArgs.y = y;
    consoleArgs.fontSize = 0;

    if (t == SMALL) {
      consoleArgs.fontSize = 1;
      t = DEC;   //TEMP FIX
    }

    DebugPrint(text, value, t);
}
#endif NKC_GDP





//https://stackoverflow.com/questions/12430016/bezier-drawing-routine-to-fixed-point-how-to-rewrite#
