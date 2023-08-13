
#define BitSync P1_0
#define BitColor P3_0

__idata __at(0x80) unsigned char frameBuffer[30*4];
unsigned char line=0;

__bit bit00, bit01, bit02, bit03, bit04, bit05, bit06, bit07, 
	  bit08, bit09, bit10, bit11, bit12, bit13, bit14, bit15, 
	  bit16, bit17, bit18, bit19, bit20, bit21, bit22, bit23, 
	  bit24, bit25, bit26, bit27, bit28, bit29, bit30, bit31,
	  lineHigh;
unsigned char __at(0x20) currentByte[4];

#define tvMatrixData(x) frameBuffer[(x)*4+9] 

void matrixClearAll()
{
	tvMatrixData(0) = tvMatrixData(1) = tvMatrixData(2) = tvMatrixData(3) = 0;
	tvMatrixData(4) = tvMatrixData(5) = tvMatrixData(6) = tvMatrixData(7) = 0;
}

// Setea (prende) un pixel de la matriz
void matrixSet(unsigned char x, unsigned char y)
{
	tvMatrixData(y) |= (1<<(7-x));
}

// Clearea (apaga) un pixel de la matriz
void matrixClr(unsigned char x, unsigned char y)
{
	tvMatrixData(y) &= ~(1<<(7-x));
}

// Togglea (invierte) un pixel de la matriz
void matrixTgl(unsigned char x, unsigned char y)
{
	tvMatrixData(y) ^= (1<<(7-x));
}

// Devuelve el estado de un pixel de la matriz
unsigned char matrixGet(unsigned char x, unsigned char y)
{
	return tvMatrixData(y) & (1<<(7-x));
}

// Settea el valor de una fila
void matrixSetRow(unsigned char y, unsigned char value)
{
	tvMatrixData(y) = value;
}

// Orea el valor actual de una fila con otro
void matrixOrRow(unsigned char y, unsigned char value)
{
	tvMatrixData(y) |= value;
}

// Deuvelve el valor de una fila
unsigned char matrixGetRow(unsigned char y)
{
	return tvMatrixData(y);
}


// Apaga o prende (dependiendo de value) un pixel, devuelve el valor en el que estaba antes
unsigned char matrixColPut(unsigned char value, unsigned int x, unsigned int y)
{
	unsigned char retv=0;
	if(tvMatrixData(y) & (1<<(7-x))) retv = 1;
	
	if(value)
	{
		tvMatrixData(y) |= (1<<(7-x));
	}
	else
	{
		tvMatrixData(y) &= ~(1<<(7-x));
	}
	return retv;
}

void tvInit(){
	unsigned char i;
	frameBuffer[0] = frameBuffer[1] = frameBuffer[2] = frameBuffer[3] = 0x00;
	frameBuffer[116] = frameBuffer[117] = frameBuffer[118] = frameBuffer[119] = 0x00;
	for(i=4;i<116;i+=4) frameBuffer[i] = 0x7F,frameBuffer[i+1] = 0xFF, frameBuffer[i+2] = 0xFF, frameBuffer[i+3] = 0xEF;
}
