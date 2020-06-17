#include "pch.h"
#include "AudioClip.h"
#include "AudioManager.h"

namespace GameDesktop
{
	AudioClip::AudioClip(std::string name, std::string fileName, bool isBackground) :
		mName(name), mFileName(fileName), mIsBackground(isBackground)
	{
	}

	void AudioClip::Play()
	{
		if (mIsBackground)
		{
			AudioManager::Instance()->PlaySound(*this);
		}
		else
		{
			AudioManager::Instance()->PlayBackgroundMusic(*this);
		}
	}

	const std::string& AudioClip::Name()
	{
		return mName;
	}

	const std::string& AudioClip::FileName()
	{
		return mFileName;
	}

	void AudioClip::SetName(std::string newName)
	{
		mName = newName;
	}

	void AudioClip::SetFileName(std::string newFileName)
	{
		mFileName = newFileName;
	}
}
