#define SOUND_PIN P3_7

void soundIsr() __interrupt(TF2_VECTOR) __using(0)
{
	SOUND_PIN = !SOUND_PIN;
	TF2 = 0;	
}

void soundStop()
{
	ET2 = 0;
	SOUND_PIN = 0;
}

__code const unsigned short soundNotes[] = 
{
	0xFFFF- 500000.0/440.0,  // A - 440Hz
	0xFFFF- 500000.0/493.0,  // B
	0xFFFF- 500000.0/523.0, // C
	0xFFFF- 500000.0/587.0, // D
	0xFFFF- 500000.0/659.0, // E
	0xFFFF- 500000.0/698.0, // F
	0xFFFF- 500000.0/783.0, // G
	0xFFFF- 500000.0/880.0, // aH (H)
	0xFFFF- 500000.0/986.0, // bH (I)
	0xFFFF- 500000.0/1046.0, // cH (J)
	0xFFFF- 500000.0/1174.0, // dH (K)
	0xFFFF- 500000.0/1318.0, // eH (L)
	0xFFFF- 500000.0/1396.0, // fH (M)
	0xFFFF- 500000.0/1566.0, // gH (N)
	0xFFFF- 500000.0/740.0, // F# (O)
};

void soundPlay(unsigned char note)
{
	RCAP2H = soundNotes[note] >> 8;
	RCAP2L = soundNotes[note] & 0xFF;
	ET2 = 1;
}