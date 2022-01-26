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

void hsync(void) __interrupt(TF0_VECTOR) __naked{
	__asm
		setb	_P1_0
		inc	_line
		mov	a,_line
		rrc	a
		anl a,#0x7C
		add	a,#0xF8+_frameBuffer
		mov	r1,a
		clr	_P1_0
	
		mov	_currentByte,@r1
		inc r1
		mov	_currentByte+1,@r1
		inc r1
		mov	_currentByte+2,@r1
		inc r1
		mov	_currentByte+3,@r1
		nop
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
	BitColor = bit27;
	BitColor = bit26;
	__asm;	
		nop
		nop
	__endasm;
	BitColor = 1;
	
	if(line == 0){
		BitSync = 1;
		P1_1 = 1;

		TR1 = 0;
		TH1 = (char)0xFF-(char)10;
		TL1 = 0xF0;
		TR1 = 1;
		
		gameLogic();
		while(!TF1);
		TR1 = 0;
		TF1 = 0;
	
		
		BitSync = 0;
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
	
	while(1);
}
