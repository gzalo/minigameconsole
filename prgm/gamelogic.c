// Combinaciones que se prenden en cada boton (para acceso por polling)
#define BTN_UP (!P3_4)
#define BTN_DOWN (!P3_2)
#define BTN_LEFT (!P3_5)
#define BTN_RIGHT (!P3_3)

// Tiempo cada cuanto se ejecuta la logica de juegos
#define GAMELOGIC_US 20000
// Cantidad de juegos
#define AMOUNT_GAMES 5

// Estados posibles de la maquina de estados
enum
{
	STATE_LIST,
	STATE_GAMEOVER,
	STATE_TETRIS,
	STATE_CAR,
	STATE_CHOPPER,
	STATE_SNAKE,
	STATE_GUITAR
};

// Posicion X, Y, juego seleccionado, estado actual, numero aleatorio
unsigned char posX,posY,game=0,state=STATE_LIST,randomNumber = 0;

// SNAKE
unsigned char fruitX, fruitY;
__idata __at (0x80) unsigned char snakePosX[64];
__idata __at (0xC0) unsigned char snakePosY[64];
unsigned char snakeLen;

unsigned char keyAffects = 0;
char vX=1, vY=0;
// Cantidad de veces que es ejecutó la lógica
unsigned short loop = 0;
// Puntaje del juego
unsigned char score = 0;
// Variables temporales para botones
unsigned char buttonUp=0, buttonDown=0, buttonLeft=0, buttonRight=0;
// Nota actual en la cancion, tiempo actual
unsigned char currentNote=0, currentTiming=0;
__code unsigned char song[] 	= "EBCDCB AACEDC BBCDE CAA   DFHH  GFEECE EDCBBC DDECA A  EC DB CA AB EC DB CEH H";
__code unsigned char timing[] 	= "211211 211211 21122 2222 221211 112211 211121 21221 22 44 44 44 44 44 44 224 8";

// Fuente 5x3 (numérica), para los puntajes, guardada en codigo por ser constante
__code const unsigned char fuente[10*5] = 
{
	0b00000010,0b00000101,0b00000101,0b00000101,0b00000010, //Cero
	0b00000001,0b00000011,0b00000001,0b00000001,0b00000001, //Uno
	0b00000110,0b00000001,0b00000010,0b00000100,0b00000111, //Dos
	0b00000110,0b00000001,0b00000010,0b00000001,0b00000110, //Tres
	0b00000001,0b00000011,0b00000101,0b00000111,0b00000001, //Cuatro
	0b00000111,0b00000100,0b00000111,0b00000001,0b00000110, //Cinco
	0b00000010,0b00000100,0b00000110,0b00000101,0b00000010, //Seis
	0b00000111,0b00000001,0b00000010,0b00000010,0b00000010, //Siete
	0b00000010,0b00000101,0b00000010,0b00000101,0b00000010, //Ocho
	0b00000010,0b00000101,0b00000111,0b00000001,0b00000110, //Nueve
};

void leftIsr() __interrupt(IE0_VECTOR) __using(0)
{
	randomNumber = TL0;
}
void upIsr() __interrupt(IE1_VECTOR) __using(0)
{
	randomNumber = TL0;
}

// Devuelve un numero pseudo-aleatorio de 8 bits (cuidado, se hace deducible)
unsigned char getRandomNumber()
{
	randomNumber=randomNumber*9+7;
	return randomNumber;
}

void gameOver(){
	unsigned char num1, num2;
	// Mostramos el puntaje
	soundStop();
	matrixClearAll();
	num1 = score/10;
	num2 = score%10;
	matrixData[0] = fuente[num2*5];
	matrixData[1] = fuente[num2*5+1];
	matrixData[2] = fuente[num2*5+2];
	matrixData[3] = fuente[num2*5+3];
	matrixData[4] = fuente[num2*5+4];
	
	matrixData[0] |= fuente[num1*5]<<4;
	matrixData[1] |= fuente[num1*5+1]<<4;
	matrixData[2] |= fuente[num1*5+2]<<4;
	matrixData[3] |= fuente[num1*5+3]<<4;
	matrixData[4] |= fuente[num1*5+4]<<4;
	
	// Quedamos esperando teclas
	state = STATE_GAMEOVER;
}

void gameList()
{
	matrixClearAll();
	// Mostramos las imagenes correspondientes al juego seleccionado
	switch(game)
	{
		case 0:
			matrixData[0] = 0b01110111;
			matrixData[1] = 0b00100100;
			matrixData[2] = 0b00100111;
			matrixData[3] = 0b00100100;
			matrixData[4] = 0b00100111;
			matrixData[5] = 0b00000000;
			matrixData[6] = 0b00000000;
			matrixData[7] = 0b00000000;
			break;
		case 1:
			matrixData[0] = 0b01110101;
			matrixData[1] = 0b01010101;
			matrixData[2] = 0b01110101;
			matrixData[3] = 0b01010101;
			matrixData[4] = 0b01010111;
			matrixData[5] = 0b00000000;
			matrixData[6] = 0b00000000;
			matrixData[7] = 0b00000000;					
			break;
		case 2:
			matrixData[0] = 0b01110101;
			matrixData[1] = 0b01000101;
			matrixData[2] = 0b01000111;
			matrixData[3] = 0b01000101;
			matrixData[4] = 0b01110101;
			matrixData[5] = 0b00000000;
			matrixData[6] = 0b00000000;
			matrixData[7] = 0b00000000;					
			break;
		case 3:
			matrixData[0] = 0b11101001;
			matrixData[1] = 0b10001101;
			matrixData[2] = 0b11101011;
			matrixData[3] = 0b00101001;
			matrixData[4] = 0b11101001;
			matrixData[5] = 0b00000000;
			matrixData[6] = 0b00000000;
			matrixData[7] = 0b00000000;									
			break;
		case 4:
			matrixData[0] = 0b11101001;
			matrixData[1] = 0b10001001;
			matrixData[2] = 0b11101001;
			matrixData[3] = 0b10101001;
			matrixData[4] = 0b11101111;
			matrixData[5] = 0b00000000;
			matrixData[6] = 0b00000000;
			matrixData[7] = 0b00000000;									
			break;
	}
	if(BTN_LEFT)
	{
		if(buttonLeft == 0)
		{
			buttonLeft = 1;
			// Si se apretó el boton izquierdo, pasamos al proximo juego
			game++;
			// Evitamos que se pase de la cantidad total de juegos
			game%=AMOUNT_GAMES;
			// Esperamos a que se suelte el boton		
		}
	}else
	{
		buttonLeft = 0;
	}
	
	if(BTN_RIGHT)
	{
		// Se eligió un juego, reseteamos el puntaje, seteamos el estado
		// correspondiente, establecemos la posicion del personaje si es 
		// necesario
		score=0;
		switch(game)
		{
			case 0:
				matrixClearAll();					
				state = STATE_TETRIS;
				posX = 3;
				posY = 0;
				break;	
			case 1:				
				matrixClearAll();
				state = STATE_CAR;
				posX = 3;
				posY = 7;
				matrixData[0] = 0b11100011;
				break;
			case 2:
				matrixClearAll();
				state = STATE_CHOPPER;
				posX = 0;
				posY = 3;
				break;					
			case 3:
				matrixClearAll();
				state = STATE_SNAKE;
				fruitX = getRandomNumber()%8;
				fruitY = getRandomNumber()%8;
				matrixSet(fruitX,fruitY);
				snakeLen = 1;
				break;						
			case 4:	
				matrixClearAll();
				state = STATE_GUITAR;
				break;
		}
	}
}

void gameTetris(){
	unsigned char i;
	
	matrixClr(posX, posY);
	
	if(BTN_LEFT)
	{
		if(buttonLeft == 0)
		{
			randomNumber = TL1;
			buttonLeft = 1;
			if(posX!=0 && !matrixGet(posX-1,posY)) posX--;
		}
	}else
	{
		buttonLeft = 0;
	}
	
	if(BTN_RIGHT)
	{	
		if(buttonRight == 0)
		{
			randomNumber = TL1;
			buttonRight = 1;
			if(posX!=7 && !matrixGet(posX+1,posY)) posX++;
		}
	}else
	{
		buttonRight = 0;
	}
	matrixSet(posX, posY);
	
	// Cada 5 ticks
	
	if(loop%5 == 0)
	{
		// Borramos la posicion actual,				
		matrixClr(posX, posY);		
		
		// Si la pieza actual no esta al final, y si no hay nada abajo, la bajamos
		if(posY!=7 && !matrixGet(posX,posY+1))
		{
			posY++;	
		}
		else
		{
			// Si hay algo abajo, dejamos la pieza quieta
			matrixSet(posX, posY);
			
			// Si la ultima fila está llena
			if(matrixData[7] == 0xFF)
			{
				// Movemos todo uno para abajo, limpiamos la primer linea y aumentamos el puntaje
				for(i=7;i>0;i--) matrixData[i] = matrixData[i-1];
				matrixData[0] = 0;
				score++;
			}
			
			// Reseteamos la posición del bloque para que caiga desde arriba
			posX = 3;
			posY = 0;
		}
		
		// Si al pintar ya estaba encendido, el juego está terminado
		if(matrixColPut(1, posX, posY)) return gameOver();
	}
	
	// Cada 400 ms
	if(loop%10 == 0){
		if(timing[currentNote] == ' '){
			soundStop();
			currentNote++;
			currentNote%=78;									
		}
		if(song[currentNote] != ' ')
			soundPlay(song[currentNote]-'A');
		currentTiming++;
			
		if(currentTiming == timing[currentNote]-'0'){
			currentTiming = 0;
			currentNote++;
			currentNote%=78;					
		}
	}	
}

void gameCar()
{
	unsigned char i;
	
	matrixClr(posX, posY);
	// Nos movemos si no estamos en los extremos
	if(BTN_LEFT)
	{
		if(buttonLeft == 0)
		{
			randomNumber = TL1;
			buttonLeft = 1;
			if(posX!=0) posX--;
		}	
	}else
	{
		buttonLeft = 0;
	}
	if(BTN_RIGHT)
	{
		if(buttonRight == 0)
		{	
			randomNumber = TL1;
			buttonRight = 1;
			if(posX!=7) posX++;
		}
	}else
	{
		buttonRight = 0;
	}
	
	// Prendemos la posición actual
	matrixSet(posX, posY);
	
	if(loop%10 == 0)
	{	
		// 1 de cada 10 ticks, 
		
		// Muevo todo una fila para abajo
		for(i=7;i>0;i--) matrixData[i] = matrixData[i-1];
		
		// Si al pintar el auto ya había algo abajo, el juego está terminado
		if(matrixColPut(1, posX, posY)) return gameOver();		
		
		// Si la primer columna no está encendida,
		if(!(matrixData[1] & 0x80))
		{
			// La primera fila es la segunda corrida una a la derecha
			matrixData[0] = (matrixData[1] >> 1) | 0x80; 			
		}
		else
		{
			// Si la ultima columna no está encendida
			if(!(matrixData[1] & 0x01))
			{
				// La primera fila es la segunda corrida uno a la izquierda
				matrixData[0] = (matrixData[1] << 1) | 1; 			
			}
			else
			{
				// Si no (estoy en el medio), el random decide para donde ir
				
				if(getRandomNumber()&0x80)
				{
					matrixData[0] = (matrixData[1] << 1) | 1; 			
				}
				else
				{
					matrixData[0] = (matrixData[1] >> 1) | 0x80; 
				}
			}
		}
		
	}
	if(loop%80==0)
	{
		// Cada una pantalla, sumo uno al puntaje
		score++;
	}
}

void gameChopper()
{
	unsigned char i;
	if(loop%5 == 0)
	{
		// Cada 5 ticks me fijo las teclas presionadas
		// y actualizo la posicion del helicóptero
		matrixClr(posX,posY);
		if(BTN_UP || BTN_DOWN || BTN_LEFT || BTN_RIGHT) randomNumber = TL1;
		if(BTN_UP && posY!=0)posY--;
		if(BTN_DOWN)posY++;
		if(BTN_LEFT && posX!=0)posX--;
		if(BTN_RIGHT && posX!=7)posX++;
		if(posY>7) return gameOver();
		if(matrixColPut(1,posX,posY)) return gameOver();
	}
	if(loop%15 == 0)
	{
		// Cada 15 ticks, muevo todo
		for(i=0;i<8;i++) matrixData[i] <<=1;
		
		// Algunos patterns aleatorios
		
		switch(getRandomNumber()%10)
		{
			case 0:
				matrixData[3] |=1;
				matrixData[4] |=1;
				break;
			case 1:
				matrixData[4] |=1;
				matrixData[5] |=1;
				break;
			case 2:
				matrixData[1] |=1;
				matrixData[2] |=1;
				break;
			case 3:
				matrixData[1] |=1;
				matrixData[3] |=1;
				break;
			case 4:
				matrixData[0] |=1;
				matrixData[4] |=1;
				break;
			case 5:
				matrixData[6] |=1;
				matrixData[5] |=1;
				break;
			default:
				break;
		}
		
		// La ultima fila está siempre encendida			
		matrixData[7] = 0xFF;
		
		// Apago el pixel a la izquierda del actual (estela después
		// de mover todo a la izq)
		matrixClr(posX-1,posY);
		
		// Si al dibujar la posicion actual habia algo antes, el juego se terminó				
		if(matrixColPut(1,posX,posY)) return gameOver();				
		
		if(loop%120==0)
		{
			// Cada una pantalla, aumento el puntaje
			score++;
		}				
	}
}

void gameSnake()
{
	char i;
	if(keyAffects==1){
		keyAffects=0;
		if(BTN_LEFT && vX==0)
		{
			vX=(char)-1;
			vY=0;
		}else if(BTN_RIGHT && vX==0)
		{
			vX=1;
			vY=0;
		}else if(BTN_UP && vY == 0)
		{
			vX=0;
			vY=(char)-1;
		}else if(BTN_DOWN && vY == 0)
		{
			vX=0;
			vY=1;
		}else{	
			keyAffects=1;
		}
	}
	if(loop%10 == 0)
	{
		keyAffects = 1;

		matrixClearAll();
		
		matrixSet(fruitX, fruitY);

		for(i=snakeLen;i>=1;i--){
			snakePosX[i] = snakePosX[i-1];
			snakePosY[i] = snakePosY[i-1];				
		}
				
		snakePosX[0]+=vX;
		snakePosY[0]+=vY;
				
		if(snakePosX[0]==8) snakePosX[0] = 0;
		if(snakePosX[0]==0xFF) snakePosX[0] = 7;

		if(snakePosY[0]==8) snakePosY[0] = 0;
		if(snakePosY[0]==0xFF) snakePosY[0] = 7;
		
		for(i=1;i<snakeLen;i++)
			matrixSet(snakePosX[i], snakePosY[i]);
		
		if(snakePosX[0] == fruitX && snakePosY[0] == fruitY){
			do{
				fruitX = getRandomNumber()>>5;
				fruitY = getRandomNumber()>>5;
			}while(matrixGet(fruitX, fruitY));
			score++;
			snakeLen++;
		}else{	
			if(matrixColPut(1, snakePosX[0], snakePosY[0])) return gameOver();
		}
	}
}

__code const unsigned char song1Notes[]  = "DKAGNAOA EKAGNAOA GKAGNAOA";
__code const unsigned char song1Timing[] = "11111111 11111111 11111111";

void gameGuitar()
{
	// Cada 400 ms
	if(loop%10 == 0){
		if(song1Notes[currentNote] == ' '){
			soundStop();
			currentNote++;
			currentNote%=78;									
		}
		if(song1Notes[currentNote] != ' ')
			soundPlay(song1Notes[currentNote]-'A');
		currentTiming++;
			
		if(currentTiming == song1Timing[currentNote]-'0'){
			currentTiming = 0;
			currentNote++;
			currentNote%=62;					
		}
	}	
}

void gameLogic() __interrupt(TF1_VECTOR) __using(0)
{
	if(state == STATE_LIST){
		gameList();
	}else if(state == STATE_GAMEOVER){
		if(BTN_UP || BTN_DOWN) state = STATE_LIST;
	}else if(state == STATE_TETRIS){	
		gameTetris();
	}else if(state == STATE_CAR){
		gameCar();
	}else if(state == STATE_CHOPPER){
		gameChopper();
	}else if(state == STATE_SNAKE){
		gameSnake();
	}else if(state == STATE_GUITAR){
		gameGuitar();
	}
	
	// Incrementamos el contador de veces que se ejecutó esta función
	loop++;
	
	// Reseteamos el valor del timer	
	TL1 = (0xFFFF-GAMELOGIC_US) & 0xFF;
	TH1 = (0xFFFF-GAMELOGIC_US) >> 8; 
} 
