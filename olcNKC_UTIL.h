#ifndef NKC_UTIL
#define NKC_UTIL

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)
#define BYTE_AT(adr) (*(unsigned char volatile *) adr)
#define WORD_AT(adr) (*(unsigned short volatile *) adr)
#define LONG_AT(adr) (*(unsigned long volatile *) adr)

#define STROBE 0b10000000

#define LA_TRIGGER_A BYTE_AT(0xFFFFFF32)
#define LA_TRIGGER_B BYTE_AT(0xFFFFFF33)


//#define FOREACH(i, a) for (int i=0; i < sizeof(a) / sizeof(typeof(a)); i++)
//#define INIT(what, how) for (int i=0; i < sizeof(what) / sizeof(typeof(what)); i++) {what[i]=how;}

//#define DB DebugPrint(__FILE__, __LINE__, DEC);
#define DB //DebugPrintPos(0, 0, __FILE__, __LINE__, DEC);

#define mb asm volatile("": : :"memory"); 

#define NOP asm volatile("nop");
//#define GDP_WAIT() asm volatile ("dc.w 0x0839, 0x0002, 0xFFFF, 0xFF70, 0x67F6"); /* wait: btst.b #$02, $FFFFFF70.l  beq.s wait */

//#define WAIT_FOR_KEY() asm volatile ("dc.w 0x7e0c, 0x4e41, 0x0c00, 0x000d, 0x66f6"); 					  
//#define NKC_BREAKPOINT() asm volatile ("dc.l 0x41FA000A, 0x20084EB9, 0x00001C0C"); //GP 7.10
#define NKC_BREAKPOINT() __asm__ volatile ("dc.w 0x4eb9, 0x0000, 0x1bd0"); //GP 7.20
//#define NKC_BREAKPOINT_NEW() asm volatile ("dc.w 0x2f00, 0x2f08, 0x4eb9, 0x0000, 0x1bd0, 0xdefc, 0x0008, 0x205f, 0x2017, 0x9efc, 0x000c"); //GP 7.20

#define TRUE 1
#define FALSE 0

typedef unsigned char  byte; // .b
typedef unsigned short word; // .w

byte KEY_REG = 0x00;       //#define KEY_REG BYTE_AT(0xFFFFFF68)
byte KEY_PORT_REG = 0x00;  //#define KEY_PORT_REG BYTE_AT(0xFFFFFF69)

byte IOE_PORTA = 0xFF;  //#define IOE_PORTA BYTE_AT(0xFFFFFF30)
byte IOE_PORTB = 0xFF;  //#define IOE_PORTB BYTE_AT(0xFFFFFF31)

int ticks = 0;
char flic = 0;

static int CRITICAL_ADR;
static int CRITICAL_VAL;

//m68k specifics
#define CPU 1   //1=68008 2=68000 4=68020

#if defined(NKC)
	//defined in linkerscript ram.ld
	extern int _bss_sta,  _bss_end;
	extern int _text_sta, _text_end;
	extern int _data_sta, _data_end;
	extern int _int_handler_sta;			//location of interrupt handler
#endif


typedef enum  {DEC=0, HEX=1, NONE=2, SMALL=3, SLOWTEXT=4} DATA_TYPE;

typedef struct ConsoleArgs
{
	int x;
	int y;
	int fontSize;
} ConsoleArgs;
ConsoleArgs consoleArgs;


#if defined(NKC)
	void DebugPrintPos(int x, int y, char *text, int value, DATA_TYPE t);
#endif

/*void  __attribute__((section(".gp_ram_interrupt"))) int_jump() {
	//define .gp_ram_interrupt section in linker script (GP_ram at $10000)
	//asm("jmp (int_handler_func:w,%pc)");
	//will also place 'RTS', cannot avoid
}

//long getStackAdr(int dummy) {return (int)&dummy;}

void __attribute__ ((inline)) STOP() {
	asm("stop #0b0010011100000000");
}*/

/*void __attribute__ ((inline)) interrupt_OFF() {
	asm("move #0b0010011100000000,%SR"); //ignore all interrupts 
}*/

/*void __attribute__ ((inline)) interrupt_ON() {
	asm("move #0b0010000000000000,%SR"); //allow interrupts 
}*/

/*void init_interrupt() {
	//places jump to interrupt handler into GP ram
	#define NMI_JMP WORD_AT(0x10000)  //location of NMI interrupt
	#define NMI_ADR LONG_AT(0x10002)  //handler in GP 7.4
	NMI_JMP = 0x4ef9;         //'jmp' memnomic
	NMI_ADR = (int)&_int_handler_sta;  
	ticks=0;
	interrupt_OFF();
}*/

//section .interrupt set as per linker script location is known: _int_handler_sta
//void  __attribute__((interrupt)) __attribute__((section(".interrupt"))) int_handler_func() {
	//do something
	//ticks++;
//}

/*int __attribute__ ((noinline)) get_pc() {
	asm("lea get_pc,%a0");   //put %pc in %a0
	asm("movel %a0,%d0");    //d0 is location of func return value
}*/

//GCC extended asm example
/*void getStack() {
	
	__asm__ __volatile__(
		"movel %%sp, %0\n\t"
		"movel %%sp@, %1\n\t"
		: "=m"(CRITICAL_ADR), "=m"(CRITICAL_VAL)  
		: 
		: 
	);
}*/



/*void initMemory() {

#if defined(NKC)
	//clear BSS section
	int* text_sta = &_text_sta;
	int* bss_start = &_bss_sta;
	int* bss_end = &_bss_end;

	int size = bss_end - bss_start;
	memset(bss_start, 0x00, size);
#else
	//extern int gMaxCountsPerHSYNC;
	//gMaxCountsPerHSYNC = 1927; //typical value for 68008, 1 wait state
#endif
}*/


byte toASCII(byte val) {

   byte res = val;
 
	if (res < 0x0a) {
		res += '0';
	} else {
		res -= 0x0a;
		res += 'A';
	}
	return res;
}




/*void __attribute__ ((noinline)) playYMFrame(char value) {

	unsigned char reg[12] = {0x08, 0x00, 0x01, 0x07};
 	unsigned char val[12] = {0x0F, 0x5D, value, 0x00};
	
 	for (int i=0; i<4; i++) {
    	SOUND_CMD0 = reg[i];
   	 	SOUND_CMD1 = val[i]; 
  	}
}*/


//byte __attribute__((inline)) readKey() {  
byte readKey() {

	byte keys;
	byte keyd = KEY_REG;
			
	if (keyd <= 0b10000000) {
		keys = KEY_PORT_REG;    //clear STROBE
		keyd = 0x00;
	}

	return keyd;	
}

//word __attribute__((inline)) readIOEports() {
word readIOEports() {

	//code: UP    - 0xFB, DOWN - 0xF7
	//		RIGHT - 0xFD, LEFT - 0xFE
	//      FIRE  - 0xEF;

	byte portA = IOE_PORTA;
	byte portB = IOE_PORTB;

	word w = portA + (portB << 8);

	return w;
}
#endif


