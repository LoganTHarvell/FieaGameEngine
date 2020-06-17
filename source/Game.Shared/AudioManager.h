#pragma once

#include <irrKlang.h>
#include "AudioClip.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "HashMap.h"

namespace GameDesktop
{
	class AudioManager final
	{

	public:
		/// <summary>
		/// Initializes the AudioManager instance, if necessary.
		/// </summary>
		static void Create();

		/// <summary>
		/// Deletes the AudioManager instance, if necessary.
		/// </summary>
		static void Destroy();

		/// <summary>
		/// Gets the AudioManager instance.
		/// </summary>
		/// <returns>AudioManager instance, if it is created. Otherwise, nullptr.</returns>
		static AudioManager* Instance();
		
		void PlaySound(std::string audioClip);
		void PlayBackgroundMusic(std::string audioClip);
		void StopAllSound();
	private:
		/// <summary>
		/// Pointer to a AudioManager instance managed by the singleton.
		/// </summary>
		inline static AudioManager* mInstance{ nullptr };

		AudioManager();
		~AudioManager();
		AudioManager(const AudioManager& other) = delete;
		AudioManager(AudioManager&& other) = delete;
		AudioManager& operator=(const AudioManager& other) = delete;
		AudioManager& operator=(AudioManager&& other) = delete;

		irrklang::ISoundEngine* mEngine{ nullptr };

		inline static Library::HashMap<std::string, std::string> sounds = {
			{"BombExplode", "Content\\Sound\\bomb_explode.wav"},
			{"Death", "Content\\Sound\\death.wav"},
			{"EndMusic", "Content\\Sound\\end_music.mp3"},
			{"Walking", "Content\\Sound\\footsteps.wav"},
			{"GameMusic", "Content\\Sound\\game_music.wav"},
			{"MenuMusic", "Content\\Sound\\menu_music.wav"},
			{"MenuInput", "Content\\Sound\\menu_input.wav"},
			{"MenuSelect", "Content\\Sound\\menu_select.wav"},
			{"PlaceBomb", "Content\\Sound\\place_bomb.wav"},
			{"PowerUp", "Content\\Sound\\powerup.wav"},
		};
	};
}