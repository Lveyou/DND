#include "DNDSound_imp.h"
#include "DNDDebug.h"

namespace DND
{
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK)
		{
			DWORD dwRead;
			if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;

			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) return S_FALSE;

		}


		return S_OK;

	}

	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	void Sound_imp::SetOpen(bool open /*= true*/)
	{
		_open = open;
		
		if(_open)
			_init();
		
	}

	bool Sound_imp::IsOpen()
	{
		return _open;
	}

	


	//////////////////////////////////////////////////////////////////////////
	//以上代码copy自dx sdk文档


	void Sound_imp::Load(const String& name, const String& path)
	{
		if (!_open)
			return;

		HANDLE hFile = CreateFile(
			path.GetWcs(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			debug_err(String(L"DND: 音效文件加载失败: ") + path);
			return;
		}


		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		{
			debug_err(String(L"DND: 音效文件加载失败2: ") + path);
			return;
		}

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE)
		{
			debug_err(String(L"DND: 音效文件加载失败，不是wav文件: ") + path);
			return;
		}

		VoiceData* ret = new VoiceData;


		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &(ret->_wfx), dwChunkSize, dwChunkPosition);


		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		BYTE * pDataBuffer = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		(ret->_buffer).AudioBytes = dwChunkSize;  //buffer containing audio data
		(ret->_buffer).pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		(ret->_buffer).Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		(ret->_buffer).LoopCount = XAUDIO2_LOOP_INFINITE;
		

		_mapVoiceData[name] = ret;

		//至少一个准备播放
		_mapAllVoice[name].push_back((Voice_imp*)Create(name));
		
	}

	DND::Voice* Sound_imp::Create(const String& name)
	{
		if (!_open)
			return NULL;

		Voice_imp* ret = new Voice_imp;
		ret->_voiceCallBack = new VoiceCallback; 
		ret->_voiceCallBack->_voice = ret;

		auto& iter = _mapVoiceData.find(name);

		if (iter == _mapVoiceData.end())
		{
			debug_err(String(L"DND: Sound::Create: 音频没有加载: ") + name);
			return NULL;
		}


		if (FAILED(_xaudio2->CreateSourceVoice(&(ret->_sourceVoice), (WAVEFORMATEX*)&(iter->second->_wfx),
			0, XAUDIO2_DEFAULT_FREQ_RATIO, ret->_voiceCallBack, NULL, NULL)))
		{
			debug_err(String(L"DND: Sound::Create: 创建SourceVoice失败: ") + name);
			return NULL;
		}


		if (FAILED(ret->_sourceVoice->SubmitSourceBuffer(&(iter->second->_buffer))))
		{
			debug_err(String(L"DND: Sound::Create: 提交SourceBuffer失败: ") + name);
			return NULL;
		}
		
		return ret;
	}

	void Sound_imp::SetVolume(float v)
	{
		if (!_open)
			return;
		_masterVoice->SetVolume(v);
	}

	float Sound_imp::GetVolume()
	{
		if (!_open)
			return 0;
		float ret;
		_masterVoice->GetVolume(&ret);
		return ret;
	}

	DND::Voice* Sound_imp::GetVoice(const String& name)
	{
		auto& iter = _mapAllVoice.find(name);
		if (iter == _mapAllVoice.end())
			return NULL;

		for (auto& iter2 : iter->second)
		{
			if (iter2->_ready)
				return iter2;
		}

		Voice* ret = Create(name);
		iter->second.push_back((Voice_imp*)ret);

		return ret;

	}

	void Sound_imp::_init()
	{
		if (_inited)
			return;
		_inited = true;

#ifndef _XBOX
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
		HRESULT hr;
		if (FAILED(hr = XAudio2Create(&_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		{
			dnd_assert(0, L"DND: XAudio2 初始化失败！");
			return;
		}

		//直接使用0，全局音频设备
		if (FAILED(hr = _xaudio2->CreateMasteringVoice(&_masterVoice, XAUDIO2_DEFAULT_CHANNELS,
			XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL)))
		{
			dnd_assert(0, L"DND: XAudio2 创建 MasterVoice 失败！");
			return;
		}

	}

	void Voice_imp::Play()
	{
		_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
		_ready = false;

		/*WaitForSingleObject(_voiceCallBack->hBufferEndEvent, INFINITE);
		Thread::Start();*/

	}


	void Voice_imp::Pause()
	{
		_sourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);

	}

	/*void Voice_imp::_run()
	{

	}*/

}

