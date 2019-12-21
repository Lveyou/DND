//////////////////////////////////////////////////////////////////////////
//name:		DNDSound imp
//author:	Lveyou
//date:		18-08-09

//other:
//18-08-09: ʵ�� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SOUND_IMP_H_
#define _DND_SOUND_IMP_H_

#include <..\dx11_sdk\include\XAudio2.h>
#include <map>
#include <list>
using namespace std;

#include "DNDSound.h"
#include "DNDThread.h"


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

	class VoiceCallback;
	class Voice_imp : public Voice//, public Thread
	{
	public:
		virtual void Play() override;
		virtual void Pause() override;

		//virtual void _run() override;

		IXAudio2SourceVoice* _sourceVoice;
		VoiceCallback* _voiceCallBack;
		bool _ready;

		Voice_imp()
		{	
			_sourceVoice = NULL;
			_voiceCallBack = NULL;
			_ready = true;
		}
	};

	//�ص�
	class VoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		Voice_imp* _voice;
		HANDLE hBufferEndEvent;
		VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
		~VoiceCallback() { CloseHandle(hBufferEndEvent); }

		//Called when the voice has just finished playing a contiguous audio stream.
		void STDMETHODCALLTYPE OnStreamEnd()
		{
			SetEvent(hBufferEndEvent);
		}

		//Unused methods are stubs
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() { }
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
		void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) { }
		void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {    }
		void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) 
		{    
			//ֹͣѭ��
			_voice->_sourceVoice->Stop(0);
			//ָ�����Ѿ����Ž�����
			_voice->_ready = true;
		}
		void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) { }
	};

	

	class Sound_imp : public Sound
	{
	public:
		virtual void SetOpen(bool open = true) override;
		virtual bool IsOpen() override;
		virtual void Load(const String& name, const String& path) override;
		virtual Voice* Create(const String& name) override;
		virtual void SetVolume(float v) override;
		virtual float GetVolume() override;
		Voice* GetVoice(const String& name) override;
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

		//������Ƶ
		map<String, list<Voice_imp*>> _mapAllVoice;
		

		bool _open;
	};
}

#endif