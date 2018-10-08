// 
// 
// 

#include "Colors.h"

void PrintLUT()
{
	Serial.println("const int SinLUT[360] =");
	Serial.print("{");

	// Print values
	int i = 0;
	for (float a = -PI; a < PI; a += PI / 120, i++)
	{
		if (i % 10 == 0)
			Serial.print("\n\t");

		int v = int((cos(a) + 1) * PWMRANGE / 2 + 0.5);
		Serial.print(v);
		Serial.print(",");
		if (v < 10)
			Serial.print(" ");
		if (v < 100)
			Serial.print(" ");
		if (v < 1000)
			Serial.print(" ");
	}

	// Print zeroes
	for (; i < 360; i++)
	{
		if (i % 10 == 0)
			Serial.print("\n\t0,   ");
		else
			Serial.print("0,   ");
	}

	Serial.println("\n};");
}
