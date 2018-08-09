//////////////////////////////////////////////////////////////////////////
//name:		DNDSound imp
//author:	Lveyou
//data:		18-08-09

//other:
//18-08-09: й╣ож - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SOUND_IMP_H_
#define _DND_SOUND_IMP_H_

#include <..\dx11_sdk\include\XAudio2.h>
#include <map>
using namespace std;

#include "DNDSound.h"



namespace DND
{
	class VoiceData
	{
	public:	
		WAVEFORMATEXTENSIBLE _wfx;
		XAUDIO2_BUFFER _buffer;
		VoiceData()
		{
			ZeroMemory(&_wfx, sizeof(_wfx));
			ZeroMemory(&_buffer, sizeof(_buffer));
		}
	};

	class Voice_imp : public Voice
	{
	public:
		virtual void Play() override;

		IXAudio2SourceVoice* _sourceVoice;
		Voice_imp()
		{	
			_sourceVoice = NULL;
		}
	};

	class Sound_imp : public Sound
	{
	public:
		virtual void SetOpen(bool open = true) override;
		virtual void Load(const String& name, const String& path) override;
		virtual Voice* Create(const String& name) override;
		virtual void SetVolume(float v) override;

		void _init();
		Sound_imp()
		{
			_xaudio2 = NULL;
			_masterVoice = NULL;
			_open = false;
		}
	public:
		IXAudio2* _xaudio2;
		IXAudio2MasteringVoice* _masterVoice;

		//name -> data
		map<String, VoiceData*> _mapVoiceData;

		bool _open;
	};
}

#endif