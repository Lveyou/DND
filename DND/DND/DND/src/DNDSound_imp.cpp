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

	DND::Voice* Sound_imp::Create(const String& name, UINT32 group_id, bool dsp, float scaler)
	{
		if (!_open)
			return NULL;

		Voice_imp* ret = new Voice_imp;
		ret->_voiceCallBack = new VoiceCallback; 
		ret->_voiceCallBack->_voice = ret;
		ret->_groupID = group_id;

		auto& iter = _mapVoiceData.find(name);

		if (iter == _mapVoiceData.end())
		{
			debug_err(String(L"DND: Sound::Create: 音频没有加载: ") + name);
			delete ret;
			return NULL;
		}


		if (FAILED(_xaudio2->CreateSourceVoice(&(ret->_sourceVoice), (WAVEFORMATEX*)&(iter->second->_wfx),
			0, XAUDIO2_DEFAULT_FREQ_RATIO, ret->_voiceCallBack, &_voiceSends[group_id], NULL)))
		{
			debug_err(String(L"DND: Sound::Create: 创建SourceVoice失败: ") + name);
			delete ret;
			return NULL;
		}

		
		if (FAILED(ret->_sourceVoice->SubmitSourceBuffer(&(iter->second->_buffer))))
		{
			debug_err(String(L"DND: Sound::Create: 提交SourceBuffer失败: ") + name);
			delete ret;
			return NULL;
		}
		
		//dsp
		if (dsp)
		{
			ret->_emitter = new X3DAUDIO_EMITTER;
			ZeroMemory(ret->_emitter, sizeof(X3DAUDIO_EMITTER));
			ret->_emitter->ChannelCount = 1;
			ret->_emitter->CurveDistanceScaler = scaler;
			
			

			ret->_emitter->OrientFront = {0.0f, 0.0f, 1.0f};
			//ret->_emitter->OrientTop = { 0.0f, -1.0f, 0.0f };
			
		}

		return ret;
	}

	void Sound_imp::SetVolume(float v, UINT32 group_id)
	{
		if (!_open)
			return;
		//_masterVoice->SetVolume(v);
		_submixVoice[group_id]->SetVolume(v);
	}

	float Sound_imp::GetVolume(UINT32 group_id)
	{
		if (!_open)
			return 0;
		float ret;
		_submixVoice[group_id]->GetVolume(&ret);
		//_masterVoice->GetVolume(&ret);
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

	void Sound_imp::UpdateDSP(Voice* voice, Vector3 voice_pos, Vector3 listener_pos, Vector3 voice_v /*= Vector3()*/, Vector3 listener_v /*= Vector3()*/)
	{
		Voice_imp* p = (Voice_imp*)voice;
		if (p && p->_emitter)
		{
			//设置速度与位置
			p->_emitter->Position.x = voice_pos.a;
			p->_emitter->Position.y = voice_pos.b;
			p->_emitter->Position.z = voice_pos.c;

			p->_emitter->Velocity.x = voice_v.a;
			p->_emitter->Velocity.y = voice_v.b;
			p->_emitter->Velocity.z = voice_v.c;

			_listener.Position.x = listener_pos.a;
			_listener.Position.y = listener_pos.b;
			_listener.Position.z = listener_pos.c;

			_listener.Velocity.x = listener_v.a;
			_listener.Velocity.y = listener_v.b;
			_listener.Velocity.z = listener_v.c;

			//计算
			X3DAudioCalculate(_x3dInstance, &_listener, p->_emitter,
				X3DAUDIO_CALCULATE_MATRIX |
				X3DAUDIO_CALCULATE_DOPPLER |
				X3DAUDIO_CALCULATE_LPF_DIRECT |
				X3DAUDIO_CALCULATE_REVERB,
				&_dsp);

			//应用效果
			p->_sourceVoice->SetOutputMatrix(_submixVoice[p->_groupID], 1, _deviceDetails.OutputFormat.Format.nChannels, _dsp.pMatrixCoefficients);
			p->_sourceVoice->SetFrequencyRatio(_dsp.DopplerFactor);

			//p->_sourceVoice->SetOutputMatrix(_submixVoice[p->_groupID], 1, 1, &_dsp.ReverbLevel);
			////_submixVoice[p->_groupID]
			/*XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * _dsp.LPFDirectCoefficient), 1.0f };
			p->_sourceVoice->SetFilterParameters(&FilterParameters);*/
		}
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

		//Dx版本的使用下列方法获取ChannelMask（不同于更高版本）
		_xaudio2->GetDeviceDetails(0, &_deviceDetails);

		//子混音（应和主混音通道数一致）
		if (FAILED(hr = _xaudio2->CreateSubmixVoice(&(_submixVoice[0]),
			_deviceDetails.OutputFormat.Format.nChannels,
			44100, 0, 0, 0)))
		{
			dnd_assert(0, L"DND: XAudio2 创建 SubmixVoice0 失败！");
			return;
		}
		if (FAILED(hr = _xaudio2->CreateSubmixVoice(&(_submixVoice[1]), 
			_deviceDetails.OutputFormat.Format.nChannels,
			44100, 0, 0, 0)))
		{
			dnd_assert(0, L"DND: XAudio2 创建 SubmixVoice1 失败！");
			return;
		}
		_sendArray0[0] = { 0, _submixVoice[0] };
		_sendArray1[0] = { 0, _submixVoice[1] };
		_voiceSends[0] = { 1, _sendArray0 };
		_voiceSends[1] = { 1, _sendArray1 };

		

		//初始化X3DAudio
		X3DAudioInitialize(_deviceDetails.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, _x3dInstance);

		//听者只有一个
		ZeroMemory(&_listener, sizeof(X3DAUDIO_LISTENER));
		_listener.OrientFront = { 0.0f,0.0f,-1.0f };
		//_listener.OrientTop = { 0.0f,-1.0f,0.0f };
		//dsp
		ZeroMemory(&_dsp, sizeof(X3DAUDIO_DSP_SETTINGS));
		FLOAT32 * matrix = new FLOAT32[_deviceDetails.OutputFormat.Format.nChannels];
		_dsp.SrcChannelCount = 1;
		_dsp.DstChannelCount = _deviceDetails.OutputFormat.Format.nChannels;
		_dsp.pMatrixCoefficients = matrix;


	}

	void Sound_imp::Delete(Voice* voice, bool wait_end)
	{
		if (!voice)
			return;

		Voice_imp* p = (Voice_imp*)voice;

		if (!wait_end || p->_ready)
		{
			delete p;
		}
		else
		{
			_listDelete.push_back(p);
		}
	}

	void Voice_imp::Play()
	{
		_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
		_ready = false;

		/*WaitForSingleObject(_voiceCallBack->hBufferEndEvent, INFINITE);
		Thread::Start();*/
		//_sourceVoice->SetChannelVolumes(,)
	}


	void Voice_imp::PlayCheck()
	{
		if (_ready)
		{
			_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
			_ready = false;
		}
	}

	void Voice_imp::Pause()
	{
		_sourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);

	}

	void Voice_imp::SetVolume(float volume)
	{
		_sourceVoice->SetVolume(volume);
	}


	/*void Voice_imp::_run()
	{

	}*/

}

