#include "DNDSound.h"
#include "DNDDebug.h"

//#define LIB_ZPLAY_STATIC
#include <libzplay.h>
using namespace libZPlay;

namespace DND
{

	Sound* Sound::Create(const String& path)
	{

		ZPlay *player = ::CreateZPlay();
		//player->
	
		if (!player->OpenFileW(path.GetWcs(), sfAutodetect))
		{
			debug_warn(String(L"DND: ¼ÓÔØSoundÊ§°Ü: ") + path);
			return NULL;
		}

		Sound* snd = new Sound;
		snd->_player = player;
		
		return snd;
	}

	void Sound::Replay()
	{
		((ZPlay*)_player)->Stop();
		((ZPlay*)_player)->Play();
	}

	void Sound::Play()
	{
		((ZPlay*)_player)->Play();
	}

	void Sound::Pause()
	{
		((ZPlay*)_player)->Pause();
	}

	void Sound::Resume()
	{
		((ZPlay*)_player)->Resume();
	}

	void Sound::Stop()
	{
		((ZPlay*)_player)->Stop();
	}

	void Sound::SetRate(UINT32 rate)
	{
		((ZPlay*)_player)->SetRate(rate);
	}


	void Sound::SetVolume(UINT32 left, UINT32 right)
	{
		((ZPlay*)_player)->SetPlayerVolume(left, right);
	}

	void Sound::GetVolume(UINT32& left, UINT32& right)
	{
		((ZPlay*)_player)->GetPlayerVolume(&left, &right);
	}

	Sound::~Sound()
	{
		((ZPlay*)_player)->Close();
		((ZPlay*)_player)->Release();
		delete _player;
	}

}

