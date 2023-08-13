#include <at89x52.h>

// Combinaciones que se prenden en cada boton (para acceso por polling)
#define BTN_UP (!P3_2)
#define BTN_DOWN (!P3_4)
#define BTN_LEFT (!P3_3)
#define BTN_RIGHT (!P3_5)

// Para no quedarnos sin RAM
#define TV_MODE 

#include "tv_framebuffer.c"
#include "sound_disabled.c"
#include "gamelogic.c"

inline void drawLine(void) {
	BitSync = 1;
	__asm
		mov	a,_line
		mov c,_lineHigh
		rrc	a					; A = line number/2 (0 .. 154)
		anl a,#0x7C				 ; A = 01111100 (4 bytes per 2 lines)
		add	a,#0xF0+_frameBuffer ; Subtract 32 lines
		mov	r1,a
		nop
		clr	_P1_0 ; SYNC
		nop
		nop
	
		mov	_currentByte,@r1
		inc r1
		mov	_currentByte+1,@r1
		inc r1
		mov	_currentByte+2,@r1
		inc r1
		mov	_currentByte+3,@r1			
	__endasm;
		
	BitColor = bit07;
	BitColor = bit06;
	BitColor = bit05;
	BitColor = bit04;
	BitColor = bit03;
	BitColor = bit02;
	BitColor = bit01;
	BitColor = bit00;
	BitColor = bit15;
	BitColor = bit14;
	BitColor = bit13;
	BitColor = bit12;
	BitColor = bit11;
	BitColor = bit10;
	BitColor = bit09;
	BitColor = bit08;
	BitColor = bit23;
	BitColor = bit22;
	BitColor = bit21;
	BitColor = bit20;
	BitColor = bit19;
	BitColor = bit18;
	BitColor = bit17;
	BitColor = bit16;        
	BitColor = bit31;
	BitColor = bit30;
	BitColor = bit29;
	BitColor = bit28;
	__asm;	
		nop
		nop
	__endasm;	
	BitColor = 1;
}

inline void sixusdelay(){
	__asm;
	nop
	nop
	nop
	__endasm;
}

inline void vsync(void) {
	sixusdelay();
	BitSync = 1;

	TH1 = 0xFE; // 320 ticks (160 us, 2.5 lines)
	TL1 = 0xC6;
	TR1 = 1;

    gameLogic();
	P1_1 = 1;
	P1_1 = 0;
	if(TF1){ // Took more time than expected
		TR1 = 0;
		unsigned char estimatedLines = (TH1<<1) + (TL1>>7); // (TH1:TL1)/128 (64 us per line)
		line += estimatedLines + 1; // Compensate for delay (estimate)
	} else {
		while(!TF1); // Wait remaining 320 us
	}
	BitSync = 0;
	TR1 = 0;
	TF1 = 0;
}

int main(){
	tvInit();
		
	TMOD = T0_M1 | T1_M0; // Timer 0 8 bit autoreload, timer 1 16 bit mode
	TH0 = 128; // T0 interrumpe cada 64 uS (HSYNC)
	TR0 = 1; // Run timer0
	TR1 = 0;

	EA = 1;
	ET0 = 1;
	
	while(1){
		PCON = 1; // Go into idle mode
		drawLine();
		
		// Next line
		line++;
		if(line == 0 && lineHigh == 0){
			lineHigh = 1;
		}else if(line >= 53 && lineHigh == 1){
			lineHigh = 0;
			line = 0;

			vsync(); // 320 us max
		}
	}
}
