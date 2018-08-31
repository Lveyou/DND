//////////////////////////////////////////////////////////////////////////
//name:		DNDSound
//author:	Lveyou
//date:		17-11-11

//other:
//17-11-11: �򵥷�װzplaylib�� - Lveyou
//18-08-09: ��Ϊ XAudio2 ���� wav�ļ� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SOUND_H_
#define _DND_SOUND_H_


#include "DNDDLL.h"
#include "DNDTypedef.h"
#include "DNDString.h"


#define snd_safe(snd, func1) if(snd)snd->func1;

namespace DND
{
	class DLL_API Voice
	{
	public:
		virtual void Play() = 0;
	};

	class DLL_API Sound
	{
	public:
		virtual void SetOpen(bool open = true) = 0;
		virtual void Load(const String& name, const String& path) = 0;
		virtual Voice* Create(const String& name) = 0;
		virtual void SetVolume(float v) = 0;
	};
}

#endif