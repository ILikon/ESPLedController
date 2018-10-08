// LedController.h

#ifndef _LEDCONTROLLER_h
#define _LEDCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Colors.h"

class LedControllerClass
{
 public:
	LedControllerClass();

	void Parse(uint8_t* msg);
	void Loop();
	void UpdateLeds();
	String GetInfoMsg() const;

protected:
	enum ColorMode
	{
		Equal,
		Oposite,
		Unrelated,
	};
	enum ColorTransition
	{
		None,
		Rand,
	};
	enum LightMode
	{
		Constant,
		Fade,
		Pulse,
		MusicVis,
		MusicBeat,
		Gaming
	};

	ColorMode	m_ColorMode;
	LightMode	m_LightMode;
	ColorTransition m_ColorTransition;

	RGB m_LedBass;
	RGB m_LedTreb;

	int m_HueBass;
	int m_HueTreb;

	int m_BeatCount;

	int m_IntensityGlobal;
	int m_IntensityBass;
	int m_IntensityTreb;

	unsigned long m_TransitionStart;

	bool m_OnOff;


	struct FadeRGB
	{
		RGB Start;
		RGB Current;
		RGB End;

		void PrepareFade(int hue);
	};

	FadeRGB m_FadeBass;
	FadeRGB m_FadeTreb;

	bool m_GamingStateBass;
	bool m_GamingStateTreb;

	// Process messages
	void OnBassValue(int val);
	void OnTrebleValue(int val);
	void OnBeatDetected();
	void OnColorMode(int val);
	void OnLightMode(int val);
	void OnColorTransition(int val);
	void OnTurnOnOff(int val);
	void OnIntensity(int val);

	// Control data
	void ApplyColorMode();
	int RandomiseDiffColor(int current);
	int OpositeColor(int current);

	void InitFade();
	void UpdateFade();
	void InitPulse();
	void UpdatePulse();
	void InitMusicVis();
	void InitMusicBeat();
	void InitConstant();
	void InitGaming();
	void UpdateGaming();
};

//extern LedControllerClass LedController;

#endif

