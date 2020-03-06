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
#include "DNDGame.h"
#include "DNDDebug.h"



namespace DND
{
	

	class DLL_API Voice
	{
		friend class Voice_imp;
	public:
		virtual void Play() = 0;
		//��ͣ����Ч���������Play����PlayCheck����֡����ѭ������
		virtual void PlayCheck() = 0;
		virtual void Pause() = 0;
		virtual void SetVolume(float volume) = 0;
		
	private:
		virtual ~Voice() {}
	};

	class DLL_API Sound
	{
	public:
		virtual void SetOpen(bool open = true) = 0;
		virtual bool IsOpen() = 0;
		virtual void Load(const String& name, const String& path) = 0;
		virtual Voice* Create(const String& name,UINT32 group_id = 0, bool dsp = false, float scaler = 64.0f) = 0;
		virtual void Delete(Voice* voice, bool wait_end = true) = 0;
		virtual void SetVolume(float v, UINT32 group_id = 0) = 0;
		virtual float GetVolume(UINT32 group_id = 0) = 0;
		//���ܱ�create����
		virtual Voice* GetVoice(const String& name) = 0;
		virtual void UpdateDSP(Voice* voice,
			Vector3 voice_pos, Vector3 listener_pos,
			Vector3 voice_v = Vector3(), Vector3 listener_v = Vector3()) = 0;
	};

	inline void snd_play(const String& name)
	{
		auto* voice = Game::Get()->sound->GetVoice(name);
		if (voice)
			voice->Play();
#ifndef DND_NO_DEBUG
		else
			debug_err(String(L"��Ч��ȡʧ��") + name);
#endif // !DND_NO_DEBUG

		
	}
}

#endif