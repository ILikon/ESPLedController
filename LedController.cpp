// 
// 
// 

#include "LedController.h"

//#define DEBUG
#ifdef DEBUG
	#define DPRINT(...)    Serial.print(__VA_ARGS__)
	#define DPRINTLN(...)  Serial.println(__VA_ARGS__)
	#define DPRINTF(...)   Serial.printf(__VA_ARGS__)
	#define DINIT()        Serial.begin(115200)
#else
	#define DPRINT(...)
	#define DPRINTLN(...)
	#define DPRINTF(...)
	#define DINIT()
#endif


//LedControllerClass LedController;

LedControllerClass::LedControllerClass()
{
	m_ColorMode = ColorMode::Unrelated;
	m_ColorTransition = ColorTransition::Rand;
	m_LightMode = LightMode::MusicVis;

	m_HueBass = 0;
	m_HueTreb = 0;

	m_BeatCount = 0;

	m_IntensityGlobal = PWMRANGE;
	m_IntensityBass = PWMRANGE;
	m_IntensityTreb = PWMRANGE;

	OnTurnOnOff(0);

	m_TransitionStart = 0;

	m_GamingStateBass = false;
	m_GamingStateTreb = false;
}

void LedControllerClass::Parse(uint8_t* msg)
{
	// Tokenize
	char* pToken = strtok((char*)msg, ";");
	while (pToken)
	{
		// Parse
		switch (pToken[0])
		{
		case 'B':
			OnBassValue(atoi(&pToken[1])); break;
		case 'T':
			OnTrebleValue(atoi(&pToken[1])); break;
		case 'P':
			OnBeatDetected(); break;
		case 'C':
			OnColorMode(atoi(&pToken[1])); break;
		case 'L':
			OnLightMode(atoi(&pToken[1])); break;
		case 'S':
			OnColorTransition(atoi(&pToken[1])); break;
		case 'O':
			OnTurnOnOff(atoi(&pToken[1])); break;
		case 'I':
			OnIntensity(atoi(&pToken[1])); break;
		default:
			break;
		}

		pToken = strtok(nullptr, ";");
	}

	UpdateLeds();
}

void LedControllerClass::Loop()
{
	if (m_LightMode == LightMode::Fade)
		UpdateFade();
	else if (m_LightMode == LightMode::Pulse)
		UpdatePulse();
	else if (m_LightMode == LightMode::Gaming)
		UpdateGaming();

	UpdateLeds();
}

void LedControllerClass::UpdateLeds()
{
	if (!m_OnOff)
		return;

	if (m_LightMode == LightMode::Fade)
	{
		m_LedBass = m_FadeBass.Current;
		m_LedTreb = m_FadeTreb.Current;
	}
	else
	{
		Hue2RGB(m_HueBass, m_LedBass);
		Hue2RGB(m_HueTreb, m_LedTreb);
	}


	if (m_LightMode == LightMode::MusicVis ||
		m_LightMode == LightMode::Pulse)
	{
		m_LedBass *= m_IntensityBass;
		m_LedTreb *= m_IntensityTreb;
	}
	else if (m_LightMode == LightMode::Gaming)
	{
		m_LedBass *= max(m_IntensityBass, 128);
		m_LedTreb *= max(m_IntensityTreb, 128);
	}

	m_LedBass *= m_IntensityGlobal;
	m_LedTreb *= m_IntensityGlobal;

	
	analogWrite(D2, PWMRANGE - m_LedBass.R);
	analogWrite(D1, PWMRANGE - m_LedBass.G);
	analogWrite(D3, PWMRANGE - m_LedBass.B);

	analogWrite(D6, PWMRANGE - m_LedTreb.R);
	analogWrite(D7, PWMRANGE - m_LedTreb.G);
	analogWrite(D5, PWMRANGE - m_LedTreb.B);

	
	//DPRINTF("%d, %d, %d\n", PWMRANGE - m_LedBass.R, PWMRANGE - m_LedBass.G, PWMRANGE - m_LedBass.B);
}

String LedControllerClass::GetInfoMsg() const
{
	String s;
	s += String('O') + (int)m_OnOff;
	s += String(";I") + (int)m_IntensityGlobal;
	s += String(";C") + (int)m_ColorMode;
	s += String(";L") + (int)m_LightMode;
	s += String(";S") + (int)m_ColorTransition + String(';');
	return s;
}

void LedControllerClass::OnBassValue(int val)
{
	m_IntensityBass = val;
	//DPRINTF("R:%d G:%d B:%d\n", m_LedBass.R, m_LedBass.G, m_LedBass.B);
}

void LedControllerClass::OnTrebleValue(int val)
{
	m_IntensityTreb = val;
}

void LedControllerClass::OnBeatDetected()
{
	m_BeatCount++;
	if (m_LightMode == LightMode::MusicBeat && m_BeatCount >= 1 ||
		m_LightMode == LightMode::MusicVis && m_ColorTransition == ColorTransition::Rand && m_BeatCount >= 4)
	{
		m_BeatCount = 0;

		// Change color
		m_HueBass = RandomiseDiffColor(m_HueBass);
		ApplyColorMode();
	}
}

void LedControllerClass::OnColorMode(int val)
{
	m_ColorMode = (ColorMode)val;
	ApplyColorMode();
}

void LedControllerClass::OnLightMode(int val)
{
	m_LightMode = (LightMode)val;

	switch (m_LightMode)
	{
	case LedControllerClass::Constant:
		InitConstant();  break;
	case LedControllerClass::Fade:
		InitFade();  break;
	case LedControllerClass::Pulse:
		InitPulse();  break;
	case LedControllerClass::MusicVis:
		InitMusicVis();  break;
	case LedControllerClass::MusicBeat:
		InitMusicBeat();  break;
	case LedControllerClass::Gaming:
		InitGaming();  break;
	default:
		break;
	}
}

void LedControllerClass::OnColorTransition(int val)
{
	m_ColorTransition = (ColorTransition)val;
}

void LedControllerClass::OnTurnOnOff(int val)
{
	m_OnOff = bool(val);

	// Turn all leds off
	if (!m_OnOff)
	{
		analogWrite(D2, PWMRANGE);
		analogWrite(D1, PWMRANGE);
		analogWrite(D3, PWMRANGE);

		analogWrite(D6, PWMRANGE);
		analogWrite(D7, PWMRANGE);
		analogWrite(D5, PWMRANGE);
	}
}

void LedControllerClass::OnIntensity(int val)
{
	m_IntensityGlobal = val;
}

void LedControllerClass::ApplyColorMode()
{
	if (m_ColorMode == ColorMode::Equal)
		m_HueTreb = m_HueBass;
	else if (m_ColorMode == ColorMode::Oposite)
		m_HueTreb = OpositeColor(m_HueBass);
	else
		m_HueTreb = RandomiseDiffColor(m_HueBass);
}

// choose a random color from default colors distant from the current one
int LedControllerClass::RandomiseDiffColor(int current)
{
	int ind = current / 30;
	ind = (ind + random(1, NUM_COLORS - 1)) % NUM_COLORS;
	return DefaultColors[ind];
}

int LedControllerClass::OpositeColor(int current)
{
	return (current + 180) % 360;
}

void LedControllerClass::InitFade()
{
	m_TransitionStart = millis();
	m_IntensityBass = PWMRANGE;
	m_IntensityTreb = PWMRANGE;

	// Compute next hues first
	m_HueBass = RandomiseDiffColor(m_HueBass);
	ApplyColorMode();

	// Update Fades
	m_FadeBass.PrepareFade(m_HueBass);
	m_FadeTreb.PrepareFade(m_HueTreb);
}

// Do fade by mixing colors with sin wave transition
void LedControllerClass::UpdateFade()
{
	const unsigned long CycleTime = 15000;
	const int MaxStep = 120;

	unsigned long diff = min(millis() - m_TransitionStart, CycleTime);

	int CurrentStep = int(MaxStep * diff / CycleTime);
	MixColors(m_FadeBass.Start, m_FadeBass.End, CurrentStep, m_FadeBass.Current);
	MixColors(m_FadeTreb.Start, m_FadeTreb.End, CurrentStep, m_FadeTreb.Current);

	if (diff == CycleTime)
		InitFade();
}

void LedControllerClass::InitPulse()
{
	m_TransitionStart = millis();
	m_IntensityBass = 0;
	m_IntensityTreb = 0;

	if (m_ColorTransition == ColorTransition::Rand)
	{
		m_HueBass = RandomiseDiffColor(m_HueBass);
		ApplyColorMode();
	}
}

// Pulse intensity with sin wave
void LedControllerClass::UpdatePulse()
{
	const unsigned long CycleTime = 15000;
	const int MaxStep = 240;

	unsigned long diff = min(millis() - m_TransitionStart, CycleTime);

	int CurrentStep = int(MaxStep * diff / CycleTime);
	m_IntensityBass = SinLUT[CurrentStep % MaxStep];
	m_IntensityTreb = m_IntensityBass;

	if (diff == CycleTime)
		InitPulse();

	//DPRINTF("CurrentStep:%d\nIntensity:%d", CurrentStep, m_IntensityBass);
}

void LedControllerClass::InitMusicVis()
{
	m_BeatCount = 0;
	m_IntensityBass = 0;
	m_IntensityTreb = 0;
}

void LedControllerClass::InitMusicBeat()
{
	m_BeatCount = 0;
	m_IntensityBass = PWMRANGE;
	m_IntensityTreb = PWMRANGE;
}

void LedControllerClass::InitConstant()
{
	m_IntensityBass = PWMRANGE;
	m_IntensityTreb = PWMRANGE;
}

void LedControllerClass::InitGaming()
{
	m_IntensityBass = 0;
	m_IntensityTreb = 0;
	m_GamingStateBass = false;
	m_GamingStateTreb = false;
	m_HueBass = Hue::Red;
	m_HueTreb = Hue::Orange;
}

void LedControllerClass::UpdateGaming()
{
	if (m_IntensityBass == 0 && m_GamingStateBass)
	{
		m_GamingStateBass = false;
		m_HueBass = Hue::Red;
	}
	else if (m_IntensityBass != 0 && !m_GamingStateBass)
	{
		m_GamingStateBass = true;
		int h = random(3);
		if (h == 0)
			m_HueBass = Hue::Red;
		else if (h == 1)
			m_HueBass = Hue::Orange;
		else if (h == 2)
			m_HueBass = Hue::Yellow - 10;
	}

	if (m_IntensityTreb == 0 && m_GamingStateTreb)
	{
		m_GamingStateTreb = false;
		m_HueTreb = Hue::Orange;
	}
	else if (m_IntensityTreb != 0 && !m_GamingStateTreb)
	{
		m_GamingStateTreb = true;
		int h = random(3);
		if (h == 0)
			m_HueTreb = Hue::Red;
		else if (h == 1)
			m_HueTreb = Hue::Orange;
		else if (h == 2)
			m_HueTreb = Hue::Yellow - 10;
	}
}

// Prepare next fade transition
void LedControllerClass::FadeRGB::PrepareFade(int hue)
{
	Start = End;
	Current = End;
	Hue2RGB(hue, End);
}
