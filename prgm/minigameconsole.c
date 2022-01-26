#include <at89x52.h>

// Combinaciones que se prenden en cada boton (para acceso por polling)
#define BTN_UP (!P3_4)
#define BTN_DOWN (!P3_2)
#define BTN_LEFT (!P3_5)
#define BTN_RIGHT (!P3_3)

#include "ledmatrix.c"
#include "sound.c"
#include "gamelogic.c"

// EX0 y EX1 -> Botones (arriba/abajo y izquierda/derecha)
// T0 -> Refresco de matriz y sonido
// T1 -> Lógica de juego
// T2 -> Sonido

void main()
{
	SOUND_PIN = 0;
	IT0 = 1;
	IT1 = 1;
	
	// Habilitados Timer0, Timer1
	IE = 0x80;
	ET0 = 1;
	ET1 = 1;
	EX0 = 1;
	EX1 = 1;
	
	// Prioridad alta al sonido (ayuda a reducir glitches de sonido)
	// El sonido es lo unico que necesita una respuesta en tiempo real
	PT2=1;
	
	// Timer0 y 1 en 16 bits, corriendo
	TMOD = T0_M0 | T1_M0;
	TR0 = 1;
	TR2 = 1;
	
	// Timer2 en modo 16 bits auto recarga
	T2CON_0 = 0;	
	
	// Apagar toda la matriz
	matrixClearAll();
	
	// Habilitar la logica de juego
	TR1 = 1;
	
	while(1);
}
