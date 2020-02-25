//////////////////////////////////////////////////////////////////////////
//name:		DNDSound imp
//author:	Lveyou
//date:		18-08-09

//other:
//18-08-09: 实现 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_SOUND_IMP_H_
#define _DND_SOUND_IMP_H_

#include <..\dx11_sdk\include\XAudio2.h>
#include <..\dx11_sdk\include\X3DAudio.h>
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
		virtual void PlayCheck() override;
		virtual void Pause() override;
		virtual void SetVolume(float volume) override;
		

		//virtual void _run() override;

		IXAudio2SourceVoice* _sourceVoice;
		VoiceCallback* _voiceCallBack;
		bool _ready;
		UINT32 _groupID;
		//
		X3DAUDIO_EMITTER* _emitter;
		

		Voice_imp()
		{	
			_sourceVoice = NULL;
			_voiceCallBack = NULL;
			_ready = true;
			_groupID = 0;

			_emitter = NULL;
		}

		~Voice_imp()
		{
			delete _voiceCallBack;
			if(_sourceVoice)
				_sourceVoice->DestroyVoice();

			delete _emitter;
		}
	};

	//回调
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
			//停止循环
			_voice->_sourceVoice->Stop(0);
			//指明它已经播放结束了
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
		virtual Voice* Create(const String& name, UINT32 group_id = 0, bool dsp = false, float scaler = 64.0f) override;
		virtual void Delete(Voice* voice, bool wait_end = true) override;
		virtual void SetVolume(float v, UINT32 group_id = 0) override;
		virtual float GetVolume(UINT32 group_id = 0) override;
		virtual Voice* GetVoice(const String& name) override;

		virtual void UpdateDSP(Voice* voice,
			Vector3 voice_pos, Vector3 listener_pos, 
			Vector3 voice_v = Vector3(), Vector3 listener_v = Vector3()) override;

		void _init();
		Sound_imp()
		{
			_xaudio2 = NULL;
			_masterVoice = NULL;
			_open = false;
			_inited = false;
		}
	public:
		IXAudio2* _xaudio2;
		IXAudio2MasteringVoice* _masterVoice;

		//子混音
		IXAudio2SubmixVoice * _submixVoice[2];

		//0为音效、1为音乐（group id）
		XAUDIO2_SEND_DESCRIPTOR _sendArray0[1];
		XAUDIO2_SEND_DESCRIPTOR _sendArray1[1];
		XAUDIO2_VOICE_SENDS _voiceSends[2];

		//设备信息
		XAUDIO2_DEVICE_DETAILS _deviceDetails;
		X3DAUDIO_HANDLE _x3dInstance;

		//name -> data
		map<String, VoiceData*> _mapVoiceData;

		//所有音频
		map<String, list<Voice_imp*>> _mapAllVoice;
		
		//删除列表
		list<Voice_imp*> _listDelete;

		//听者信息
		X3DAUDIO_LISTENER _listener;
		X3DAUDIO_DSP_SETTINGS _dsp;

		bool _open;
		bool _inited;
	};
}

#endif