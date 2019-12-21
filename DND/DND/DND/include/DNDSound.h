//////////////////////////////////////////////////////////////////////////
//name:		DNDSound
//author:	Lveyou
//date:		17-11-11

//other:
//17-11-11: 简单封装zplaylib库 - Lveyou
//18-08-09: 改为 XAudio2 播放 wav文件 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SOUND_H_
#define _DND_SOUND_H_


#include "DNDDLL.h"
#include "DNDTypedef.h"
#include "DNDString.h"
#include "DNDGame.h"



namespace DND
{
	

	class DLL_API Voice
	{
	public:
		virtual void Play() = 0;
		virtual void Pause() = 0;
	};

	class DLL_API Sound
	{
	public:
		virtual void SetOpen(bool open = true) = 0;
		virtual bool IsOpen() = 0;
		virtual void Load(const String& name, const String& path) = 0;
		virtual Voice* Create(const String& name) = 0;
		virtual void SetVolume(float v) = 0;
		virtual float GetVolume() = 0;
		//可能比create更快
		virtual Voice* GetVoice(const String& name) = 0;
	};

	inline void snd_safe_play(const String& name)
	{
		auto* voice = Game::Get()->sound->GetVoice(name);
		if (voice)
			voice->Play();
	}
}

#endif