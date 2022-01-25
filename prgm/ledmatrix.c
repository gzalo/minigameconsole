// Tiempo por el que se muestra cada fila (en uS)
#define LEDMATRIX_REDRAW_US 200

// Patas que van conectadas al shift register
#define SHIFTREG_DATA P1_0
#define SHIFTREG_CLOCK P1_2

// Valor que deberia tener el SR en cada fila
#define LEDMATRIX_ROW_ON 0
#define LEDMATRIX_ROW_OFF 1

#define SND P1_3

// Matriz de translacion de filas
__code const unsigned char matrixTranslationY[8] = {3,2,1,0,7,6,5,4};

// Fila que esta siendo mostrada
unsigned char matrixPtr = 0;
// Estado de las distintas filas , un 1 en el bit es Encendido, un 0 apagado
// El bit menos significativo es el de la derecha, la fila 0 es la primera
unsigned char matrixData[8];

void matrixIsr() __interrupt(TF0_VECTOR) __using(0)
{
	unsigned char tempPtr = matrixTranslationY[matrixPtr];
	
	// Si estamos en la primer fila, ponemos un "fila encendida" en el SR
	SHIFTREG_DATA = (matrixPtr == 0)?LEDMATRIX_ROW_ON:LEDMATRIX_ROW_OFF;

	// Movemos un paso el SR
	SHIFTREG_CLOCK = 1;
	SHIFTREG_CLOCK = 0;	
	
	// Cargamos los datos para la fila
	P2 = matrixData[tempPtr];
	
	// Pasamos a la proxima fila (y volvemos a cero si = 8)
	matrixPtr++;
	matrixPtr&=7;
	
	// Reseteamos el valor del timer
	TH0 = (0xFFFF - LEDMATRIX_REDRAW_US) >> 8;
	TL0 = (0xFFFF - LEDMATRIX_REDRAW_US) & 0xFF;
}

// Borra la matriz
void matrixClearAll()
{
	matrixData[0] = matrixData[1] = matrixData[2] = matrixData[3] = 
	matrixData[4] = matrixData[5] = matrixData[6] = matrixData[7] = 0;
}

void matrixSetAll()
{
	matrixData[0] = matrixData[1] = matrixData[2] = matrixData[3] = 
	matrixData[4] = matrixData[5] = matrixData[6] = matrixData[7] = 0xFF;
}

void matrixToggle()
{
	matrixData[0] ^= 0xFF; 	matrixData[1] ^= 0xFF;
	matrixData[2] ^= 0xFF; 	matrixData[3] ^= 0xFF;
	matrixData[4] ^= 0xFF; 	matrixData[5] ^= 0xFF;
	matrixData[6] ^= 0xFF; 	matrixData[7] ^= 0xFF;	
}

// Setea (prende) un pixel de la matriz
void matrixSet(unsigned char x, unsigned char y)
{
	matrixData[y] |= (1<<(7-x));
}

// Clearea (apaga) un pixel de la matriz
void matrixClr(unsigned char x, unsigned char y)
{
	matrixData[y] &= ~(1<<(7-x));
}

// Togglea (invierte) un pixel de la matriz
void matrixTgl(unsigned char x, unsigned char y)
{
	matrixData[y] ^= (1<<(7-x));
}

// Devuelve el estado de un pixel de la matriz
unsigned char matrixGet(unsigned char x, unsigned char y)
{
	return matrixData[y] & (1<<(7-x));
}

// Apaga o prende (dependiendo de value) un pixel, devuelve el valor en el que estaba antes
unsigned char matrixColPut(unsigned char value, unsigned int x, unsigned int y)
{
	unsigned char retv=0;
	if(matrixData[y] & (1<<(7-x))) retv = 1;
	
	if(value)
	{
		matrixData[y] |= (1<<(7-x));
	}
	else
	{
		matrixData[y] &= ~(1<<(7-x));
	}
	return retv;
}
