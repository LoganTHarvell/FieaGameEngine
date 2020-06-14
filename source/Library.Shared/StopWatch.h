#pragma once

#include <chrono>

namespace Library
{
	class StopWatch final
	{
	public:
		StopWatch() = default;
		~StopWatch() = default;
		StopWatch(const StopWatch&) = delete;		
		StopWatch(StopWatch&&) = default;
		StopWatch& operator=(const StopWatch&) = delete;
		StopWatch& operator=(StopWatch&&) = default;

		const std::chrono::microseconds& Elapsed() const;
		std::chrono::milliseconds ElapsedMilliseconds() const;
		std::chrono::seconds ElapsedSeconds() const;
		bool IsRunning() const;

		void Reset();
		void Restart();
		void Start();
		void Stop();

	private:
		std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::microseconds mElapsedTime{ 0 };
		bool mIsRunning{ false };
	};
}

#include "StopWatch.inl"