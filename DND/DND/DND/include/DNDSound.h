//////////////////////////////////////////////////////////////////////////
//name:		DNDSound
//author:	Lveyou
//data:		17-11-11

//other:
//17-11-11: ¼òµ¥·â×°zplaylib¿â - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SOUND_H_
#define _DND_SOUND_H_


#include "DNDDLL.h"
#include "DNDTypedef.h"
#include "DNDString.h"

namespace DND
{
	class DLL_API Sound
	{
	public:
		static Sound* Create(const String& path);
		void Replay();
		void Play();
		void Pause();
		void Resume();
		void Stop();
		void SetRate(UINT32 rate);
		void SetVolume(UINT32 left, UINT32 right);
		void GetVolume(UINT32& left, UINT32& right);
		~Sound();
	private:
		void* _player;
	};
}

#endif