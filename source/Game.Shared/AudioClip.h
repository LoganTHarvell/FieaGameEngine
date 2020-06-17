#pragma once

#include <string>
#include <irrklang.h>

namespace GameDesktop
{
	class AudioClip
	{
	public:
		/// <summary>
		/// AudioClip Constructor
		/// </summary>
		/// <param name="name">Name of the clip</param>
		/// <param name="fileName">Path to the file for the clip</param>
		/// <param name="isBackground">Whether to this clip is backgroud music</param>
		AudioClip(std::string name, std::string fileName, bool isBackground = false);
		
		AudioClip() = default;
		AudioClip(const AudioClip& other) = default;
		AudioClip(AudioClip&& other) noexcept = default;
		AudioClip& operator=(const AudioClip& other) = default;
		AudioClip& operator=(AudioClip&& other) noexcept = default;
		~AudioClip() = default;

		/// <summary>
		/// Plays the clip
		/// </summary>
		void Play();

		/// <summary>
		/// Gets the name of the clip
		/// </summary>
		/// <returns>The name of the clip</returns>
		const std::string& Name();

		/// <summary>
		/// Gets the file path of the clip
		/// </summary>
		/// <returns>The filepath of the clip</returns>
		const std::string& FileName();

		/// <summary>
		/// Sets the name of the clip
		/// </summary>
		/// <param name="newName">The new name for the clip</param>
		void SetName(std::string newName);

		/// <summary>
		/// Sets the file path of the clip
		/// </summary>
		/// <param name="newFileName">The new file path for the clip</param>
		void SetFileName(std::string newFileName);

	private:
		std::string mName;
		std::string mFileName;
		bool mIsBackground{ false };
	};

	//Convenience Macro
#define DerivedAudioClip(AudioType, ClipFileName, IsBackground)				\
	class AudioType##Sound final : public AudioClip							\
	{																		\
		AudioType##Soud() :													\
			AudioClip(AudioType, ClipFileName, IsBackground);								\
		{																	\
		}																	\
	};
}