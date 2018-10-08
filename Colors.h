// Colors.h

#ifndef _COLORS_h
#define _COLORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// addapted from http://www.instructables.com/id/How-to-Make-Proper-Rainbow-and-Random-Colors-With-/
// Sine wave LUT to build light signal
const int SinLUT[360] =
{
	0,   0,   1,   2,   3,   4,   6,   9,   11,  14,
	17,  21,  25,  29,  34,  39,  44,  50,  56,  62,
	69,  75,  83,  90,  98,  106, 114, 123, 131, 140,
	150, 159, 169, 179, 190, 200, 211, 222, 233, 244,
	256, 267, 279, 291, 303, 316, 328, 341, 353, 366,
	379, 392, 405, 418, 431, 445, 458, 471, 485, 498,
	512, 525, 538, 552, 565, 578, 592, 605, 618, 631,
	644, 657, 670, 682, 695, 707, 720, 732, 744, 756,
	767, 779, 790, 801, 812, 823, 833, 844, 854, 864,
	873, 883, 892, 900, 909, 917, 925, 933, 940, 948,
	954, 961, 967, 973, 979, 984, 989, 994, 998, 1002,
	1006,1009,1012,1014,1017,1019,1020,1021,1022,1023,
	1023,1023,1022,1021,1020,1019,1017,1014,1012,1009,
	1006,1002,998, 994, 989, 984, 979, 973, 967, 961,
	954, 948, 940, 933, 925, 917, 909, 900, 892, 883,
	873, 864, 854, 844, 833, 823, 812, 801, 790, 779,
	767, 756, 744, 732, 720, 707, 695, 682, 670, 657,
	644, 631, 618, 605, 592, 578, 565, 552, 538, 525,
	511, 498, 485, 471, 458, 445, 431, 418, 405, 392,
	379, 366, 353, 341, 328, 316, 303, 291, 279, 267,
	256, 244, 233, 222, 211, 200, 190, 179, 169, 159,
	150, 140, 131, 123, 114, 106, 98,  90,  83,  75,
	69,  62,  56,  50,  44,  39,  34,  29,  25,  21,
	17,  14,  11,  9,   6,   4,   3,   2,   1,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

void PrintLUT();


////////////////////////////////////////

struct RGB
{
	int R;
	int G;
	int B;

	RGB() :R(0), G(0), B(0)
	{};

	RGB(int r, int g, int b) :R(r), G(g), B(b)
	{};

	RGB operator*(int pwmVal) const
	{
		RGB c;
		c.R = R * pwmVal / PWMRANGE;
		c.G = G * pwmVal / PWMRANGE;
		c.B = B * pwmVal / PWMRANGE;
		return c;
	};

	void operator*=(int pwmVal)
	{
		R = R * pwmVal / PWMRANGE;
		G = G * pwmVal / PWMRANGE;
		B = B * pwmVal / PWMRANGE;
	};
};



// Step = 0..120
inline void MixColors(const RGB& Start, const RGB& End, int step, RGB& Mix)
{
	Mix.R = int((Start.R * SinLUT[120 + step] + End.R * SinLUT[step]) / PWMRANGE);
	Mix.G = int((Start.G * SinLUT[120 + step] + End.G * SinLUT[step]) / PWMRANGE);
	Mix.B = int((Start.B * SinLUT[120 + step] + End.B * SinLUT[step]) / PWMRANGE);
}


////////////////////////////////////////

enum Hue
{
	Red = 0,
	Orange = 30,
	Yellow = 60,
	YellowGreen = 90,
	Green = 120,
	GreenCyan = 150,
	Cyan = 180,
	CyanBlue = 210,
	Blue = 240,
	Purple = 270,
	Magenta = 300,
	Pink = 330
};

const int NUM_COLORS = 12;
const int DefaultColors[NUM_COLORS] =
{
	Red, Orange, Yellow, YellowGreen, Green, GreenCyan, Cyan, CyanBlue, Blue, Purple, Magenta, Pink
};

// H = 0-359
inline void Hue2RGB(int h, RGB& C)
{
	C.R = SinLUT[(h + 120) % 360];
	C.G = SinLUT[h];
	C.B = SinLUT[(h + 240) % 360];
}

#endif

