#ifndef NKC_CONSOLE
#define NKC_CONSOLE

//#include "C:/SysGCC/m68k-elf/bin/libs/NKC_GDP64.h"
#include <limits.h>

//  console.h
//typedef unsigned short word; // .w

//should be merged into NKC_UTIL.h

#define noOfLines 5
#define charsInLine 5

int result = 0;
int prev = 42;

#if defined(_WIN32)
  static inline int static_result = 0;
  static inline int static_prev = 42;
#else 
  /*static inline*/ int static_result = 0;
  /*static inline*/ int static_prev = 42;
#endif

byte noOfCharsInLine[noOfLines];
unsigned char lines[noOfLines][charsInLine];

static byte pageA;
static byte pageB;
static byte page;

//pointer to lines, no re-shuffle on insert



/*void NKC_console_init() {

	for (int i = 0; i < noOfLines; i++) {
		noOfCharsInLine[i] = 1;
	}
	
	lines[0][0] = '0', 0x00;
	lines[1][0] = '1', 0x00;
	lines[2][0] = '2', 0x00;
	lines[3][0] = '3', 0x00;
	lines[4][0] = '4', 0x00;

}

void NKC_console_out() {

	word y = 0;
	
	CMD(0x04);  //CLS
	//GDP_PAGE_REG = page;  //assumes new page is empty
	
	for (int i = 0; i < noOfLines; i++) {
		GDP_MOVETO(0, y);
		for (int p = 0; p < noOfCharsInLine[p]; p++) {
			if (lines[i][p] == 0x00) break;
			unsigned char c = lines[i][p];
			CMD(c);
		}
		y += 10;
	}
}

void NKC_console_write() {

	//move all up
	for (int i = noOfLines-1; i > -1; i--) {
		
		for (int p = 0; p < noOfCharsInLine[p]; p++) {
			if (lines[i][p] == 0x00) break;
			lines[i+1][p] = lines[i][p];
		}

	}

	//place new
	lines[0][0] = 0x00;
}*/










/*#ifdef old_random_func
void __attribute__ ((noinline)) mod_68k(int a, int b) {

	__asm__ __volatile__(
		"movel %1, %%d0\n\t"
		"movel %2, %%d1\n\t"
		
		"divu %%d1, %%d0\n\t"
		"clr.w %%d0\n\t"
		"swap %%d0\n\t"
		
		"movel %%d0, %0\n\t"
		
		: "=m"(result)  //
		: "r"(a), "r"(b) //
		: "d0"// 
	);

// access to C Variables:
   //use .l to access int
   //use .w to access word
//
}

void bar(word *loc) {

	__asm__ __volatile__(

	"movea %0, %%a0\n\t"
	"add #1, (%%a0)\n\t"
		:  
		: "r"(loc) 
		: "a0"
	);
}
word foo(word valueA) {

	register long res  __asm__("%d5");

	__asm__ __volatile__(
		"move 0, %%d5\n\t"
		//"add #1, d0\n\t"
		//"move d0, %1\n\t"
		//"clr.w d1\n\t"
		//"swap d1\n\t"

		: "=r"(res)  // OUTPUTS 
		: "r"(valueA) // INPUTS 
		: // CLOBBERS 
	);

	return res;
} 
#endif
*/

static int rnd() {

	int a = static_prev;

	//mod_68k(7 * a, 997);  //this changes 'result'
	static_result = (29 * a) % 5003;

	static_prev = static_result;

	//mod_68k(result - 1, 100);
	static_result = (static_result % 100) - 1;

	return static_result + 1;
}

//returns number between 0 and 99
int getRandom() {
	


	//int a = prev;

	//mod_68k(7 * a, 997);  //this changes 'result'
	result = (29 * prev) % 5003;

	prev = result;

	//mod_68k(result - 1, 100);
	result = (result % 100) - 1;

	return result+1;
	//return rand() % 100;
}

int getRandomS(int seed) {

	//int a = prev;

	//mod_68k(7 * a, 997);  //this changes 'result'
	result = (29 * seed) % 5003;

	prev = result;

	//mod_68k(result - 1, 100);
	result = (result % 100) - 1;

	return result + 1;
	//srand(seed);
	//return rand() % 100;
}

/*unsigned int getRandom2()
{
	//lcg_parkmiller
	
	unsigned int state = prev * 48271 % 0x7fffffff;

	prev = state;

	result = state % 100;

	return NKClib_abs(result);
}*/

unsigned int getRandom2S(int seed)
{
	//lcg_parkmiller

	unsigned int state = seed * 48271 % 0x7fffffff;

	//prev = state;

	result = (state % 100) - 1;

	return result + 1;
}

uint32_t getRandom3S(uint32_t seed) {

	//Lehmer32 https://www.youtube.com/watch?v=ZZY9YE7rZJw&t=1881s  16:27min

	uint32_t nLehmer = seed;

	nLehmer += 0x120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2 % 101;
}

/*unsigned int getRandom3S(unsigned int seed) {

	//Lehmer32 https://www.youtube.com/watch?v=ZZY9YE7rZJw&t=1881s  16:27min
	//https://learn.microsoft.com/en-us/archive/msdn-magazine/2016/august/test-run-lightweight-random-number-generation

	unsigned int nLehmer = seed;

	nLehmer += 0x120fc15;
	unsigned int tmp;
	tmp = (unsigned int)nLehmer * 0x4a39b70d;
	unsigned int m1 = (tmp >> 32) ^ tmp;
	tmp = (unsigned int)m1 * 0x12fad5c9;
	unsigned int m2 = (tmp >> 32) ^ tmp;
	return m2 % 101;
}*/

///*** MATH

int NKClib_abs(int x) {
	int mask = (x >> (sizeof(int) * CHAR_BIT - 1));
	return (x + mask) ^ mask;
}

// https://spin.atomicobject.com/2012/03/15/simple-fixed-point-math/

/*int MULT_FPMM(int a, int b) {

	//return ((int64_t)x * (int64_t)y) / (1 << 16);
	       x *          

}*/






#endif 