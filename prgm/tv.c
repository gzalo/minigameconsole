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

void inline shortPulse(){
	// Short 2us sync pulses every 32 us
	BitSync = 1;
	__asm;
	nop
	nop
	nop
	__endasm;
	BitSync = 0;
	__asm;
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop				
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop			
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop				
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop		
	__endasm;
}

void inline shortPulseLast(){
	// Short 2us sync pulses every 32 us
	BitSync = 1;
	__asm;
	nop
	nop
	nop
	__endasm;
	BitSync = 0;
	__asm;
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop				
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop			
	nop
	nop
	nop
	__endasm;
}

void inline longPulse(){
	// Long sync 30us every 32
	BitSync = 1;
	__asm;
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop				
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop			
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop				
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	__endasm;
	BitSync = 0;
	__asm;
	nop
	nop
	nop
	__endasm;	
}

void hsync(void) __interrupt(TF0_VECTOR) __naked{
	if (line == 0 && lineHigh==0 ) {
		shortPulse();
		shortPulse();
		shortPulse();
		shortPulse();
		shortPulse();
		shortPulse();
		longPulse();
		longPulse();
		longPulse();
		longPulse();
		longPulse();
		shortPulse();
		shortPulse();
		shortPulse();
		shortPulse();
		shortPulseLast();
	} else {
		BitSync = 1;
		__asm
			mov	a,_line
			mov c,_lineHigh
			rrc	a
			anl a,#0x7C
			add	a,#0xF5+_frameBuffer
			mov	r1,a
			mov	_currentByte,@r1
			inc r1
			clr	_P1_0 ; SYNC
		
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
		// BitColor = bit27;
		// BitColor = bit26;		
		BitColor = 1;
	}

	// Next line
	line++;
	if(line == 0 && lineHigh == 0){
		lineHigh = 1;
	}else if(line >= 49 && lineHigh == 1){
		lineHigh = 0;
		line = 0;
		newFrame = 1;
		P1_1 = 1;
		P1_1 = 0;
	}

	__asm;
		reti
	__endasm;
}

int main(){
	tvInit();
		
	TMOD = T0_M1 | T1_M0;
	TH0 = 128; //T0 interrumpe cada 64 uS (HSYNC)
	IT0 = 1;
	
	TR0 = 1;
	
	EA = 1;
	ET0 = 1;

	SCON = 0;
	
	while(1){
		if(newFrame){
			newFrame = 0;
			gameLogic();
		}
	}
}
