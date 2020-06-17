#include "pch.h"
#include "AudioManager.h"


namespace GameDesktop
{
	void AudioManager::Create()
	{
		if (!mInstance) mInstance = new AudioManager();
	}

	void AudioManager::Destroy()
	{
		delete mInstance;
		mInstance = nullptr;
	}

	AudioManager* AudioManager::Instance()
	{
		return mInstance;
	}

	void AudioManager::PlaySound(std::string audioClip)
	{
		mEngine->play2D(sounds.At(audioClip).c_str(), false);
	}

	void AudioManager::PlayBackgroundMusic(std::string audioClip)
	{
		mEngine->play2D(sounds.At(audioClip).c_str(), true);
	}

	void AudioManager::StopAllSound()
	{
		mEngine->stopAllSounds();
	}

	GameDesktop::AudioManager::AudioManager()
	{
		// start the sound engine with default parameters
		mEngine = irrklang::createIrrKlangDevice();
	}

	AudioManager::~AudioManager()
	{
		// delete engine
		mEngine->drop();
	}
}

